#include "Application.h"
#include "Widget.h"
#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES;
}

- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}
@end

@interface CocoaApplicationObjC : NSApplication
@end

@implementation CocoaApplicationObjC
@end

struct CocoaApplicationWrapper {
    CocoaApplicationObjC *wrapped;
    ~CocoaApplicationWrapper() {
        [wrapped release];
    }
};

Application::Application() : wrapper(std::make_unique<CocoaApplicationWrapper>()) {
    @autoreleasepool {
        wrapper->wrapped = [NSApplication sharedApplication];
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        [wrapper->wrapped setDelegate:appDelegate];
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

// TODO: Optimise this function
void Application::updateAnimations() {
    using Clock = std::chrono::steady_clock;

    auto now = Clock::now();

    // Initialize lastUpdate if uninitialized
    if (lastUpdate == Clock::time_point{}) {
        lastUpdate = now;
    }

    // Calculate time difference (delta time in milliseconds)
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();
    lastUpdate = now;

    // Remove expired or non-running animations and update active ones
    animations.erase(std::remove_if(animations.begin(), animations.end(),
        [deltaTime](std::weak_ptr<VariantAnimation>& weakAnim) {
            if (auto anim = weakAnim.lock()) {  // Lock weak_ptr to check validity
                if (!anim->isRunning()) {
                    return true;  // Remove if animation is not running
                }
                anim->setCurrentTime(anim->getCurrentTime() + deltaTime);
                return false;  // Keep active animations
            }
            return true;  // Remove expired weak_ptr
        }), animations.end());
}

void Application::run() {
    @autoreleasepool {
        NSRunLoop *runLoop = [NSRunLoop currentRunLoop];

        if (![NSApp isRunning]) {
            [NSApp finishLaunching];
        }

        while (true) {
            processEvents();
            updateAnimations();

            NSDate *nextFrameTime = [NSDate dateWithTimeIntervalSinceNow:1.0 / 60.0];
            [runLoop runMode:NSDefaultRunLoopMode beforeDate:nextFrameTime];
        }
    }
}

void Application::addWidget(Widget *widget) {
    widgets.push_back(widget);
}

void Application::addAnimation(const std::shared_ptr<VariantAnimation>& animation) {
    if (!animation) return;
    animations.push_back(animation);
}
