#import "CocoaApplication.h"
#import "Cocoa/Cocoa.h"


@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSLog(@"applicationDidFinishLaunching");
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    NSLog(@"applicationWillTerminate");
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

namespace GUI {

    struct CocoaApplicationWrapper {
        CocoaApplicationObjC *wrapped;
    };

    CocoaApplication::CocoaApplication() : wrapper(new CocoaApplicationWrapper()) {
        @autoreleasepool {
            wrapper->wrapped = [NSApplication sharedApplication];
            AppDelegate *appDelegate = [[AppDelegate alloc] init];
            [wrapper->wrapped setDelegate:appDelegate];
        }
    }

    CocoaApplication::~CocoaApplication() {
        @autoreleasepool {
            if (wrapper)
                [wrapper->wrapped release];
            delete wrapper;
        }
    }

    void CocoaApplication::run() {
        @autoreleasepool {
            [wrapper->wrapped run];
        }
    }

    // ========== C-interface
    extern "C" CCocoaApplication* CocoaApplication_create() {
        return reinterpret_cast<CCocoaApplication*>(new CocoaApplication());
    }

    extern "C" void CocoaApplication_run(CCocoaApplication* self) {
        return reinterpret_cast<CocoaApplication*>(self)->run();
    }
}