#pragma once
#include "Windows.h"
#include "Widget.h"

class WindowManager {
public:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
