#include "CocoaWindow.h"
#include "WindowDelegate.h"
#include "CocoaWindowObjC.h"
#include <Color.h>
#include "ViewObjC.h"
#include <Cocoa/Cocoa.h>
#include <CoreGraphicsContext.h>

CocoaWindow::CocoaWindow(Widget *widget): PlatformWindow(widget) {
    CGContextRef _context = [[NSGraphicsContext currentContext] CGContext];
    m_graphicsContext = new CoreGraphicsContext(_context);
}

CocoaWindow::~CocoaWindow() {
    @autoreleasepool {
        [(id) m_nativeObject release];
    }
}

void CocoaWindow::show() {
    [(NSWindow *) m_nativeObject makeKeyAndOrderFront:nil];
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

void CocoaWindow::setBackgroundColor(const std::string &hexColor) {
    Color color(hexColor);

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
        NSWindow* window = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                                              styleMask:styleMask
                                                                backing:NSBackingStoreBuffered
                                                                  defer:NO];

        m_nativeObject = window;
        [window setTitle:@"Window"];
        [window center];
        WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
        [window setDelegate:windowDelegate];

        [window setShowsResizeIndicator:YES];
        [window setAcceptsMouseMovedEvents:YES];
        [window setLevel:NSNormalWindowLevel];
        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                      NSWindowCollectionBehaviorManaged];

        NSView* view = [[ViewObjC alloc] initWithWidget:m_widget];


        [view setWantsLayer:YES];
        [[view layer] setBackgroundColor:[[NSColor whiteColor] CGColor]];
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
    [[(NSWindow*)m_nativeObject contentView] addSubview:(NSView*)subView->getNativeObject()];
}
