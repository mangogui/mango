#import "CocoaWindow.h"
#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include <simd/simd.h>
#include "ShaderTypes.h"
#include "../../Rect.h"
#include <iostream>
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

namespace GUI {

    struct CocoaWindowWrapper {
        CocoaWindowObjC *wrapped;
    };

    CocoaWindow::CocoaWindow() : wrapper(new CocoaWindowWrapper()) {
        @autoreleasepool {
            NSRect rect = NSMakeRect(0, 0, 400, 400);
            NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                    | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
            wrapper->wrapped = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                                                  styleMask:styleMask
                                                                    backing:NSBackingStoreBuffered
                                                                      defer:NO];
            WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
            [wrapper->wrapped setDelegate:windowDelegate];
            [wrapper->wrapped setShowsResizeIndicator:YES];
            [wrapper->wrapped setAcceptsMouseMovedEvents:YES];
            [wrapper->wrapped setLevel:NSNormalWindowLevel];
            [wrapper->wrapped setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                                    NSWindowCollectionBehaviorManaged];
            [wrapper->wrapped setTitle:@"MainWindow"];

            initMTKView();
        }
    }

    CocoaWindow::~CocoaWindow() {
        @autoreleasepool {
            if (wrapper)
                [wrapper->wrapped release];
            delete wrapper;
        }
    }

    void CocoaWindow::center() {
        @autoreleasepool {
            [wrapper->wrapped center];
        }
    }

    void CocoaWindow::display() {
        @autoreleasepool {
            [wrapper->wrapped display];
            [wrapper->wrapped orderFrontRegardless];
        }
    }

    const char *CocoaWindow::windowTitle() const {
        @autoreleasepool {
            const char *title = [[wrapper->wrapped title] UTF8String];
            return title;
        }
    }

    void CocoaWindow::setWindowTitle(const char *title) {
        @autoreleasepool {
            [wrapper->wrapped setTitle:@(title)];
        }
    }

    void CocoaWindow::resize(int width, int height) {
        @autoreleasepool {
            NSRect frame = [wrapper->wrapped frame];
            frame.size.width = width;
            frame.size.height = height;
            [wrapper->wrapped setFrame:frame display:YES];
        }
    }

    Size CocoaWindow::size() const {
        @autoreleasepool {
            NSRect frame = [wrapper->wrapped frame];
            Size _size = Size(frame.size.width, frame.size.height);
            return _size;
        }
    }

    void CocoaWindow::maximize() {
        @autoreleasepool {
            [wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        }
    }

    void CocoaWindow::fullscreen() {
        @autoreleasepool {
            [wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
            [wrapper->wrapped toggleFullScreen:wrapper->wrapped];
        }
    }

    void CocoaWindow::initMTKView() {
        @autoreleasepool {
            MTKView *mtkView = [[MTKView alloc] init];
            mtkView.enableSetNeedsDisplay = YES;
            mtkView.device = MTLCreateSystemDefaultDevice();
            mtkView.clearColor = MTLClearColorMake(0.2, 0.2, 0.2, 1.0);
            Renderer *renderer = [[Renderer alloc] initWithMetalKitView:mtkView];
            renderer.window = this;
            [mtkView setDelegate:renderer];
            [wrapper->wrapped setContentView:mtkView];
            [wrapper->wrapped makeFirstResponder:mtkView];
        }
    }

    void CocoaWindow::paintEvent() {
        // std::cout << "paintEvent" << std::endl;
    }
}
