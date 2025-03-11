#include "CocoaWindow.h"
#include "WindowDelegate.h"
#include "CocoaWindowObjC.h"
#include <Color.h>
#include "ViewObjC.h"
#include <Cocoa/Cocoa.h>
#include <CoreGraphicsContext.h>

CocoaWindow::CocoaWindow(AbstractWidget *widget): PlatformWindow(widget) {
}

CocoaWindow::~CocoaWindow() {
    @autoreleasepool {
        [(id) m_nativeObject release];
        [(id) m_windowDelegate release];
    }
}

void CocoaWindow::show() {
    [(NSWindow *) m_nativeObject makeKeyAndOrderFront:nil];
    [(NSWindow *) m_nativeObject makeMainWindow];
    [(NSWindow *) m_nativeObject makeKeyWindow];
}

void CocoaWindow::hide() {
    [(NSWindow *)m_nativeObject orderOut:nil];
}

void CocoaWindow::resize(int w, int h) {
    if (!m_nativeObject) return;
    [(NSWindow *)m_nativeObject setContentSize:NSMakeSize(w, h)];
}

void CocoaWindow::move(int x, int y) {
    if (!m_nativeObject) return;
    NSScreen *screen = [NSScreen mainScreen];
    CGFloat screenHeight = [screen frame].size.height;
    CGFloat adjustedY = screenHeight - y;
    [(NSWindow *)m_nativeObject setFrameTopLeftPoint:NSMakePoint(x, adjustedY)];
    [(NSWindow *)m_nativeObject display];
}

void CocoaWindow::setTitle(const std::string &title) {
    [(NSWindow *)m_nativeObject setTitle:[NSString stringWithUTF8String:title.c_str()]];
}

// TODO
void CocoaWindow::setBackgroundColor(const std::string &hexColor) {
    if (!m_nativeObject) return;
    Color color(hexColor);

    CGFloat redFloat = color.red() / 255.0;
    CGFloat greenFloat = color.green() / 255.0;
    CGFloat blueFloat = color.blue() / 255.0;
    CGFloat alphaFloat = color.alpha() / 255.0;

    NSColor *nscolor = [NSColor colorWithCalibratedRed:redFloat green:greenFloat blue:blueFloat alpha:alphaFloat];

    [[(NSWindow *)m_nativeObject contentView] setBackgroundColor:nscolor];
}

// TODO
void CocoaWindow::setBackgroundColor(const Color &color) {
    if (!m_nativeObject) return;

    CGFloat redFloat = color.red() / 255.0;
    CGFloat greenFloat = color.green() / 255.0;
    CGFloat blueFloat = color.blue() / 255.0;
    CGFloat alphaFloat = color.alpha() / 255.0;

    NSColor *nscolor = [NSColor colorWithCalibratedRed:redFloat green:greenFloat blue:blueFloat alpha:alphaFloat];

    [[(NSWindow *)m_nativeObject contentView] setBackgroundColor:nscolor];
}

void CocoaWindow::create() {
    @autoreleasepool {
        NSRect rect = NSMakeRect(x(), y(), width(), height());
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
        NSWindow *window = [[CocoaWindowObjC alloc] initWithWidget:m_widget];
        [window setFrame:rect display:YES];
        [window setStyleMask:styleMask];
        [window setBackingType:NSBackingStoreBuffered];
        [window setReleasedWhenClosed:NO];

        m_nativeObject = window;
        [window setTitle:@"Window"];
        [window center];
        m_windowDelegate = [[WindowDelegate alloc] init];
        [window setDelegate:(WindowDelegate*)m_windowDelegate];

        [window setAcceptsMouseMovedEvents:YES];
        [window setLevel:NSNormalWindowLevel];
        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                      NSWindowCollectionBehaviorManaged];
        [window setShowsResizeIndicator:YES];

        NSView* view = [[ViewObjC alloc] initWithWidget:m_widget];
        [view setWantsLayer:YES];
        const Color& color = m_widget->backgroundColor();
        CGColorRef cgcolor = CGColorCreateGenericRGB(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
        [[view layer] setBackgroundColor:cgcolor];
        [window setContentView:view];
        [window makeFirstResponder:view];
        [window center];
    }
}

void CocoaWindow::maximize() {

}

void CocoaWindow::update() {
    [(NSWindow *)m_nativeObject display];
}

void CocoaWindow::addSubView(PlatformView *subView) {
    [[(NSWindow*)m_nativeObject contentView] addSubview:(NSView*)subView->nativeObject()];
}

void *CocoaWindow::nativeObject() {
    return m_nativeObject;
}


