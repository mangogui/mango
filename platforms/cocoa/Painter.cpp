#include "Painter.h"

namespace GUI {
    void Painter::drawRectangle(const Rect &rect) {
        _window->painterPath().addRect(rect);
    }

    void Painter::drawEllipse(const Rect& rect) {
        _window->painterPath().addEllipse(rect);
    }
}