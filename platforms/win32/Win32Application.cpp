#include "Win32Application.h"
#include <Windows.h>


namespace GUI {

    Win32Application::Win32Application() {

    }

    Win32Application::~Win32Application() {

    }

    void Win32Application::run() {

    }

    void Win32Application::quit() {
        isRunning = false;
    }

}