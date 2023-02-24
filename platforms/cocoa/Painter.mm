#include "Painter.h"
#import "Cocoa/Cocoa.h"

namespace GUI {
    void Painter::drawRectangle(const Rect &rect) {
        float r = float(color.red())/255;
        float g = float(color.green())/255;
        float b = float(color.blue())/255;
        float a = float(color.alpha())/255;
        NSColor *nscolor = [NSColor colorWithRed:r green:g blue:b alpha:a];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSRectFill(rectangle);
    }

    void Painter::drawRoundedRectangle(const Rect &rect, int x_radius, int y_radius) {
        float r = float(color.red())/255;
        float g = float(color.green())/255;
        float b = float(color.blue())/255;
        float a = float(color.alpha())/255;
        NSColor *nscolor = [NSColor colorWithRed:r green:g blue:b alpha:a];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSBezierPath *roundedRectPath = [NSBezierPath bezierPathWithRoundedRect:rectangle xRadius:x_radius yRadius:y_radius];
        [roundedRectPath fill];
    }

    void Painter::drawEllipse(const Rect& rect) {
        float r = float(color.red())/255;
        float g = float(color.green())/255;
        float b = float(color.blue())/255;
        float a = float(color.alpha())/255;
        NSColor *nscolor = [NSColor colorWithRed:r green:g blue:b alpha:a];
        [nscolor setFill];
        NSRect rectangle = NSMakeRect(rect.x(), rect.y(), rect.width(), rect.height());
        NSBezierPath *ellipsePath = [NSBezierPath bezierPath];
        [ellipsePath appendBezierPathWithOvalInRect:rectangle];
        [ellipsePath fill];
    }
}