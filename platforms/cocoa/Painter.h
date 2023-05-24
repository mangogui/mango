#ifndef MANGO_PAINTER_H
#define MANGO_PAINTER_H

#include "../../Rect.h"
#include "CocoaWindow.h"
#include "../../Color.h"
#include <map>

namespace GUI {
    class Painter {
    public:
        explicit Painter(CocoaWindow* window): _window(window) {}
        void drawRectangle(const Rect& rect);
        void drawEllipse(const Rect& rect);
        void drawRoundedRect(const Rect& rect, float border_radius);
        void setFillColor(const Color color) {
            int index = _window->painterPath().elementCount();
            _window->states()[index] = {color};
        }
    private:
        CocoaWindow* _window;
    };
}

#endif //MANGO_PAINTER_H
