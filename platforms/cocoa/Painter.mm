#include "Painter.h"
#import "Cocoa/Cocoa.h"

namespace GUI {
    void Painter::drawRectangle(const Rect &rect) {
        NSColor *nscolor = [NSColor colorWithRed:color.red()/255 green:color.green()/255 blue:color.blue()/255 alpha:color.alpha()/255];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSRectFill(rectangle);
    }

    void Painter::drawRoundedRectangle(const Rect &rect, int x_radius, int y_radius) {
        NSColor *nscolor = [NSColor colorWithRed:color.red()/255 green:color.green()/255 blue:color.blue()/255 alpha:color.alpha()/255];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSBezierPath *roundedRectPath = [NSBezierPath bezierPathWithRoundedRect:rectangle xRadius:x_radius yRadius:y_radius];
        [roundedRectPath fill];
    }

    void Painter::drawEllipse(const Rect& rect) {
        NSColor *nscolor = [NSColor colorWithRed:color.red()/255 green:color.green()/255 blue:color.blue()/255 alpha:color.alpha()/255];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSBezierPath *ellipsePath = [NSBezierPath bezierPath];
        [ellipsePath appendBezierPathWithOvalInRect:rectangle];
        [ellipsePath fill];
    }
}