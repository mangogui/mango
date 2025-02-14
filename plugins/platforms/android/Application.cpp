#include "Application.h"
#include "jni.h"

__attribute__((weak)) int main();

extern "C" JNIEXPORT void JNICALL
Java_mangoproject_mango_android_MainActivity_cppMain(JNIEnv
*env,
jobject mainActivity
) {
main();

}

Application::Application() {
}

Application::~Application() {
}

void Application::run() {
}
