#include <Cocoa/Cocoa.h>
#include <Widget.h>

@interface ViewObjC : NSView
- (NSView *)initWithWidget:(Widget *)_widget;

- (void)setBackgroundColor:(NSColor *)color;
@end