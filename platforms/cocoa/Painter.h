#ifndef MANGO_PAINTER_H
#define MANGO_PAINTER_H

#include "../../Rect.h"
#include "Widget.h"
#include "../../Color.h"
#include <map>

namespace GUI {
    class Painter {
    public:
        explicit Painter(Widget* window): _window(window) {}
        void drawRect(const Rect& rect);
        void drawEllipse(const Rect& rect);
        void drawRoundedRect(const Rect& rect, float x_radius, float y_radius);
        void setFillColor(const Color color) {
            int index = _window->painterPath().elementCount();
            _window->states()[index] = {color};
        }
    private:
        Widget* _window;
    };
}

#endif //MANGO_PAINTER_H
