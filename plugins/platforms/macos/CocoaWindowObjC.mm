#import "CocoaWindowObjC.h"

@implementation CocoaWindowObjC
- (instancetype)initWithWidget:(AbstractWidget*)widgetInstance {
    self = [super init];
    if (self) {
        self.widget = widgetInstance;
    }
    return self;
}

- (BOOL)canBecomeKeyWindow { return YES; }

- (BOOL)canBecomeMainWindow { return YES; }

@end
