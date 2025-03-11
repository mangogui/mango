#include <Application.h>
#include <Widget.h>
#import <Cocoa/Cocoa.h>
#include <AnimationScheduler.h>

@interface ApplicationDisplayLinkHandler : NSObject

- (instancetype)initWithApplication:(void*)application;
- (void)handleDisplayLink:(CADisplayLink *)displayLink;

@end

@interface ApplicationDisplayLinkHandler ()
@property (nonatomic, assign) void* application;
@end

@implementation ApplicationDisplayLinkHandler

- (instancetype)initWithApplication:(void*)application {
    self = [super init];
    if (self) {
        _application = application;
    }
    return self;
}

- (void)handleDisplayLink:(CADisplayLink *)displayLink {
    Application *app = static_cast<Application*>(_application);
    app->updateAnimations();
}

@end

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSLog(@"Application did finish launching");
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
    Application::instance().stop();
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

        m_appDelegate = [[AppDelegate alloc] init];
        [NSApp setDelegate:(AppDelegate*)m_appDelegate];
        [(AppDelegate*)m_appDelegate retain];
        [NSApp activateIgnoringOtherApps:YES];
    }
}

Application::~Application() {
    @autoreleasepool {
        if (wrapper) {
            [wrapper->wrapped release];
        }
    }
}

Application &Application::instance() {
    static Application instance;
    return instance;
}

void Application::run() {
    @autoreleasepool {
        if (![NSApp isRunning]) {
            [NSApp finishLaunching];
        }
        setupDisplayLink();
        [NSApp run];
    }
}

void Application::addWidget(Widget *widget) {
    widgets.push_back(widget);
}

void Application::updateAnimations() {
    AnimationScheduler::instance().updateAnimations();
}

void Application::setupDisplayLink() {
    displayLinkHandler = [[ApplicationDisplayLinkHandler alloc] initWithApplication:this];
    displayLink = [CADisplayLink displayLinkWithTarget:displayLinkHandler selector:@selector(handleDisplayLink:)];
    [displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    [displayLink retain];
    [(ApplicationDisplayLinkHandler*)displayLinkHandler retain];
}
