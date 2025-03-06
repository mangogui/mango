#include "Widget.h"
#include "MNRect.h"
#include "MouseEvent.h"
#include "Color.h"
#include "WindowManager.h"

#ifdef _WIN64
    #include <Direct2DGraphicsContext.h>
    #include <Windows.h>
    #pragma comment(lib, "d2d1.lib")
    #pragma comment(lib, "dwrite.lib")
    #include <d2d1.h>
    #include <dwrite.h>
    #include <Win32Window.h>
    typedef Window = Win32Window;
#elif __APPLE__
    #include <CoreGraphicsContext.h>
    #include <CocoaWindow.h>
    #include <CocoaView.h>
#endif


Widget::Widget(Widget *parent)
        : m_isCreated(false), m_parent(parent),
        Object(), m_isTopLevel(parent==nullptr) {
    if (m_parent)
        m_parent->addChild(this);
}

Widget::~Widget() {
    delete m_window;
    delete m_view;
    for (Widget* child : m_children) {
        delete child;
    }
}

void Widget::addChild(Widget *child) {
    m_children.push_back(child);
    child->setParent(this);
}

void Widget::setParent(Widget* parent) {
    m_parent = parent;
}

void Widget::center() {
    // TODO
}

void Widget::move(int x, int y) {
    m_geometry.move(x, y);
    if (!isCreated()) return;
    m_window->move(x, y);
}

void Widget::resize(int w, int h) {
    m_geometry.resize(w, h);
    if (!isCreated()) return;
    if (isTopLevel() and m_window)
        m_window->resize(w, h);
    else if (!isTopLevel() and m_view)
        m_view->resize(w, h);
    else
        return;

    update();
}

void Widget::maximize() {
    m_window->maximize();
}

void Widget::fullscreen() {

}

void Widget::create() {
    if (isTopLevel())
#ifdef _WIN64
        m_window = std::make_unique<Win32Window>(parentWindow);
        m_view = std::make_unique<Win32View>(m_window->getNativeObject(), this);
#elif __APPLE__
        m_window = new CocoaWindow(this);
#endif
    else {
        m_view = new CocoaView(this);
    }

    if (isTopLevel()) {
        m_window->setObjectName(objectName());
        m_window->create();
    }
    else {
        m_view->create();
    }

    if (m_parent) {
        if (m_parent->isTopLevel()) {
            m_parent->window()->addSubView(m_view);
        }
        else {
            m_parent->view()->addSubView(m_view);
        }
    }

    m_isCreated = true;
}

void Widget::display() {
    create();
    WindowManager::insertWidget(getWinId(), this);
    for (Widget* child : m_children) {
        child->setWindow(this->window());
        if (child->window() == nullptr)
            std::cout << "Child window is null" << std::endl;
        child->display();
        child->update();
    }

    if (isTopLevel())
        m_window->show();
}

void Widget::update() {
    if (!isTopLevel())
        m_view->update();
}

void Widget::setWindowTitle(const std::string& title) {
    m_windowTitle = title;
    m_window->setTitle(title);
}

void Widget::setBackgroundColor(const std::string &hexColor) {

}

GraphicsContext* Widget::getGraphicsContext() {
    return (GraphicsContext*) m_window->getGraphicsContext();
}

int Widget::x() const {
    return (int) m_geometry.x();
}

int Widget::y() const {
    return (int) m_geometry.y();
}

int Widget::width() const {
    return (int) m_geometry.width();
}

int Widget::height() const {
    return (int) m_geometry.height();
}

const MNRect& Widget::geometry() {
    return m_geometry;
}

MNSize Widget::size() const {
    return m_geometry.size();
}

MNRect Widget::rect() const {
    return m_window->geometry();
}

std::string Widget::getWindowTitle() const {
    return m_windowTitle;
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

void Widget::mousePressEvent(MouseEvent *event) {}
void Widget::mouseReleaseEvent(MouseEvent *event) {}
void Widget::resizeEvent(ResizeEvent *event) {}
void Widget::paintEvent(PaintEvent *event) {}

void Widget::setNativeContext(void *context) {
    nativeContext = context;
    getGraphicsContext()->setNativeContext(context);
}
