#import "Cocoa/Cocoa.h"
#import "Widget.h"

@interface ViewObjC : NSView
- (NSView *)initWithWidget:(Widget *)_widget;

- (void)setBackgroundColor:(NSColor *)color;
@end