/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_quickfix_FileLog */

#ifndef _Included_org_quickfix_FileLog
#define _Included_org_quickfix_FileLog
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_quickfix_FileLog
 * Method:    create
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileLog_create
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileLog
 * Method:    destroy
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileLog_destroy
  (JNIEnv *, jobject);

/*
 * Class:     org_quickfix_FileLog
 * Method:    onIncoming0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileLog_onIncoming0
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_quickfix_FileLog
 * Method:    onOutgoing0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileLog_onOutgoing0
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_quickfix_FileLog
 * Method:    onEvent0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_quickfix_FileLog_onEvent0
  (JNIEnv *, jobject, jstring);

#ifdef __cplusplus
}
#endif
#endif