#import "CocoaWindow.h"
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

@interface View: NSView
- (View*)initWithWindow:(GUI::CocoaWindow*)_window;
@end

@implementation View {
    GUI::CocoaWindow* window;
}

- (View*)initWithWindow:(GUI::CocoaWindow*)_window {
    self = [super init];

    window = _window;

    return self;
}

- (void)mouseDown:(NSEvent *)event {
    if ([event type] == NSEventTypeLeftMouseDown) {
        window->mousePressEvent();
    }
}

- (BOOL)isFlipped {
    return YES;
}

- (void)setFrameSize:(NSSize)newSize {
    [super setFrameSize:newSize];
    
}

- (void)drawRect:(NSRect)rect
{
    window->paintEvent();
}
@end


@interface CocoaWindowObjC : NSWindow
@end

@implementation CocoaWindowObjC

- (BOOL)canBecomeKeyWindow { return YES; }

- (BOOL)canBecomeMainWindow { return YES; }

@end

namespace GUI {

    struct CocoaWindowWrapper {
        CocoaWindowObjC *wrapped;
    };

    struct CocoaViewWrapper {
        NSView *wrapped;
    };

    void* CocoaWindow::getWindow() {
        return window_wrapper->wrapped;
    }

    void* CocoaWindow::getView() {
        return view_wrapper->wrapped;
    }

    CocoaWindow::CocoaWindow(CocoaWindow *parent): window_wrapper(new CocoaWindowWrapper()) {
        @autoreleasepool {
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

            View *view = [[View alloc] initWithWindow:this];
            [window_wrapper->wrapped setContentView:view];
            [window_wrapper->wrapped makeFirstResponder:view];

            if (parent != nullptr) {
                [parent->window_wrapper->wrapped.contentView addSubview:view];
                view_wrapper->wrapped = view;
                embedded = true;
            }
            else {
                embedded = false;
            }

        }
    }

    CocoaWindow::~CocoaWindow() {
        @autoreleasepool {
            if (window_wrapper)
                [window_wrapper->wrapped release];
            delete window_wrapper;
        }
    }

    void CocoaWindow::center() {
        @autoreleasepool {
            [window_wrapper->wrapped center];
        }
    }

    void CocoaWindow::display() {
        @autoreleasepool {
            [window_wrapper->wrapped display];
            [window_wrapper->wrapped orderFrontRegardless];
        }
    }

    const char *CocoaWindow::windowTitle() const {
        @autoreleasepool {
            const char *title = [[window_wrapper->wrapped title] UTF8String];
            return title;
        }
    }

    void CocoaWindow::setWindowTitle(const char *title) {
        @autoreleasepool {
            [window_wrapper->wrapped setTitle:@(title)];
        }
    }

    void CocoaWindow::resize(int width, int height) {
        @autoreleasepool {
            NSSize size = CGSizeMake(width, height);
            [wrapper->wrapped setContentSize:size];
        }
    }

    void CocoaWindow::move(int x, int y) {
        if (embedded) {
            [view_wrapper->wrapped setFrameOrigin:NSMakePoint(x, y)];}
        else
            [window_wrapper->wrapped setFrameOrigin:NSMakePoint(x, y)];
    }

    Size CocoaWindow::size() const {
        @autoreleasepool {
            NSRect frame;
            if (embedded)
                frame = [view_wrapper->wrapped frame];
            else
                frame = [window_wrapper->wrapped frame];
            Size _size = Size(frame.size.width, frame.size.height);
            return _size;
        }
    }

    void CocoaWindow::maximize() {
        @autoreleasepool {
            [window_wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        }
    }

    void CocoaWindow::fullscreen() {
        @autoreleasepool {
            [window_wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
            [window_wrapper->wrapped toggleFullScreen:window_wrapper->wrapped];
        }
    }

    void CocoaWindow::paintEvent() {

    }

    Screen CocoaWindow::screen() {
        NSScreen *nsscreen = [wrapper->wrapped screen];
        NSRect screenFrame = [nsscreen frame];
        CGSize screenSize = NSMakeSize(screenFrame.size.width, screenFrame.size.height);
        Screen screen;
        screen.setSize(Size(screenSize.width, screenSize.height));
        return screen;
    }
}
