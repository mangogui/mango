#include "WindowManager.h"
#include <Win32Window.h>
#include <ResizeEvent.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <Widget.h>

std::map<HWND, Object*> WindowManager::windowMapper;

LRESULT CALLBACK WindowManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    Widget* widget = nullptr;
    if (auto objOpt = WindowManager::findWidget(hWnd)) {
        Object* obj = *objOpt;
        widget = static_cast<Widget*>(obj);
    }

    switch (uMsg) {
        case WM_QUIT:
            PostQuitMessage(0);
            return 0;
        case WM_NCCREATE: {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            return TRUE;
        }
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 1;
        case WM_SIZE: {
            if (!widget) break;
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (window->getGraphicsContext())
                static_cast<Direct2DGraphicsContext*>(window->getGraphicsContext())->resizeContext(width, height);
            ResizeEvent event(width, height);
            widget->handleEvent(&event);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
        {
            if (!widget) break;
            PaintEvent event;
            widget->paintEvent(&event);
            break;
        }
        case WM_SHOWWINDOW:
        {
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN: {
            if (!widget) break;
            MouseEvent::Type type;
            switch (uMsg) {
                case WM_LBUTTONDOWN: type = MouseEvent::Type::LeftButtonDown; break;
                case WM_RBUTTONDOWN: type = MouseEvent::Type::RightButtonDown; break;
            }
            MouseEvent event(type);
            widget->mousePressEvent(&event);
            break;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}