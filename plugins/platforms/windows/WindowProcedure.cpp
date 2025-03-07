#include "WindowProcedure.h"
#include <WindowManager.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return WindowManager::WindowProc(hWnd, uMsg, wParam, lParam);
}
