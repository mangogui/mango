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

    void Painter::drawLine(const Point &p0, const Point &p1) {
        _window->painterPath().moveTo(p0);
        _window->painterPath().lineTo(p1);
    }

    void Painter::drawCubicBezierCurve(const Point &c1, const Point &c2, const Point &endPoint) {
        _window->painterPath().cubicTo(c1, c2, endPoint);
    }

    void Painter::moveTo(int x, int y) {
        _window->painterPath().moveTo(x, y);
    }

    void Painter::moveTo(const Point &p) {
        _window->painterPath().moveTo(p);
    }
}