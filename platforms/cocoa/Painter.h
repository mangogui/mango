#ifndef MANGO_PAINTER_H
#define MANGO_PAINTER_H

#include "../../Rect.h"
#include "../../Color.h"

namespace GUI {
    class Painter {
    public:
        Painter(): color(Color(102, 255, 102, 255)) {}
        void setColor(const Color &newColor) { color = newColor; };
        void drawRectangle(const Rect& rect);
        void drawRoundedRectangle(const Rect &rect, int x_radius, int y_radius);
        void drawEllipse(const Rect& rect);
    private:
        Color color;
    };
}

#endif //MANGO_PAINTER_H
