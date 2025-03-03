#include "CocoaWindow.h"
#include "WindowDelegate.h"
#include "CocoaWindowObjC.h"
#include <Color.h>

CocoaWindow::CocoaWindow() {
    @autoreleasepool {
        NSRect rect = NSMakeRect(0, 0, 400, 400);
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
        window = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                             styleMask:styleMask
                                               backing:NSBackingStoreBuffered
                                                 defer:NO];
        [window setTitle:@"Window"];
        [window center];

        WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
        [window setDelegate:windowDelegate];

        [window setShowsResizeIndicator:YES];
        [window setAcceptsMouseMovedEvents:YES];
        [window setLevel:NSNormalWindowLevel];
        [window setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                      NSWindowCollectionBehaviorManaged];
    }
}

CocoaWindow::~CocoaWindow() {
    @autoreleasepool {
        [window release];
    }
}

void CocoaWindow::show() {
    [window makeKeyAndOrderFront:nil];
}

void CocoaWindow::hide() {
    [window orderOut:nil];
}

void CocoaWindow::resize(int width, int height) {
    [window setContentSize:NSMakeSize(width, height)];
}

void CocoaWindow::move(int x, int y) {
    NSScreen *screen = [NSScreen mainScreen];
    CGFloat screenHeight = [screen frame].size.height;
    CGFloat adjustedY = screenHeight - y;
    [window setFrameTopLeftPoint:NSMakePoint(x, adjustedY)];
    [window display];
}

void CocoaWindow::setTitle(const std::string &title) {
    [window setTitle:[NSString stringWithUTF8String:title.c_str()]];
}

void CocoaWindow::setBackgroundColor(const std::string &hexColor) {
    Color color(hexColor);

    CGFloat redFloat = color.red() / 255.0;
    CGFloat greenFloat = color.green() / 255.0;
    CGFloat blueFloat = color.blue() / 255.0;
    CGFloat alphaFloat = color.alpha() / 255.0;

    NSColor *nscolor = [NSColor colorWithCalibratedRed:redFloat green:greenFloat blue:blueFloat alpha:alphaFloat];

    [[window contentView] setBackgroundColor:nscolor];
}

void* CocoaWindow::getNativeWindow() {
    return window;
}
