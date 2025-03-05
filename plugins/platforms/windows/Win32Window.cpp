#include "Win32Window.h"
#include "WindowManager.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return WindowManager::WindowProc(hWnd, uMsg, wParam, lParam);
}

Win32Window::Win32Window(Win32Window* parent): PlatformWindow(), m_parent(parent), m_hInstance(GetModuleHandle(nullptr)) {

}

Win32Window::~Win32Window() {
    const wchar_t* CLASS_NAME = L"Widget";
    UnregisterClass(reinterpret_cast<LPCSTR>(CLASS_NAME), m_hInstance);
}

void Win32Window::create() {
    DWORD style;
    if (m_parent) {
        style = WS_VISIBLE | WS_CHILD;
    }
    else {
        style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW;
    }
    const char *CLASS_NAME = objectName().c_str();
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
        (m_parent==nullptr) ? nullptr : m_parent->getWinId(),
        nullptr,
        m_hInstance,
        this
    );

    if (!m_hWnd) {
        DWORD error = GetLastError();
        std::cerr << objectName() << "::Failed to create window. Error Code: " << error << std::endl;
        return;
    }

    graphics = std::make_unique<Direct2DGraphicsContext>(m_hWnd);
}

void Win32Window::show() {
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

void Win32Window::hide() {
    ShowWindow(m_hWnd, SW_HIDE);
}

void Win32Window::maximize() {
    ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void Win32Window::resize(int w, int h) {
    PlatformWindow::resize(w, h);
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x(), y(), width(), height(), SWP_NOZORDER | SWP_NOACTIVATE);
}

void Win32Window::move(int x, int y) {
    PlatformWindow::move(x, y);
    if (!m_hWnd) return;

    SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    UpdateWindow(m_hWnd);
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

void *Win32Window::getNativeObject() {
    return m_hWnd;
}

