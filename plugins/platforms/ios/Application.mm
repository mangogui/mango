#import "Application.h"
#import <UIKit/UIKit.h>

CocoaApplicationWrapper *Application::sharedWrapper = nullptr;

@interface CocoaApplicationObjC : UIApplication
@end

@implementation CocoaApplicationObjC
@end


struct CocoaApplicationWrapper {
    CocoaApplicationObjC *wrapped;
};

Application::Application() {
    @autoreleasepool {
        if (sharedWrapper == nullptr) {
            sharedWrapper = new CocoaApplicationWrapper();
        }
        sharedWrapper->wrapped = [UIApplication sharedApplication];
    }
}

Application::~Application() {
    @autoreleasepool {
        if (sharedWrapper)
            [sharedWrapper->wrapped release];
        delete sharedWrapper;
    }
}

void Application::run() {
    @autoreleasepool {
        int argc = 0;
        char **argv = nullptr;
        UIApplicationMain(argc, argv, nil, NSStringFromClass([CocoaApplicationObjC class]));
    }
}

Application &Application::instance() {
    static Application singletonInstance;
    return singletonInstance;
}
