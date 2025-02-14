#include <jni.h>
#include "AndroidActivity.h"
#include "JniEnvironment.h"
#include <android/log.h>

#define LOG_TAG "my-lib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


extern "C" JNIEXPORT void JNICALL
Java_mangoproject_mango_android_MainActivity_nativeMain(JNIEnv
*env,
jobject mainActivity
) {
JniEnvironment::getInstance()

.
initialize(env);

AndroidActivity::getInstance()

.
initialize(mainActivity);
}


//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
//    JNIEnv* env = nullptr;
//
//    // Get the JNIEnv from JavaVM
//    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
//        return JNI_ERR; // JNI version not supported or other error
//    }
//
//    JniEnvironment::getInstance().initialize(env);
//
//    return JNI_VERSION_1_6;
//}