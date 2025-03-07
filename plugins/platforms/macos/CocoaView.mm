#include "CocoaView.h"
#include <Cocoa/Cocoa.h>
#include "ViewObjC.h"


CocoaView::CocoaView(Widget *widget): PlatformView(widget) {
}

CocoaView::~CocoaView() {
    [(id) view release];
}

void CocoaView::create() {
    @autoreleasepool {
        NSRect frame = NSMakeRect(x(), y(), width(), height());
        view = [[ViewObjC alloc] initWithWidget:m_widget];
        [(NSView*) view setFrame:frame];
        [(NSView*)view setWantsLayer:YES];
    }
}

void CocoaView::update() {
    [(id) view setNeedsDisplay:YES];
}

void CocoaView::setFrame(int x, int y, int width, int height) {
    [(id) view setFrame:NSMakeRect(x, y, width, height)];
}

void *CocoaView::nativeObject() {
    return view;
}

void CocoaView::resize(int width, int height) {
    NSSize size = CGSizeMake(width, height);
    [(id) view setFrameSize:size];
}

void CocoaView::move(int x, int y) {
    [(id) view setFrameOrigin:NSMakePoint(x, y)];
}

void CocoaView::show() {
    [(id) view setHidden:NO];
}

void CocoaView::addSubView(PlatformView *subView) {
    [(NSView*)view addSubview:(NSView*)subView->nativeObject()];
}


