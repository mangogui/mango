#include <Canvas.h>
#include <Cocoa/Cocoa.h>
#include <CoreGraphicsContext.h>

CGColor *createNSColor(const Color &color) {
    return [[NSColor colorWithCalibratedRed:color.red() / 255.0
                                      green:color.green() / 255.0
                                       blue:color.blue() / 255.0
                                      alpha:color.alpha() / 255.0] CGColor];
}

CGPathRef convertPainterPathToCGPath(const GUI::PainterPath &painterPath) {
    CGMutablePathRef cgPath = CGPathCreateMutable();

    for (int i = 0; i < painterPath.elementCount(); ++i) {
        const GUI::PainterPath::Element &element = painterPath.elementAt(i);

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

Canvas::Canvas(Widget *widget) : _widget(widget), context((GraphicsContext*)widget->getGraphicsContext()) {
    this->fillStyle = std::make_unique<FillStyle>(Color::Gray);

}

void Canvas::begin() {
    context->beginDraw();
}

void Canvas::end() {
    context->endDraw();
}

Canvas::~Canvas() {
    if (context) {
        context->endDraw();
    }
};

void Canvas::drawEllipse(const MNRect &rect) {
    context->drawEllipse(rect);
}

void Canvas::drawRect(const MNRect &rect) {
    context->drawRectangle(rect);
}

void Canvas::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    context->drawRoundedRect(rect, x_r, y_r);
}

void Canvas::setFillStyle(const FillStyle &style) {
    fillStyle = std::make_unique<FillStyle>(style);
    context->setFillStyle(style);
}

