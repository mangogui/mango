#include <Cocoa/Cocoa.h>
#include <AbstractWidget.h>

@interface CocoaWindowObjC : NSWindow
@property (nonatomic, assign) AbstractWidget *widget;
- (instancetype)initWithWidget:(AbstractWidget *)_widget;
@end