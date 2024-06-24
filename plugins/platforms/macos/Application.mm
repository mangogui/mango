#import "Application.h"
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


struct CocoaApplicationWrapper {
    CocoaApplicationObjC *wrapped;
};

Application::Application() : wrapper(new CocoaApplicationWrapper()) {
    @autoreleasepool {
        wrapper->wrapped = [NSApplication sharedApplication];
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        [wrapper->wrapped setDelegate:appDelegate];
    }
}

Application::~Application() {
    @autoreleasepool {
        if (wrapper)
            [wrapper->wrapped release];
        delete wrapper;
    }
}

void Application::run() {
    @autoreleasepool {
        [wrapper->wrapped run];
    }
}
