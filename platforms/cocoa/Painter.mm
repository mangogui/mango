#include "Painter.h"

namespace GUI {
    void Painter::drawRect(const Rect &rect) {
        _window->painterPath().addRect(rect);
    }

    void Painter::drawEllipse(const Rect& rect) {
        _window->painterPath().addEllipse(rect);
    }

    void Painter::drawRoundedRect(const Rect& rect, float x_radius, float y_radius) {
        _window->painterPath().addRoundedRect(rect, x_radius, y_radius);
    }
}