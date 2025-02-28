#include "WindowManager.h"

LRESULT CALLBACK WindowManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    auto* widget = reinterpret_cast<Widget*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
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
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (widget->getGraphicsContext())
                widget->getGraphicsContext()->resizeContext(width, height);
            ResizeEvent event(width, height);
            widget->handleEvent(&event);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
        {
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