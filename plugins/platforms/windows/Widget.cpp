#include "Widget.h"
#include "MNRect.h"
#include "MouseEvent.h"

#include "Color.h"
#include <Win32Window.h>
#include "WindowManager.h"


Widget::Widget(Widget *parent)
        : m_parent(parent), Object() {
    if (m_parent) parent->addChild(this);
    Win32Window* parentWindow = nullptr;
    if (parent)
        parentWindow = dynamic_cast<Win32Window*>(parent->getWindow());
    window = std::make_unique<Win32Window>(parentWindow);
}

std::string Widget::getWindowTitle() const {
    return m_windowTitle;
}

int Widget::x() const noexcept {
    return (int) window->x();
}

int Widget::y() const noexcept {
    return (int) window->y();
}

void Widget::display() {
    window->setObjectName(objectName());
    window->create();
    WindowManager::insertWidget(getWinId(), this);
    window->show();
    for (Widget* child : m_children) {
        child->display();
        child->update();
    }
}

void Widget::center() {
    // TODO
}

Widget::~Widget() {
}

void Widget::move(int x, int y) {
    window->move(x, y);
}

void Widget::resize(int w, int h) {
    window->resize(w, h);
    update();
}

void Widget::mousePressEvent(MouseEvent *event) {}
void Widget::mouseReleaseEvent(MouseEvent *event) {}
void Widget::resizeEvent(ResizeEvent *event) {}
void Widget::paintEvent(PaintEvent *event) {}


void Widget::fullscreen() {

}

void Widget::update() {
    window->update();
}

void Widget::maximize() {
    window->maximize();
}

void Widget::setBackgroundColor(const std::string &hexColor) {

}

int Widget::width() const {
    return window->geometry().width();
}

int Widget::height() const {
    return window->geometry().height();
}

MNSize Widget::size() const {
    return window->geometry().size();
}

MNRect Widget::rect() const {
    return window->geometry();
}

void Widget::addChild(Widget *child) {
    m_children.push_back(child);
    child->setParent(this);
}

void Widget::setParent(Widget* parent) {
    m_parent = parent;
}

void Widget::setWindowTitle(const std::string& title) {
    m_windowTitle = title;
    window->setTitle(title);
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
