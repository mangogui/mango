#import "Cocoa/Cocoa.h"

#include <Widget.h>
#include <Color.h>
#include <Application.h>
#include <MNRect.h>
#include <MNSize.h>
#include <CocoaWindow.h>
#include <CocoaView.h>

Widget::Widget(Widget *_parent): parent(_parent), m_geometry(MNRect(0, 0, 100, 100)) {
    @autoreleasepool {
        Application::instance().addWidget(this);

        CGContextRef _context = [[NSGraphicsContext currentContext] CGContext];
        graphics = std::make_unique<CoreGraphicsContext>(_context);

        if (parent == nullptr) {
            window = std::make_unique<CocoaWindow>();
            view = std::make_unique<CocoaView>(static_cast<NSWindow*>(window->getNativeWindow()), this);

            auto parentWindow = static_cast<NSWindow*>(window->getNativeWindow());
            auto parentView = static_cast<NSView*>(view->getNativeView());
            [parentWindow setContentView:parentView];
            [parentWindow makeFirstResponder:parentView];
            [parentWindow center];
        } else {
            view = std::make_unique<CocoaView>(static_cast<NSView*>(parent->getView()->getNativeView()), this);
        }
    }
}

Widget::~Widget() {
}

std::string Widget::windowTitle() const {
    return m_windowTitle;
}

void Widget::setWindowTitle(const std::string &title) {
    m_windowTitle = title;
    window->setTitle(title);
}

void Widget::resize(int width, int height) {
    m_geometry.resize(width, height);
    if (!parent) {
        window->resize(width, height);
    } else {
        view->resize(width, height);
    }
}

MNSize Widget::size() const {
    return m_geometry.size();
}

void Widget::maximize() {
//    @autoreleasepool {
//        [static_cast<id>(window_wrapper) setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
//    }
}

void Widget::fullscreen() {
//    @autoreleasepool {
//        [static_cast<id>(window_wrapper) setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
//        [static_cast<id>(window_wrapper) toggleFullScreen:static_cast<id>(window_wrapper)];
//    }
}

void Widget::paintEvent(PaintEvent *event) {
}

void Widget::setBackgroundColor(const std::string &hexColor) {
    window->setBackgroundColor(hexColor);
}


void Widget::move(int x, int y) {
    if (parent) {
        view->move(x, y);
    } else {
        window->move(x, y);
    }
}

void Widget::mousePressEvent(MouseEvent *event) {
    if (event->type() == MouseEvent::Type::LeftButtonDown) {
    }
}

void Widget::mouseReleaseEvent(MouseEvent *event) {
    if (event->type() == MouseEvent::Type::LeftButtonUp) {
    }
}

void Widget::resizeEvent(ResizeEvent *event) {
    std::cout << "ResizeEvent" << std::endl;
}

void Widget::update() {
    view->update();
}

float Widget::scaleFactor() const {
//    NSWindow *window = static_cast<id>(window_wrapper);
//    NSScreen *windowScreen = [window screen];
//    CGFloat screenScale = [windowScreen backingScaleFactor];
//    return screenScale;
}

MNRect Widget::rect() const {
    return m_geometry;
}

int Widget::width() const {
    return m_geometry.width();
}

int Widget::height() const {
    return m_geometry.height();
}

int Widget::x() const {
    return m_geometry.x();
}

int Widget::y() const {
    return m_geometry.y();
}

void Widget::setObjectName(const std::string &obj_name) {
    this->object_name = obj_name;
}

void Widget::display() {
    window->show();
}

