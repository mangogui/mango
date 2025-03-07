#include "Win32Window.h"
#include "WindowManager.h"
#include <Widget.h>
#include <WindowProcedure.h>


Win32Window::Win32Window(Widget* widget): PlatformWindow(widget), m_hInstance(GetModuleHandle(nullptr)) {

}

Win32Window::~Win32Window() {
    const char* CLASS_NAME = "Window";
    UnregisterClass(CLASS_NAME, m_hInstance);
}

void Win32Window::create() {
    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW;
    const char* CLASS_NAME = "Window";
    WNDCLASS wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wndClass.lpfnWndProc = WindowProc;

    if (!RegisterClass(&wndClass)) {
        std::cout << "Failed to register window class! Error: " << GetLastError() << std::endl;
        return;
    }

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Window",
        style,
        x(),
        y(),
        width(),
        height(),
        nullptr, // parent id
        nullptr,
        m_hInstance,
        m_widget
    );

    if (!m_hWnd) {
        DWORD error = GetLastError();
        std::cerr << objectName() << "::Failed to create window. Error Code: " << error << std::endl;
        return;
    }
}

void Win32Window::show() {
    if (!m_hWnd) return;
    ShowWindow(m_hWnd, SW_SHOW);
}

void Win32Window::hide() {
    if (!m_hWnd) return;
    ShowWindow(m_hWnd, SW_HIDE);
}

void Win32Window::maximize() {
    if (!m_hWnd) return;
    ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void Win32Window::resize(int w, int h) {
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x(), y(), w, h, SWP_NOZORDER | SWP_NOACTIVATE);
}

void Win32Window::move(int x, int y) {
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Win32Window::update() {
    if (!m_hWnd) return;
    InvalidateRect(m_hWnd, nullptr, FALSE);
}

void Win32Window::setTitle(const std::string &title) {
    if (!m_hWnd) return;
    SetWindowTextA(m_hWnd, title.c_str());
}

void Win32Window::setBackgroundColor(const std::string &hexColor) {

}

void* Win32Window::nativeObject() {
    return m_hWnd;
}

void Win32Window::addSubView(PlatformView* subView) {
    if (!m_hWnd || !subView) return;

    HWND childHwnd = static_cast<HWND>(subView->nativeObject());
    if (!childHwnd) return;

    // Set the parent of the subView to m_hWnd
    SetParent(childHwnd, m_hWnd);

    // Ensure the child window is properly positioned within the parent
    SetWindowPos(childHwnd, HWND_TOP , subView->x(), subView->y(), subView->width(), subView->height(), SWP_NOZORDER | SWP_SHOWWINDOW);

    // Ensure the child window has the correct style.
    SetWindowLongPtr(childHwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
    SetWindowPos(childHwnd, HWND_TOP , subView->x(), subView->y(), subView->width(), subView->height(), SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}
