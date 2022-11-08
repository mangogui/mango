// reference : https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)

#include "Win32Window.h"

Window::Window()
	: m_hInstance(GetModuleHandle(nullptr)) {
	style = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW;
	createWindow();

	ShowWindow(GetConsoleWindow(), SW_HIDE);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 1;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Window::set_window_title(std::string title) {
	std::wstring stemp = std::wstring(title.begin(), title.end());
	LPCWSTR sw = stemp.c_str();
	//SetWindowText(m_hWnd, sw);
}

const std::string Window::get_window_title() const {
	char wnd_title[256];
	GetWindowTextA(m_hWnd, wnd_title, sizeof(wnd_title));
	std::string title(wnd_title);
	return title;
}

int Window::get_x() const noexcept {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return rect.left;
}

int Window::get_y() const noexcept {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return rect.top;
}

void Window::createWindow() {
	const char* CLASS_NAME = "Window";
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = CLASS_NAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpfnWndProc = WindowProc;

	RegisterClass(&wndClass);

	m_hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"Window",
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

void Window::show() const {
	ShowWindow(m_hWnd, SW_SHOW);
    bool running = true;
    while (running) {
        if (!Window::ProcessMessages())
            running = false;
    }
}


Window::~Window() {
	const wchar_t* CLASS_NAME = L"Window";
	UnregisterClass(reinterpret_cast<LPCSTR>(CLASS_NAME), m_hInstance);
}

bool Window::ProcessMessages() {
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