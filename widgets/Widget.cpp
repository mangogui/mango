#include "Widget.h"
#include "MNRect.h"
#include "MouseEvent.h"
#include "Color.h"

#ifdef _WIN32
    #include <Direct2DGraphicsContext.h>
    #include <Windows.h>
    #pragma comment(lib, "d2d1.lib")
    #pragma comment(lib, "dwrite.lib")
    #include <d2d1.h>
    #include <dwrite.h>
    #include <Win32Window.h>
    #include <Win32View.h>
#elif __APPLE__
    #include <CoreGraphicsContext.h>
    #include <CocoaWindow.h>
    #include <CocoaView.h>
#endif


Widget::Widget(Widget *parent)
        : Object(), m_isCreated(false), m_parent(parent),
        m_isTopLevel(parent==nullptr), m_backgroundColor(Color::NoColor) {
    m_geometry.resize(200, 200);
    if (m_parent)
        m_parent->addChild(this);
}

Widget::~Widget() {
    delete m_window;
    delete m_view;
    delete m_layout;
    for (Widget* child : m_children)
        delete child;
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
    if (isTopLevel())
        m_window->move(x, y);
    else
        m_view->move(x, y);
}

void Widget::resize(int w, int h) {
    m_geometry.resize(w, h);
    if (!isCreated()) return;
    m_graphicsContext->resizeContext(w, h);
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
#ifdef _WIN32
        m_window = new Win32Window(this);
#elif __APPLE__
        m_window = new CocoaWindow(this);
#endif

    else {
#ifdef _WIN32
        m_view = new Win32View(this);
#elif __APPLE__
        m_view = new CocoaView(this);
#endif
    }

    if (isTopLevel()) {
        m_window->setObjectName(objectName());
        m_window->create();
    }
    else {
        m_view->create();
    }

#ifdef _WIN32
    m_graphicsContext = new Direct2DGraphicsContext(getWinId());
#elif __APPLE__
    m_graphicsContext = new CoreGraphicsContext(getWinId());
#endif

    if (m_parent) {
        if (m_parent->isTopLevel())
            m_parent->window()->addSubView(m_view);
        else
            m_parent->view()->addSubView(m_view);
    }

    m_isCreated = true;
}

void applyLayout(Widget* widget) {
    if (widget->layout()) {
        widget->layout()->applyLayout();
    }
    std::stack<Widget*> widgetStack;
    widgetStack.push(widget);
    widgetStack.pop();

    for (Widget* child : widget->children()) {
        if (child->layout()) {
            child->layout()->applyLayout();
        }
        widgetStack.push(child);
    }
    while (!widgetStack.empty()) {
        Widget* currentWidget = widgetStack.top();
        widgetStack.pop();

        for (Widget* child : currentWidget->children()) {
            if (child->layout()) {
                child->layout()->applyLayout();
            }
            widgetStack.push(child);
        }
    }
}

void Widget::display() {
    create();
    for (Widget* child : m_children) {
        child->setWindow(this->window());
        child->create();
    }

    applyLayout(this);

    if (isTopLevel())
        m_window->show();
}

void Widget::update() {
    if (isTopLevel())
        m_window->update();
    else
        m_view->update();
}

void Widget::setWindowTitle(const std::string& title) {
    m_windowTitle = title;
    if (!isCreated()) return;
    m_window->setTitle(title);
}

// TODO
void Widget::setBackgroundColor(const std::string &hexColor) {
    m_backgroundColor = Color(hexColor);
    if (!isCreated()) return;
    if (isTopLevel())
        m_window->setBackgroundColor(hexColor);
    else
        m_view->setBackgroundColor(hexColor);
}

// TODO
void Widget::setBackgroundColor(const Color &color) {
    m_backgroundColor = color;
    if (!isCreated()) return;
    if (isTopLevel())
        m_window->setBackgroundColor(color);
    else
        m_view->setBackgroundColor(color);
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

void Widget::mousePressEvent(MouseEvent *event) {

}

void Widget::mouseReleaseEvent(MouseEvent *event) {

}

void Widget::resizeEvent(ResizeEvent *event) {
    m_geometry.resize(event->width(), event->height());
    if (m_layout)
        m_layout->applyLayout();
}

void Widget::paintEvent(PaintEvent *event) {
}

void Widget::setNativeContext(void *context) {
    ((GraphicsContext*)m_graphicsContext)->setNativeContext(context);
}

