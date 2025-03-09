#include <Cocoa/Cocoa.h>
#include <AbstractWidget.h>

@interface ViewObjC : NSView
- (NSView *)initWithWidget:(AbstractWidget *)_widget;
- (void)setBackgroundColor:(NSColor *)color;
@end