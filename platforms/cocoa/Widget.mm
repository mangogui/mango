#import "Widget.h"
#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include "Painter.h"
#include "Renderer.h"


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


@interface MTKViewObjC : MTKView
- (MTKView*)initWithWidget:(GUI::Widget*)_widget;
@end

@implementation MTKViewObjC {
    GUI::Widget* widget;
}

- (MTKView*)initWithWidget:(GUI::Widget*)_widget {
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

@end


namespace GUI {

    struct CocoaWindowWrapper {
        CocoaWindowObjC *wrapped;
    };

    struct MTKViewWrapper {
        MTKViewObjC *wrapped;
    };

    Widget::Widget(Widget *_parent) {
        @autoreleasepool {
            parent = _parent;
            window_wrapper = new CocoaWindowWrapper();
            view_wrapper = new MTKViewWrapper();
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
            init_mtk_view();

            if (parent) {
                [[parent->window_wrapper->wrapped contentView] addSubview:view_wrapper->wrapped];
            }
            else {
                [window_wrapper->wrapped setContentView:view_wrapper->wrapped];
                [window_wrapper->wrapped makeFirstResponder:view_wrapper->wrapped];
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

    void Widget::center() {
        @autoreleasepool {
            [window_wrapper->wrapped center];
        }
    }

    void Widget::display() {
        @autoreleasepool {
            [window_wrapper->wrapped display];
            [window_wrapper->wrapped orderFrontRegardless];
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

    Size Widget::size() const {
        @autoreleasepool {
            NSRect frame = [window_wrapper->wrapped frame];
            Size _size = Size(frame.size.width, frame.size.height);
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

    void Widget::init_mtk_view() {
        @autoreleasepool {
            MTKViewObjC *mtkView = [[MTKViewObjC alloc] initWithWidget:this];
            mtkView.enableSetNeedsDisplay = YES;
            mtkView.device = MTLCreateSystemDefaultDevice();
            mtkView.clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
            [mtkView.window setOpaque:NO];
            mtkView.layer.opaque = NO;
            Renderer *renderer = [[Renderer alloc] initWithMetalKitView:mtkView];
            renderer.window = this;
            [mtkView setDelegate:renderer];
            view_wrapper->wrapped = mtkView;
        }
    }

    void Widget::paintEvent(const PaintEvent& event) {

    }

    void Widget::move(int x, int y) {
        // , size().get_width(), size().get_height()
        if (parent) {
            MTKView *view = view_wrapper->wrapped;
            [view setFrameOrigin:NSMakePoint(x, y)];
            [view needsDisplay];
        }
        else {
            NSWindow *window = window_wrapper->wrapped;
            [window setFrameTopLeftPoint:NSMakePoint(x, y)];
            [window viewsNeedDisplay];
        }
    }

    Screen Widget::screen() {
        NSScreen *nsscreen = [window_wrapper->wrapped screen];
        NSRect screenFrame = [nsscreen frame];
        CGSize screenSize = NSMakeSize(screenFrame.size.width, screenFrame.size.height);
        Screen screen;
        screen.setSize(Size(screenSize.width, screenSize.height));
        return screen;
    }

    void Widget::mousePressEvent() {

    }

    void Widget::resizeEvent() {

    }

    void Widget::update() {
        [view_wrapper->wrapped setNeedsDisplay:YES];
    }
}