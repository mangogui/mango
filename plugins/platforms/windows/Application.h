#pragma once

#include <memory>
#include <vector>
#include <Windows.h>


// Define the entry point to use `main()` instead of `WinMain`
int main(int argc, char* argv[]);


// Define WinMain only in one translation unit
#ifndef DEFINE_WINMAIN
#define DEFINE_WINMAIN

extern "C" int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

#endif

class Widget;

class Application {
public:
    static Application &instance();

    void addWidget(Widget *widget);

    // Prevent copying and assignment
    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void run();

    ~Application();
private:

    Application();

    std::vector<Widget *> widgets;

    static void processEvents();
};