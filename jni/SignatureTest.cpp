//
// Created by Enid on 2016/8/17.
//
#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "SignatureTest.h"
#include <android/log.h>
#define   LOG_TAG    "JNI_LOG" 
//#define   LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define   LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
jstring Java_com_enid_signaturedemo_LibsLoad_getKeyFromJni(JNIEnv *env, jobject clazz, jobject obj)
{	
	if(appSelfChecking(env, clazz, obj)){
		return appenString(env, getPackname(env, clazz, obj), getSignature(env, clazz, obj));
	}else{
		return NULL;
	}
}

/**
* jobject 传入 Context
*/
jstring getPackname(JNIEnv *env, jobject clazz, jobject obj)
{
	//通过传入一个java的引用获取该引用的类型
    jclass native_class = env->GetObjectClass(obj);
	//查找java中getPackageName()方法的ID，最后一个参数是方法返回类型，“()“表示传入参数没有，“Ljava/lang/String“表示返回类型
    jmethodID mId = env->GetMethodID(native_class, "getPackageName", "()Ljava/lang/String;");
	//调用ID 为 mID 的方法，在这里相当于 在java中调用 getPackageName();
    jstring packName = static_cast<jstring>(env->CallObjectMethod(obj, mId));
    return packName;
}

jstring getSignature(JNIEnv *env, jobject clazz, jobject obj)
{
    jclass native_class = env->GetObjectClass(obj);
    jmethodID pm_id = env->GetMethodID(native_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject pm_obj = env->CallObjectMethod(obj, pm_id);
    jclass pm_clazz = env->GetObjectClass(pm_obj);
	// 得到 getPackageInfo 方法的 ID
    jmethodID package_info_id = env->GetMethodID(pm_clazz, "getPackageInfo","(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jstring pkg_str = getPackname(env, clazz, obj);
	// 获得应用包的信息
    jobject pi_obj = env->CallObjectMethod(pm_obj, package_info_id, pkg_str, 64);
	// 获得 PackageInfo 类
    jclass pi_clazz = env->GetObjectClass(pi_obj);
	// 获得签名数组属性的 ID
    jfieldID signatures_fieldId = env->GetFieldID(pi_clazz, "signatures", "[Landroid/content/pm/Signature;");
    jobject signatures_obj = env->GetObjectField(pi_obj, signatures_fieldId);
    jobjectArray signaturesArray = (jobjectArray)signatures_obj;
    jsize size = env->GetArrayLength(signaturesArray);
    jobject signature_obj = env->GetObjectArrayElement(signaturesArray, 0);
    jclass signature_clazz = env->GetObjectClass(signature_obj);
    jmethodID string_id = env->GetMethodID(signature_clazz, "toCharsString", "()Ljava/lang/String;");
    jstring str = static_cast<jstring>(env->CallObjectMethod(signature_obj, string_id));
    char *c_msg = (char*)env->GetStringUTFChars(str,0);
    //LOGI("signsture: %s", c_msg);
    return str;
}

jstring appenString(JNIEnv* env, jstring s1, jstring s2)
{   const char *s1x = (env)->GetStringUTFChars(s1, NULL);
    const char *s2x = (env)->GetStringUTFChars(s2, NULL);
    char *sall = new char[strlen(s1x) + strlen(s2x) + 1];
    strcpy(sall, s1x);
    strcat(sall, s2x);
    jstring retval = (env)->NewStringUTF(sall);
    (env)->ReleaseStringUTFChars(s1, s1x);
    (env)->ReleaseStringUTFChars(s2, s2x);
    free(sall);
    return retval;
}

/*
*程序自检
*如果安装程序应用签名和签名样本签名相等返回true（1），否则返回false（0）。
*/
char appSelfChecking(JNIEnv *env, jobject clazz, jobject obj)
{
	jstring checkSignature = getSignature(env, clazz, obj);
	char isCheckSuccess = 0;
	char signatureSample[] = "308202c7308201afa003020102020404f3a955300d06092a864886f70d01010b050030133111300f060355040713086d69616e79616e673020170d3135303730373039323733335a180f32333135303432363039323733335a30133111300f060355040713086d69616e79616e6730820122300d06092a864886f70d01010105000382010f003082010a0282010100d5021041b4fd22c8db1f15e82cfe7fcb1f83fb5522c3043bd3e35f2dfe36c3f01333a2ac1a4df7f331d334a69a8a234900e3910daffa25b0434f4fef357ff03e32589650589d2de96f7d552dc8be7ab3b604fd771dfb1f9e9d12c9d0c19eb338c75b43a15c678d5931aae03d2282f33b687cfebc5dca4be42a28635109bef510673ac8fe654a527a2d66921f0f19ed1a37753a638fd276915e3e6bb168b6ea585b0da644bca8fd4a5b60b5627e864ff3ccecbf4060e2bfc3212f07c205b0561fed3d7bde1f661d1efb6d277ccd7a65bab2ddacc6fc368cebcff049f488d71895aa9316579d1bc6934a8eb6f0faa0349f8f11dda8f74de112a54c586806b995810203010001a321301f301d0603551d0e041604141205a72c351f4c6f9f0c20d7b1609931514d6df5300d06092a864886f70d01010b0500038201010036755ea0936fb88bc4a66f8201c6314d8546bb5a269b745b075f1b2739d3c471e29494a205e4e31673c91814be7e0200198bf447176e40f5925a61b696a2050856beade61a62d9d4a3ee5a7ff7b50b057b78784b1a8934a1d73d3fb3002b9da2134d33f05b97040af98accbd02227bfe5d3670148a303fc611e88b48592b41cba7b2ac2dc3ce2d520efe50de97a3dd2ccfdaa1302ce60dfa68fa67d8faa5a7d3fe42b12525aa796596a1a70a42b75f7abf31ee3d15f4f3daeb6092aa06b09cf7b47c4fb3205d9c6f2f192f6921851a9f2b66bcbebc99d583152469da6f2faba67a6d2f5f2f127f2f00454c6880825dcbb2f252275d27a3e9fa2b9959741f722e";
	const char *signature = (env)->GetStringUTFChars(checkSignature,NULL);
	if(0 == strcmp(signatureSample,signature)){
		isCheckSuccess = 1;
	}
	(env)->ReleaseStringUTFChars(checkSignature,signature);
	return isCheckSuccess;
}
