/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <string>
#include <iomanip>

#include <boost/lexical_cast.hpp>

#include <osquery/filesystem.h>
#include <osquery/hash.h>
#include <osquery/core.h>

#include "osquery/tables/system/darwin/keychain.h"

namespace osquery {
namespace tables {

const std::vector<std::string> kSystemKeychainPaths = {
    "/System/Library/Keychains", "/Library/Keychains",
};

const std::vector<std::string> kUserKeychainPaths = {
    "/Library/Keychains",
};

void genKeychains(const std::string& path, CFMutableArrayRef& keychains) {
  std::vector<std::string> paths;

  // Support both a directory and explicit path search.
  if (isDirectory(path).ok()) {
    // Try to list every file in the given keychain search path.
    if (!listFilesInDirectory(path, paths).ok()) {
      return;
    }
  } else {
    // The explicit path search comes from a query predicate.
    paths.push_back(path);
  }

  for (const auto& keychain_path : paths) {
    SecKeychainRef keychain = nullptr;
    auto status = SecKeychainOpen(keychain_path.c_str(), &keychain);
    if (status == 0 && keychain != nullptr) {
      CFArrayAppendValue(keychains, keychain);
    }
  }
}

std::string getKeychainPath(const SecKeychainItemRef& item) {
  SecKeychainRef keychain = nullptr;
  std::string path;
  auto status = SecKeychainItemCopyKeychain(item, &keychain);
  if (keychain == nullptr || status != errSecSuccess) {
    // Unhandled error, cannot get the keychain reference from certificate.
    return path;
  }

  UInt32 path_size = 1024;
  char keychain_path[1024] = {0};
  status = SecKeychainGetPath(keychain, &path_size, keychain_path);
  if (status != errSecSuccess || (path_size > 0 && keychain_path[0] != 0)) {
    path = std::string(keychain_path);
  }

  CFRelease(keychain);
  return path;
}

std::string genKIDProperty(const unsigned char* data, int len) {
  std::stringstream key_id;
  for (int i = 0; i < len; i++) {
    key_id << std::setw(2) << std::hex << std::setfill('0') << (int)data[i];
  }
  return key_id.str();
}

std::string genAlgProperty(const X509* cert) {
  int nid = 0;
  OSX_OPENSSL(nid = OBJ_obj2nid(cert->cert_info->key->algor->algorithm));
  if (nid == NID_undef) {
    // Unknown algorithm OID.
    return "";
  }

  std::string property;
  OSX_OPENSSL(property = std::string(OBJ_nid2ln(nid)));
  return property;
}

std::string genSHA1ForCertificate(const CFDataRef& raw_cert) {
  return hashFromBuffer(
      HASH_TYPE_SHA1, CFDataGetBytePtr(raw_cert), CFDataGetLength(raw_cert));
}

bool CertificateIsCA(X509* cert) {
  int ca = 0;
  OSX_OPENSSL(ca = X509_check_ca(cert));
  return (ca > 0);
}

std::string genCommonName(X509* cert) {
  if (cert == nullptr) {
    return "";
  }

  X509_NAME* subject_name = nullptr;
  OSX_OPENSSL(subject_name = X509_get_subject_name(cert));
  if (subject_name == nullptr) {
    return "";
  }

  int nid = 0;
  OSX_OPENSSL(nid = OBJ_txt2nid("CN"));

  int index = 0;
  OSX_OPENSSL(index = X509_NAME_get_index_by_NID(subject_name, nid, -1));
  if (index == -1) {
    return "";
  }

  X509_NAME_ENTRY* commonNameEntry = nullptr;
  OSX_OPENSSL(commonNameEntry = X509_NAME_get_entry(subject_name, index));
  if (commonNameEntry == nullptr) {
    return "";
  }

  ASN1_STRING* commonNameData = nullptr;
  OSX_OPENSSL(commonNameData = X509_NAME_ENTRY_get_data(commonNameEntry));

  unsigned char* data = nullptr;
  OSX_OPENSSL(data = ASN1_STRING_data(commonNameData));
  return std::string(reinterpret_cast<char*>(data));
}

std::string genHumanReadableDateTime(ASN1_TIME* time) {
  BIO* bio_stream = nullptr;
  OSX_OPENSSL(bio_stream = BIO_new(BIO_s_mem()));
  if (bio_stream == nullptr) {
    return "";
  }

  // ANS1_TIME_print's format is: Mon DD HH:MM:SS YYYY GMT
  // e.g. Jan 1 00:00:00 1970 GMT (always GMT)
  auto buffer_size = 32;
  char buffer[32] = {0};
  OSX_OPENSSL(if (!ASN1_TIME_print(bio_stream, time)) {
    BIO_free(bio_stream);
    return "";
  });

  // BIO_gets() returns amount of data successfully read or written
  // (if the return value is positive) or that no data was successfully
  // read or written if the result is 0 or -1.
  OSX_OPENSSL(if (BIO_gets(bio_stream, buffer, buffer_size) <= 0) {
    BIO_free(bio_stream);
    return "";
  });

  OSX_OPENSSL(BIO_free(bio_stream));
  return std::string(buffer);
}

time_t genEpoch(ASN1_TIME* time) {
  auto datetime = genHumanReadableDateTime(time);
  if (datetime.empty()) {
    return -1;
  }

  time_t epoch;
  struct tm tm;
  // b := abbr month, e := day with leading space instead of leading zero
  if (strptime(datetime.c_str(), "%b %e %H:%M:%S %Y %Z", &tm) == nullptr) {
    return -1;
  }

  // Don't set DST, since strptime() doesn't.
  // Let mktime() determine whether DST in effect
  tm.tm_isdst = -1;
  epoch = mktime(&tm);
  if (epoch == -1) {
    return -1;
  }
  return epoch;
}

// Key Usages (i.e. Digital Signature, CRL Sign etc) in ASN1/OpenSSL
// are represented as flags. These are then set by doing bitwise OR ops.
// genKeyUsage() reverses this to figure out which key usages are set.
std::string genKeyUsage(unsigned long flag) {
  if (flag == 0) {
    return "";
  }
  std::vector<std::string> results;
  for (const auto& key : kKeyUsageFlags) {
    if (flag & key.first) {
      results.push_back(key.second);
    }
  }
  return osquery::join(results, ", ");
}

CFArrayRef CreateKeychainItems(const std::set<std::string>& paths,
                               const CFTypeRef& item_type) {
  auto keychains = CFArrayCreateMutable(nullptr, 0, &kCFTypeArrayCallBacks);
  for (const auto& path : paths) {
    genKeychains(path, keychains);
  }

  CFMutableDictionaryRef query;
  query = CFDictionaryCreateMutable(nullptr,
                                    0,
                                    &kCFTypeDictionaryKeyCallBacks,
                                    &kCFTypeDictionaryValueCallBacks);
  CFDictionaryAddValue(query, kSecClass, item_type);
  CFDictionaryAddValue(query, kSecReturnRef, kCFBooleanTrue);
  // This can be added to restrict results to x509v3
  // CFDictionaryAddValue(query, kSecAttrCertificateType, 0x03);
  CFDictionaryAddValue(query, kSecMatchSearchList, keychains);
  CFDictionaryAddValue(query, kSecAttrCanVerify, kCFBooleanTrue);
  CFDictionaryAddValue(query, kSecMatchLimit, kSecMatchLimitAll);

  CFArrayRef keychain_certs;
  auto status = SecItemCopyMatching(query, (CFTypeRef*)&keychain_certs);
  CFRelease(query);

  if (status != errSecSuccess) {
    return nullptr;
  }

  // Release each keychain search path.
  CFRelease(keychains);

  return keychain_certs;
}

std::set<std::string> getKeychainPaths() {
  std::set<std::string> keychain_paths;

  for (const auto& path : kSystemKeychainPaths) {
    keychain_paths.insert(path);
  }

  auto homes = getHomeDirectories();
  for (const auto& dir : homes) {
    for (const auto& keychains_dir : kUserKeychainPaths) {
      keychain_paths.insert((dir / keychains_dir).string());
    }
  }

  return keychain_paths;
}
}
}