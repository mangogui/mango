#include "WindowManager.h"
#include <ResizeEvent.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <AbstractWidget.h>

std::map<void*, Object*> WindowManager::windowMapper;

#ifdef _WIN32

#include <Win32Window.h>
LRESULT CALLBACK WindowManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* widget = reinterpret_cast<Widget*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (uMsg) {
        case WM_QUIT:
            PostQuitMessage(0);
            return 0;
        case WM_ERASEBKGND:
            return 1;
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
            if (widget->getGraphicsContext())
                static_cast<Direct2DGraphicsContext*>(widget->getGraphicsContext())->resizeContext(width, height);
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
            break;
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
        default:
            break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#endif