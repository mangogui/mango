#include "Widget.h"
#include "MNRect.h"
#include "MouseEvent.h"
#include "WindowManager.h"
#include "Color.h"
#include "D2D1Helpers.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return WindowManager::WindowProc(hWnd, uMsg, wParam, lParam);
}

Widget::Widget(Widget *parent)
        : m_hInstance(GetModuleHandle(nullptr)), m_parent(parent), Object() {
    if (m_parent) parent->addChild(this);
}

void Widget::resizeRenderTarget(int width, int height) {
    if (!m_pRenderTarget) return;  // Ensure the render target exists

    D2D1_SIZE_U newSize = D2D1::SizeU(width, height);
    m_pRenderTarget->Resize(newSize); // Resize render target to match widget

    if (m_pBackBuffer) {
        m_pBackBuffer->Release(); // Release old back buffer
        m_pBackBuffer = nullptr;

        HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(&m_pBackBuffer);
        if (FAILED(hr)) {
            MessageBoxW(nullptr, L"Failed to create resized back buffer", L"Error", MB_OK);
        }
    }
}

std::string Widget::getWindowTitle() const {
    char wnd_title[256] = {0}; // Ensure buffer is zero-initialized
    GetWindowTextA(m_hWnd, wnd_title, sizeof(wnd_title));
    return std::string(wnd_title); // Return a copy (safe)
}

int Widget::x() const noexcept {
    return (int) geometry.x();
}

int Widget::y() const noexcept {
    return (int) geometry.y();
}

void Widget::createWindow() {
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

    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create Direct2D factory", L"Error", MB_OK);
        return;
    }

    // Create render target
    if (!m_pD2DFactory) return;

    RECT rc;
    GetClientRect(m_hWnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWnd, size),
            &m_pRenderTarget
    );

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create Direct2D render target", L"Error", MB_OK);
        return;
    }

    hr = m_pRenderTarget->CreateCompatibleRenderTarget(&m_pBackBuffer);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create back buffer", L"Error", MB_OK);
        return;
    }
    // Finished

    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create DirectWrite factory", L"Error", MB_OK);
        return;
    }

    hr = m_pDWriteFactory->CreateTextFormat(
            L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            36.0f, L"en-us", &m_pTextFormat
    );

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create text format", L"Error", MB_OK);
    }

    hr = m_pRenderTarget->CreateSolidColorBrush(convertColorToD2DColorF(Color::White), &m_pBrush);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create solid color brush", L"Error", MB_OK);
    }
}

void Widget::display() {
    createWindow();
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
    for (Widget* child : m_children) {
        child->display();
        child->update();
    }
}

void Widget::center() {
    // TODO
}

Widget::~Widget() {
    const wchar_t* CLASS_NAME = L"Widget";
    UnregisterClass(reinterpret_cast<LPCSTR>(CLASS_NAME), m_hInstance);
    if (m_pBrush) {
        m_pBrush->Release();
        m_pBrush = nullptr;
    }

    if (m_pTextFormat) {
        m_pTextFormat->Release();
        m_pTextFormat = nullptr;
    }

    if (m_pDWriteFactory) {
        m_pDWriteFactory->Release();
        m_pDWriteFactory = nullptr;
    }

    if (m_pBackBuffer) {
        m_pBackBuffer->Release();
        m_pBackBuffer = nullptr;
    }

    if (m_pRenderTarget) {
        m_pRenderTarget->Release();
        m_pRenderTarget = nullptr;
    }

    if (m_pD2DFactory) {
        m_pD2DFactory->Release();
        m_pD2DFactory = nullptr;
    }
}

void Widget::move(int x, int y) {
    geometry.move(x, y);
    if (!m_hWnd) return;

    SetWindowPos(m_hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    UpdateWindow(m_hWnd);

    // Force repaint of all child windows
    EnumChildWindows(m_hWnd, [](HWND hWnd, LPARAM lParam) -> BOOL {
        InvalidateRect(hWnd, nullptr, TRUE);
        UpdateWindow(hWnd);
        return TRUE;
    }, 0);
}

void Widget::resize(int w, int h) {
    // Resize the window to the specified width and height
    geometry.set_width(w);
    geometry.set_height(h);
    if (!m_hWnd) return;
    SetWindowPos(m_hWnd, nullptr, x(), y(), width(), height(), SWP_NOZORDER | SWP_NOACTIVATE);
    update();
}

void Widget::mousePressEvent(MouseEvent *event) {}
void Widget::mouseReleaseEvent(MouseEvent *event) {}
void Widget::resizeEvent(ResizeEvent *event) {}
void Widget::paintEvent(PaintEvent *event) {}


void Widget::fullscreen() {

}

void Widget::update() {
    if (!m_hWnd) return;
    InvalidateRect(m_hWnd, nullptr, FALSE); // Mark the entire window for repaint
}

void Widget::maximize() {
    // Maximize the window
    ShowWindow(m_hWnd, SW_MAXIMIZE);
}

void Widget::setBackgroundColor(const std::string &hexColor) {

}

int Widget::width() const {
    return geometry.width();
}

int Widget::height() const {
    return geometry.height();
}

MNSize Widget::size() const {
    // Todo, add getSize to MNRect
    return MNSize(geometry.width(), geometry.height());
}

MNRect Widget::rect() const {
    return geometry;
}

void Widget::addChild(Widget *child) {
    m_children.push_back(child);
    child->setParent(this);
}

void Widget::setParent(Widget* parent) {
    m_parent = parent;
}

void Widget::setWindowTitle(const std::string& title) {
    if (!m_hWnd) return;
    SetWindowTextA(m_hWnd, title.c_str());
}

void Widget::handleEvent(Event *event) {
    switch (event->type()) {
        case Event::Type::MouseEvent: {
            auto* _mouseEvent = dynamic_cast<MouseEvent*>(event);
            this->mousePressEvent(_mouseEvent);
            break;
        }
        case Event::Type::PaintEvent: {
            auto* _paintEvent = dynamic_cast<PaintEvent*>(event);
            this->paintEvent(_paintEvent);
            break;
        }
        case Event::Type::ResizeEvent: {
            auto* _resizeEvent = dynamic_cast<ResizeEvent*>(event);
            this->resizeEvent(_resizeEvent);
            break;
        }
    }
}
