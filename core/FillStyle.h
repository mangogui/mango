#pragma once

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
