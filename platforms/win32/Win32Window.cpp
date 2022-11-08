#include "Win32Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 1;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

namespace GUI {

    Win32Window::Win32Window()
            : m_hInstance(GetModuleHandle(nullptr)) {
        style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW;
        createWindow();

        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

    void Win32Window::set_window_title(std::string title) {
        std::wstring stemp = std::wstring(title.begin(), title.end());
        LPCWSTR sw = stemp.c_str();
        //SetWindowText(m_hWnd, sw);
    }

    const std::string Win32Window::get_window_title() const {
        char wnd_title[256];
        GetWindowTextA(m_hWnd, wnd_title, sizeof(wnd_title));
        std::string title(wnd_title);
        return title;
    }

    int Win32Window::get_x() const noexcept {
        RECT rect;
        GetWindowRect(m_hWnd, &rect);
        return rect.left;
    }

    int Win32Window::get_y() const noexcept {
        RECT rect;
        GetWindowRect(m_hWnd, &rect);
        return rect.top;
    }

    void Win32Window::createWindow() {
        const char *CLASS_NAME = "Win32Window";
        WNDCLASS wndClass = {};
        wndClass.lpszClassName = CLASS_NAME;
        wndClass.hInstance = m_hInstance;
        wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        wndClass.lpfnWndProc = WindowProc;

        RegisterClass(&wndClass);

        m_hWnd = CreateWindowEx(
                0,
                CLASS_NAME,
                "Win32Window",
                style,
                250,
                250,
                400,
                400,
                NULL,
                NULL,
                m_hInstance,
                NULL
        );
    }

    void Win32Window::display() {
        ShowWindow(m_hWnd, SW_SHOW);
        bool running = true;
        while (running) {
            if (!Win32Window::ProcessMessages())
                running = false;
        }
    }

    void Win32Window::center() {
        // TODO
    }

    Win32Window::~Win32Window() {
        const wchar_t *CLASS_NAME = L"Win32Window";
        UnregisterClass(reinterpret_cast<LPCSTR>(CLASS_NAME), m_hInstance);
    }

    bool Win32Window::ProcessMessages() {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
            switch (msg.message) {
                case WM_QUIT:
                    PostQuitMessage(0);
                    return false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return true;
    }
}