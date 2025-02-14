#import "Widget.h"
#import "Cocoa/Cocoa.h"
#include "Color.h"
#include "CocoaWindowObjC.h"
#include "WindowDelegate.h"
#include "ViewObjC.h"
#include "PainterPath.h"
#include "Application.h"
#include "MNRect.h"
#include "MNSize.h"


Widget::Widget(Widget *_parent) {
    @autoreleasepool {
        Application::instance().addWidget(this);
        parent = _parent;

        init_view();

        // It is _a child widget
        if (parent) {
            NSWindow *parent_window = static_cast<id>(parent->window_wrapper);
            [[parent_window contentView] addSubview:static_cast<id>(view_wrapper)];
        }
            // It is _a top-level widget
        else {
            NSRect rect = NSMakeRect(0, 0, 400, 400);
            NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                                          | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
            window_wrapper = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                                                styleMask:styleMask
                                                                  backing:NSBackingStoreBuffered
                                                                    defer:NO];
            WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
            CocoaWindowObjC *window = static_cast<id>(window_wrapper);
            [window setDelegate:windowDelegate];
            [window setShowsResizeIndicator:YES];
            [window setAcceptsMouseMovedEvents:YES];
            [window setLevel:NSNormalWindowLevel];
            [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                          NSWindowCollectionBehaviorManaged];
            [window setTitle:@"MainWindow"];

            NSView *view = static_cast<id>(view_wrapper);
            [window setContentView:view];
            [window makeFirstResponder:view];
            [window center];
            [window display];
            [window orderFrontRegardless];
        }
    }
}

Widget::~Widget() {
    @autoreleasepool {
        if (window_wrapper)
            [static_cast<id>(window_wrapper) release];
    }
}

std::string Widget::windowTitle() const {
    @autoreleasepool {
        return [[static_cast<id>(window_wrapper) title] UTF8String];
    }
}

void Widget::setWindowTitle(const std::string &title) {
    @autoreleasepool {
        [static_cast<id>(window_wrapper) setTitle:[NSString stringWithUTF8String:title.c_str()]];
    }
}

void Widget::resize(int width, int height) {
    @autoreleasepool {
        NSSize size = CGSizeMake(width, height);
        if (!parent) {
            [static_cast<id>(window_wrapper) setContentSize:size];
        } else {
            [static_cast<id>(view_wrapper) setFrameSize:size];
            [static_cast<id>(view_wrapper) setBounds:NSMakeRect(0, 0, size.width, size.height)];
        }
    }
}

MNSize Widget::size() const {
    @autoreleasepool {
        NSRect frame = [static_cast<id>(view_wrapper) frame];
        MNSize _size = MNSize(frame.size.width, frame.size.height);
        return _size;
    }
}

void Widget::maximize() {
    @autoreleasepool {
        [static_cast<id>(window_wrapper) setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
    }
}

void Widget::fullscreen() {
    @autoreleasepool {
        [static_cast<id>(window_wrapper) setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        [static_cast<id>(window_wrapper) toggleFullScreen:static_cast<id>(window_wrapper)];
    }
}

void Widget::init_view() {
    @autoreleasepool {
        view_wrapper = [[ViewObjC alloc] initWithWidget:this];
        [static_cast<id>(view_wrapper) setWantsLayer:YES];
        [[static_cast<id>(view_wrapper) window] setOpaque:NO];
        ViewObjC *view = static_cast<id>(view_wrapper);
        view.layer.opaque = NO;
        view_wrapper = view;
    }
}

void Widget::paintEvent(const PaintEvent &event) {
}

void Widget::setBackgroundColor(const std::string &hexColor) {
    Color color(hexColor);

    CGFloat redFloat = color.red() / 255.0;
    CGFloat greenFloat = color.green() / 255.0;
    CGFloat blueFloat = color.blue() / 255.0;

    // Create and return the NSColor object
    NSColor *nscolor = [NSColor colorWithCalibratedRed:redFloat green:greenFloat blue:blueFloat alpha:1.0];

    // Set the background color
    [[static_cast<id>(view_wrapper) layer] setBackgroundColor:[nscolor CGColor]];
}


void Widget::move(int x, int y) {
    if (parent) {
        NSView *view = static_cast<id>(view_wrapper);
        [view setFrameOrigin:NSMakePoint(x, y)];
        [view setNeedsDisplay:YES];
    } else {
        NSWindow *window = static_cast<id>(window_wrapper);
        NSScreen *screen = [NSScreen mainScreen];
        CGFloat screenHeight = [screen frame].size.height;
        // Adjust y-coordinate for top-left origin
        CGFloat adjustedY = screenHeight - y;
        [window setFrameTopLeftPoint:NSMakePoint(x, adjustedY)];
        [window display];
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
    [static_cast<id>(view_wrapper) setNeedsDisplay:YES];
}

float Widget::scaleFactor() const {
    NSWindow *window = static_cast<id>(window_wrapper);
    NSScreen *windowScreen = [window screen];
    CGFloat screenScale = [windowScreen backingScaleFactor];
    return screenScale;
}

MNRect Widget::rect() const {
    MNSize widgetSize = size();
    int x = 0, y = 0;
    if (view_wrapper) {
        @autoreleasepool {
            NSRect frame = [static_cast<id>(view_wrapper) frame];
            x = frame.origin.x;
            y = frame.origin.y;
        }
    }
    return MNRect(x, y, widgetSize.getWidth(), widgetSize.getHeight());
}

int Widget::width() const {
    NSRect frame = [static_cast<id>(view_wrapper) frame];
    int _w = frame.size.width;
    return _w;
}

int Widget::height() const {
    NSRect frame = [static_cast<id>(view_wrapper) frame];
    int _h = frame.size.height;
    return _h;
}

int Widget::x() const {
    NSRect frame = [static_cast<id>(view_wrapper) frame];
    int x = frame.origin.x;
    return x;
}

int Widget::y() const {
    NSRect frame = [static_cast<id>(view_wrapper) frame];
    int y = frame.origin.y;
    return y;
}

void Widget::setObjectName(const std::string &obj_name) {
    this->object_name = obj_name;
}

