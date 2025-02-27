#include <Application.h>
#include <Widget.h>
#import <Cocoa/Cocoa.h>
#include <AnimationScheduler.h>

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSLog(@"Application did finish launching");

    // Set up window delegate for all windows
    for (NSWindow *window in [NSApp windows]) {
        [window setDelegate:self];
    }
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    NSLog(@"Close button clicked, terminating app...");
    return YES;
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"Window will close called");
    [NSApp terminate:nil];
}

- (BOOL)windowShouldClose:(NSWindow *)sender {
    NSLog(@"Window should close called");
    return YES;
}
@end

struct CocoaApplicationWrapper {
    NSApplication *wrapped;

    ~CocoaApplicationWrapper() {
        [wrapped release];
    }
};

Application::Application() : wrapper(std::make_unique<CocoaApplicationWrapper>()) {
    @autoreleasepool {
        [NSApplication sharedApplication];
        wrapper->wrapped = [NSApp retain];

        // Create and set delegate
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        [NSApp setDelegate:appDelegate];

        // Activate the application
        [NSApp activateIgnoringOtherApps:YES];
    }
}

Application::~Application() {
    @autoreleasepool {
        if (wrapper) {
            // TODO: Maybe release delegate here?
            [wrapper->wrapped release];
        }
    }
}

Application &Application::instance() {
    static Application instance;
    return instance;
}

void Application::processEvents() {
    @autoreleasepool {
        NSEvent *event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES])) {
            [NSApp sendEvent:event];
            [NSApp updateWindows];
        }
    }
}

void Application::run() {
    @autoreleasepool {
        NSRunLoop *runLoop = [NSRunLoop currentRunLoop];

        if (![NSApp isRunning]) {
            [NSApp finishLaunching];
        }

        while (true) {
            processEvents();
            AnimationScheduler::instance().updateAnimations();

            NSDate *nextFrameTime = [NSDate dateWithTimeIntervalSinceNow:1.0 / 60.0];
            [runLoop runMode:NSDefaultRunLoopMode beforeDate:nextFrameTime];
        }
    }
}

void Application::addWidget(Widget *widget) {
    widgets.push_back(widget);
}

