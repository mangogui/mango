#include "Application.h"
#include "jni.h"


extern "C" JNIEXPORT void JNICALL
Java_mangoproject_mango_android_MainActivity_cppMain(JNIEnv *env, jobject mainActivity) {
    main();
}

Application::Application() {
}

Application::~Application() {
}

void Application::run() {
}
