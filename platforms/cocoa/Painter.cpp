#include "Painter.h"

namespace GUI {
    void Painter::drawRectangle(const Rect &rect) {
        _window->painterPath().addRect(rect);
    }

    void Painter::drawEllipse(const Rect& rect) {
        _window->painterPath().addEllipse(rect);
    }

    void Painter::drawRoundedRect(const Rect& rect, float border_radius) {
        _window->painterPath().addRoundedRect(rect, border_radius);
    }
}