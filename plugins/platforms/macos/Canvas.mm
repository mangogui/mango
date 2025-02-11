#include "Canvas.h"
#include <Cocoa/Cocoa.h>

CGColor* createNSColor(const Color& color) {
    return [[NSColor colorWithCalibratedRed:color.red()/255.0
                                     green:color.green()/255.0
                                      blue:color.blue()/255.0
                                     alpha:color.alpha()/255.0] CGColor];
}

CGPathRef convertPainterPathToCGPath(const GUI::PainterPath& painterPath) {
    CGMutablePathRef cgPath = CGPathCreateMutable();

    for (int i = 0; i < painterPath.elementCount(); ++i) {
        const GUI::PainterPath::Element& element = painterPath.elementAt(i);

        switch (element.type) {
            case GUI::PainterPath::MoveTo:
                CGPathMoveToPoint(cgPath, nullptr, element.x, element.y);
                break;

            case GUI::PainterPath::LineTo:
                CGPathAddLineToPoint(cgPath, nullptr, element.x, element.y);
                break;

            case GUI::PainterPath::CurveTo: {
                // Ensure there are enough points left in the path to form _a valid curve
                if (i + 3 < painterPath.elementCount()) {
                    const GUI::Point c1 = painterPath.elementAt(i + 1).toPoint();
                    const GUI::Point c2 = painterPath.elementAt(i + 2).toPoint();
                    const GUI::Point endPoint = painterPath.elementAt(i + 3).toPoint();
                    CGPathAddCurveToPoint(cgPath, nullptr, c1.x(), c1.y(), c2.x(), c2.y(), endPoint.x(), endPoint.y());
                    i += 3;  // Skip over the next 3 elements that were part of the cubic curve
                }
                break;
            }

            default:
                // Handle other element types (if any)
                break;
        }
    }

    return cgPath;
}

Canvas::Canvas(Widget* widget) : _widget(widget), context(nullptr) {
    this->fillStyle = std::make_unique<FillStyle>(Color::Gray);
    NSGraphicsContext* nsContext = [NSGraphicsContext currentContext];
    if (nsContext) {
        context = (void*) nsContext.CGContext;
        setClippingRegion();
    }
}

void Canvas::setClippingRegion() {
    if (!context || !_widget) return;

    auto ctx = static_cast<CGContextRef>(context);

    CGContextSaveGState(ctx);  // Save the current state

    NSView* view = static_cast<id>(_widget->getNSViewId());
    if (!view) return;

    NSRect widgetBounds = [view bounds];  // Get widget's local bounds
    CGRect clippingRect = CGRectMake(widgetBounds.origin.x, widgetBounds.origin.y,
                                     widgetBounds.size.width, widgetBounds.size.height);

    // Apply clipping region once
    CGContextClipToRect(ctx, clippingRect);

    // Debugging: Log the clipping region
    CGRect effectiveClip = CGContextGetClipBoundingBox(ctx);
}

void Canvas::drawPath(const GUI::PainterPath& path) {
    if (!context) return;  // Ensure context is valid

    auto ctx = static_cast<CGContextRef>(context);

    // Convert the PainterPath to _a CGPathRef
    CGPathRef cgPath = convertPainterPathToCGPath(path);
    if (!cgPath) return;

    // Set the fill color (red)
    CGContextSetFillColorWithColor(ctx, [[NSColor redColor] CGColor]);

    // Add the path to the context and fill it
    CGContextAddPath(ctx, cgPath);
    CGContextFillPath(ctx);

    // Clean up the path memory
    CGPathRelease(cgPath);
}

void Canvas::begin() {
    if (!_widget) return;

    NSView* view = static_cast<id>(_widget->getNSViewId());
    if (!view) return;

    NSGraphicsContext* nsContext = [NSGraphicsContext currentContext];
    if (nsContext) {
        context = (void*) nsContext.CGContext;
    } else {
        context = nullptr;  // Ensure we don't use an invalid context
    }
}

void Canvas::end() {
}

Canvas::~Canvas() {
    if (!context) return;

    auto ctx = static_cast<CGContextRef>(context);
    CGContextRestoreGState(ctx);
};

void Canvas::drawEllipse(const MNRect &rect) {
    if (!context || !_widget) return;  // Ensure context and widget are valid

    auto ctx = static_cast<CGContextRef>(context);

    CGContextSaveGState(ctx);  // Save the current state

    // Get widget position and size
    NSView* view = static_cast<id>(_widget->getNSViewId());
    if (!view) return;

    // Set the fill color (green)
    CGContextSetFillColorWithColor(ctx, createNSColor(fillStyle->getColor()));

    // Create an ellipse path
    CGPathRef ellipsePath = CGPathCreateWithEllipseInRect(CGRectMake(rect.x(), rect.y(), rect.width(), rect.height()), nullptr);

    // Add the ellipse path to the context and fill it
    CGContextAddPath(ctx, ellipsePath);
    CGContextFillPath(ctx);

    // Clean up the path memory
    CGPathRelease(ellipsePath);

    CGContextRestoreGState(ctx);  // Restore the previous state
}

void Canvas::drawRect(const MNRect &rect) {
    if (!context) return;  // Ensure context is valid

    auto ctx = static_cast<CGContextRef>(context);

    // Get the current clipping region (typically the view bounds)
    CGContextSaveGState(ctx);  // Save the current state

    CGRect clipBounds = CGContextGetClipBoundingBox(ctx);
    CGContextClipToRect(ctx, clipBounds);  // Clip to the visible region

    // Set the fill color (blue)
    CGContextSetFillColorWithColor(ctx, createNSColor(fillStyle->getColor()));

    // Create _a rectangle path
    CGPathRef rectPath = CGPathCreateWithRect(CGRectMake(rect.x(), rect.y(), rect.width(), rect.height()), nullptr);

    // Add the rectangle path to the context and fill it
    CGContextAddPath(ctx, rectPath);
    CGContextFillPath(ctx);

    // Clean up the path memory
    CGPathRelease(rectPath);
}

void Canvas::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    if (!context) return;  // Ensure context is valid

    auto ctx = static_cast<CGContextRef>(context);

    CGContextSaveGState(ctx);  // Save the current state

    // Set the fill color (for example, gray)
    CGContextSetFillColorWithColor(ctx, createNSColor(fillStyle->getColor()));

    // Create _a rounded rectangle path
    CGPathRef roundedRectPath = CGPathCreateWithRoundedRect(
            CGRectMake(rect.x(), rect.y(), rect.width(), rect.height()),
            x_r, y_r, nullptr
    );

    // Add the path to the context and fill it
    CGContextAddPath(ctx, roundedRectPath);
    CGContextFillPath(ctx);

    // Clean up the path memory
    CGPathRelease(roundedRectPath);

    CGContextRestoreGState(ctx);  // Restore the previous state
}

void Canvas::setFillStyle(const FillStyle &style) {
    fillStyle = std::make_unique<FillStyle>(style);
}

