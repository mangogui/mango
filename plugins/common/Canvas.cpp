#include <Canvas.h>


Canvas::Canvas(AbstractWidget *widget) : _widget(widget), context((GraphicsContext*)widget->getGraphicsContext()) {
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

