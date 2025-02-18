#pragma once

#include <PainterPath.h>
#include <Widget.h>
#include <Color.h>


class FillStyle {
    Color _color;
public:

    explicit FillStyle(const Color &color) : _color(color) {}

    void setColor(const Color &color) {
        this->_color = color;
    }

    [[nodiscard]] Color getColor() const { return _color; }
};

class Canvas {
    Widget *_widget;
    void *context{}; // Opaque pointer to hide Objective-C CGContextRef
    std::unique_ptr<FillStyle> fillStyle;
public:
    explicit Canvas(Widget *widget);

    void drawPath(const GUI::PainterPath &path);

    void drawRect(const MNRect &rect);

    void drawEllipse(const MNRect &rect);

    void drawRoundedRect(const MNRect &rect, int x_r, int y_r);

    void setFillStyle(const FillStyle &style);

    void begin();

    void end();

    ~Canvas();

private:
    void setClippingRegion();
};
