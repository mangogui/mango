#include "CocoaView.h"
#include <Cocoa/Cocoa.h>
#include "ViewObjC.h"
#include <Color.h>


CocoaView::CocoaView(AbstractWidget *widget): PlatformView(widget) {
}

CocoaView::~CocoaView() {
    [(id) view release];
}


void CocoaView::create() {
    @autoreleasepool {
        NSRect frame = NSMakeRect(x(), y(), width(), height());
        view = [[ViewObjC alloc] initWithWidget:m_widget];
        [(NSView*)view setFrame:frame];
        [(NSView*)view setWantsLayer:YES];
        const Color& color = m_widget->backgroundColor();
        CGColorRef cgcolor = CGColorCreateGenericRGB(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
        [[(NSView*)view layer] setBackgroundColor:cgcolor];
    }
}

void CocoaView::update() {
    if (!view) return;
    [(id) view setNeedsDisplay:YES];
}

void CocoaView::setFrame(int x, int y, int width, int height) {
    if (!view) return;
    [(id) view setFrame:NSMakeRect(x, y, width, height)];
}

void *CocoaView::nativeObject() {
    return view;
}

void CocoaView::resize(int width, int height) {
    if (!view) return;
    NSSize size = CGSizeMake(width, height);
    [(id) view setFrameSize:size];
}

void CocoaView::move(int x, int y) {
    if (!view) return;
    [(id) view setFrameOrigin:NSMakePoint(x, y)];
}

void CocoaView::show() {
    if (!view) return;
    [(id) view setHidden:NO];
}

void CocoaView::addSubView(PlatformView *subView) {
    if (!view) return;

    [(NSView*)view addSubview:(NSView*)subView->nativeObject()];
}

void CocoaView::setBackgroundColor(const std::string &hexColor) {
    if (!view) return;
    auto color = m_widget->backgroundColor();
    CGColorRef cgcolor = CGColorCreateGenericRGB(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
    [[(NSView*)view layer] setBackgroundColor:cgcolor];
}

void CocoaView::setBackgroundColor(const Color &color) {
    if (!view) return;
    CGColorRef cgcolor = CGColorCreateGenericRGB(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
    [[(NSView*)view layer] setBackgroundColor:cgcolor];
}


