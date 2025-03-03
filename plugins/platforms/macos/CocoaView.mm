#include "CocoaView.h"

#include "ViewObjC.h"


CocoaView::CocoaView(NSView *parentView, Widget *widget) {
    if (widget == nullptr) {
        std::cout << "Widget is null" << std::endl;
    }
    @autoreleasepool {
        NSRect frame = NSMakeRect(0, 0, 200, 200);
        view = [[ViewObjC alloc] initWithWidget:widget];
        [view setWantsLayer:YES];
        if (parentView) {
            // Case: Child widget -> Add as a subview to the parent
            [parentView addSubview:view];
        }
    }
}

CocoaView::CocoaView(NSWindow *parentWindow, Widget *widget) {
    NSRect frame = NSMakeRect(0, 0, 400, 400); // Default size for top-level widget
    view = [[ViewObjC alloc] initWithWidget:widget];
    [view setWantsLayer:YES];

    // Set the background color
    [[view layer] setBackgroundColor:NSColor.whiteColor.CGColor];
}

CocoaView::~CocoaView() {
    [view release];
}

void CocoaView::update() {
    [view setNeedsDisplay:YES];
}

void CocoaView::setFrame(int x, int y, int width, int height) {
    [view setFrame:NSMakeRect(x, y, width, height)];
}

void *CocoaView::getNativeView() {
    return view;
}

void CocoaView::resize(int width, int height) {
    NSSize size = CGSizeMake(width, height);
    [view setFrameSize:size];
}

void CocoaView::move(int x, int y) {
    [view setFrameOrigin:NSMakePoint(x, y)];
}
