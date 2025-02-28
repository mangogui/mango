#include "Widget.h"
#include "MNRect.h"
#include "MouseEvent.h"
#include "WindowManager.h"
#include "Color.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return WindowManager::WindowProc(hWnd, uMsg, wParam, lParam);
}

Widget::Widget(Widget *parent)
        : m_hInstance(GetModuleHandle(nullptr)), m_parent(parent), Object() {
    if (m_parent) parent->addChild(this);
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

    graphics = std::make_unique<Direct2DGraphicsContext>(getWinId());
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
