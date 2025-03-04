#include "Canvas.h"
#include <Direct2DGraphicsContext.h>


Canvas::Canvas(Widget *widget) : _widget(widget), context(widget->getGraphicsContext()) {
}

void Canvas::createRenderTarget() {
    setFillStyle(FillStyle(Color::White));
}

void Canvas::begin() {
    context->beginDraw();
}

void Canvas::end() {
    context->endDraw();
}

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
    context->setFillStyle(style);
}

Canvas::~Canvas() {
}
