#include "Application.h"
#include <AnimationScheduler.h>
#include <iostream>


extern "C" int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Call your `main()` function here
    return main(__argc, __argv); // Pass appropriate arguments to main()
}

void Application::addWidget(Widget *widget) {
    widgets.push_back(widget);
}


#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


void CALLBACK TimerProc(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    AnimationScheduler::instance().updateAnimations();
}

void initializeTimer() {
    timeBeginPeriod(1);  // Set high timer resolution
    timeSetEvent(16, 1, TimerProc, 0, TIME_PERIODIC);  // Start periodic timer
}

// In your application loop
void Application::run() {
    initializeTimer();
    MSG msg;
    LARGE_INTEGER frequency, lastTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&lastTime);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_QUIT) break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Calculate deltaTime
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        double deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;
        lastTime = currentTime;
        AnimationScheduler::instance().updateAnimations();
    }
}



Application &Application::instance() {
    static Application instance;
    return instance;
}

void Application::processEvents() {

}

Application::Application() {

}

Application::~Application() {

}
