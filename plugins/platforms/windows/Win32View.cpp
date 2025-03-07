#include "Win32View.h"
#include <WindowProcedure.h>
#include <iostream>
#include <Widget.h>


Win32View::Win32View(Widget *widget): PlatformView(widget), m_hInstance(GetModuleHandle(nullptr)) {
}

Win32View::~Win32View() {
    const char* CLASS_NAME = "View";
    UnregisterClass(reinterpret_cast<LPCSTR>(CLASS_NAME), m_hInstance);
}

void Win32View::create() {
    DWORD style;

    const char *CLASS_NAME = "View";
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
        "Widget",
        style,
        x(),
        y(),
        width(),
        height(),
        nullptr,
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

void Win32View::update() {
    if (!m_hWnd) return;
    InvalidateRect(m_hWnd, nullptr, FALSE);
}

void Win32View::setFrame(int x, int y, int width, int height) {
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

void* Win32View::nativeObject() {
    return m_hWnd;
}

void Win32View::resize(int w, int h) {
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x(), y(), width(), height(), SWP_NOZORDER | SWP_NOACTIVATE);
}

void Win32View::move(int x, int y) {
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Win32View::show() {
    ShowWindow(m_hWnd, SW_SHOW);
}

void Win32View::addSubView(PlatformView *subView) {
}


