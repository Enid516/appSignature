//
// Created by Enid on 2016/8/17.
//

#include <jni.h>
#include <string.h>
#ifdef __cplusplus
extern "C"
{
jstring Java_com_enid_signaturedemo_LibsLoad_getKeyFromJni(JNIEnv *, jobject, jobject);
jstring getPackname(JNIEnv *, jobject, jobject);
jstring getSignature(JNIEnv *, jobject, jobject);
jstring appenString(JNIEnv *, jstring, jstring);
char appSelfChecking(JNIEnv *, jobject, jobject);
}
#endif