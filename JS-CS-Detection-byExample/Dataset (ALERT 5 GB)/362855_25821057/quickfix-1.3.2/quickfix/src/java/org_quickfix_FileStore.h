/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_quickfix_FileStore */

#ifndef _Included_org_quickfix_FileStore
#define _Included_org_quickfix_FileStore
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_quickfix_FileStore
 * Method:    create
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_create
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_destroy
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    set0
 * Signature: (ILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_quickfix_FileStore_set0
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     org_quickfix_FileStore
 * Method:    get0
 * Signature: (ILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_quickfix_FileStore_get0__ILjava_lang_String_2
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     org_quickfix_FileStore
 * Method:    get0
 * Signature: (IILjava/util/Collection;)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_get0__IILjava_util_Collection_2
  (JNIEnv *, jobject, jint, jint, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    getNextSenderMsgSeqNum0
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_quickfix_FileStore_getNextSenderMsgSeqNum0
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    getNextTargetMsgSeqNum0
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_quickfix_FileStore_getNextTargetMsgSeqNum0
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    setNextSenderMsgSeqNum0
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_setNextSenderMsgSeqNum0
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_quickfix_FileStore
 * Method:    setNextTargetMsgSeqNum0
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_setNextTargetMsgSeqNum0
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_quickfix_FileStore
 * Method:    incrNextSenderMsgSeqNum0
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_incrNextSenderMsgSeqNum0
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    incrNextTargetMsgSeqNum0
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_incrNextTargetMsgSeqNum0
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    getCreationTime0
 * Signature: ()Ljava/util/Date;
 */
JNIEXPORT jobject JNICALL Java_org_quickfix_FileStore_getCreationTime0
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileStore
 * Method:    reset0
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileStore_reset0
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif