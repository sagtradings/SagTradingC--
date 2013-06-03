/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class nativeinterfaces_DefaultNativeInterface */

#ifndef _Included_nativeinterfaces_DefaultNativeInterface
#define _Included_nativeinterfaces_DefaultNativeInterface
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    subscribeListener
 * Signature: (Llisteners/DefaultCTPListener;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_subscribeListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    unSubscribeListener
 * Signature: (Llisteners/DefaultCTPListener;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_unSubscribeListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    sendLoginMessage
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendLoginMessage
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    sendOrderInsertMessage
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendOrderInsertMessage
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    sendReturnOrderMessage
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendReturnOrderMessage
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    sendQuoteRequest
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendQuoteRequest
  (JNIEnv *, jobject, jobjectArray);

/*
 * Class:     nativeinterfaces_DefaultNativeInterface
 * Method:    sendTradeRequest
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_nativeinterfaces_DefaultNativeInterface_sendTradeRequest
  (JNIEnv *, jobject, jobjectArray);

#ifdef __cplusplus
}
#endif
#endif
