#import "Widget.h"
#import "Cocoa/Cocoa.h"
#include "Color.h"


@interface WindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation WindowDelegate
- (void)windowDidResize:(NSNotification *)notification {

}
- (void)windowWillStartLiveResize:(NSNotification *)notification {

}
- (void)windowDidEndLiveResize:(NSNotification *)notification {

}
@end

@interface CocoaWindowObjC : NSWindow
@end

@implementation CocoaWindowObjC

- (BOOL)canBecomeKeyWindow { return YES; }

- (BOOL)canBecomeMainWindow { return YES; }

@end


@interface ViewObjC : NSView
- (NSView*)initWithWidget:(Widget*)_widget;
- (void)setBackgroundColor:(NSColor *)color;
@end

@implementation ViewObjC {
    Widget* widget;
}

- (NSView*)initWithWidget:(Widget*)_widget {
    self = [super init];

    widget = _widget;

    return self;
}

- (BOOL)isFlipped { return YES; }

- (void)mouseDown:(NSEvent *)event {
    if ([event type] == NSEventTypeLeftMouseDown) {
        widget->mousePressEvent();
    }
}

- (void)setBackgroundColor:(NSColor *)color {
    if ([self wantsLayer]) {
        [self.layer setBackgroundColor:[color CGColor]];
    } else {
        [self setWantsLayer:YES];
        [self.layer setBackgroundColor:[color CGColor]];
    }
}
@end


struct CocoaWindowWrapper {
    CocoaWindowObjC *wrapped;
};

struct ViewWrapper {
    ViewObjC *wrapped;
};

Widget::Widget(Widget *_parent) {
    @autoreleasepool {
        parent = _parent;
        window_wrapper = new CocoaWindowWrapper();
        view_wrapper = new ViewWrapper();
        NSRect rect = NSMakeRect(0, 0, 400, 400);
        NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                                      | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
        window_wrapper->wrapped = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                                                     styleMask:styleMask
                                                                       backing:NSBackingStoreBuffered
                                                                         defer:NO];
        WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
        [window_wrapper->wrapped setDelegate:windowDelegate];
        [window_wrapper->wrapped setShowsResizeIndicator:YES];
        [window_wrapper->wrapped setAcceptsMouseMovedEvents:YES];
        [window_wrapper->wrapped setLevel:NSNormalWindowLevel];
        [window_wrapper->wrapped setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                                       NSWindowCollectionBehaviorManaged];
        [window_wrapper->wrapped setTitle:@"MainWindow"];
        init_view();

        if (parent) {
            [[parent->window_wrapper->wrapped contentView] addSubview:view_wrapper->wrapped];
        }
        else {
            [window_wrapper->wrapped setContentView:view_wrapper->wrapped];
            [window_wrapper->wrapped makeFirstResponder:view_wrapper->wrapped];
            [window_wrapper->wrapped center];
            [window_wrapper->wrapped display];
            [window_wrapper->wrapped orderFrontRegardless];
        }
    }
}

Widget::~Widget() {
    @autoreleasepool {
        if (window_wrapper)
            [window_wrapper->wrapped release];
        delete window_wrapper;
    }
}

const char *Widget::windowTitle() const {
    @autoreleasepool {
        const char *title = [[window_wrapper->wrapped title] UTF8String];
        return title;
    }
}

void Widget::setWindowTitle(const char *title) {
    @autoreleasepool {
        [window_wrapper->wrapped setTitle:@(title)];
    }
}

void Widget::resize(int width, int height) {
    @autoreleasepool {
        NSSize size = CGSizeMake(width, height);
        if (!parent) {
            [window_wrapper->wrapped setContentSize:size];
        }
        else {
            [view_wrapper->wrapped setFrameSize:size];
        }
    }
}

MNSize Widget::size() const {
    @autoreleasepool {
        NSRect frame = [window_wrapper->wrapped frame];
        MNSize _size = MNSize(frame.size.width, frame.size.height);
        return _size;
    }
}

void Widget::maximize() {
    @autoreleasepool {
        [window_wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
    }
}

void Widget::fullscreen() {
    @autoreleasepool {
        [window_wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        [window_wrapper->wrapped toggleFullScreen:window_wrapper->wrapped];
    }
}

void Widget::init_view() {
    @autoreleasepool {
        ViewObjC *view = [[ViewObjC alloc] initWithWidget:this];
        [view setWantsLayer:YES];
        [view.layer setBackgroundColor:[[NSColor whiteColor] CGColor]];
        [view.window setOpaque:NO];
        view.layer.opaque = NO;
        view_wrapper->wrapped = view;
    }
}

void Widget::paintEvent(const PaintEvent& event) {

}

void Widget::setBackgroundColor(std::string hexColor) {
    Color color(hexColor);

    CGFloat redFloat = color.red() / 255.0;
    CGFloat greenFloat = color.green() / 255.0;
    CGFloat blueFloat = color.blue() / 255.0;

    // Create and return the NSColor object
    NSColor *nscolor = [NSColor colorWithCalibratedRed:redFloat green:greenFloat blue:blueFloat alpha:1.0];

    // Set the background color
    [view_wrapper->wrapped.layer setBackgroundColor:[nscolor CGColor]];
}


void Widget::move(int x, int y) {
    if (parent) {
        NSView *view = view_wrapper->wrapped;
        [view setFrameOrigin:NSMakePoint(x, y)];
        [view setNeedsDisplay:YES];
    } else {
        NSWindow *window = window_wrapper->wrapped;
        NSScreen *screen = [NSScreen mainScreen];
        CGFloat screenHeight = [screen frame].size.height;
        // Adjust y-coordinate for top-left origin
        CGFloat adjustedY = screenHeight - y;
        [window setFrameTopLeftPoint:NSMakePoint(x, adjustedY)];
        [window display];
    }
}

void Widget::mousePressEvent() {

}

void Widget::resizeEvent() {

}

void Widget::update() {
    [view_wrapper->wrapped setNeedsDisplay:YES];
}

float Widget::scaleFactor() {
    NSWindow *window = window_wrapper->wrapped;
    NSScreen *windowScreen = [window screen];
    CGFloat screenScale = [windowScreen backingScaleFactor];
    return screenScale;
}
