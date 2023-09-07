#ifndef MANGO_RECT_H
#define MANGO_RECT_H

#include "Point.h"
#include <stdlib.h>
#include <ostream>

namespace GUI {
    class Rect {
    public:
        constexpr inline Rect() = default;

        constexpr inline Rect(float x, float y, float w, float h);

        constexpr inline void set_x(float x);

        constexpr inline void set_y(float y);

        constexpr inline void set_width(float w);

        constexpr inline void set_height(float h);

        constexpr inline float x() const noexcept;

        constexpr inline float y() const noexcept;

        inline float width() const noexcept;

        inline float height() const noexcept;

        friend std::ostream& operator<<(std::ostream& os, const Rect& rect) {
            os << "Rect(x=" << rect.x() << ", y=" << rect.y() << ", width=" << rect.width() << ", height=" << rect.height() << ")";
            return os;
        }

    private:
        Point p1, p2;
    };

    constexpr inline Rect::Rect(float x, float y, float w, float h) {
        p1.set_x(x);
        p1.set_y(y);
        p2.set_x(x + w);
        p2.set_y(y + h);
    }

    constexpr inline void Rect::set_x(float x) {
        p1.set_x(x);
    }

    constexpr inline void Rect::set_y(float y) {
        p1.set_y(y);
    }

    constexpr inline void Rect::set_width(float w) {
        float x = p1.x() + w;
        p2.set_x(x);
    }

    constexpr inline void Rect::set_height(float h) {
        float y = p1.y() + h;
        p2.set_y(y);
    }

    constexpr inline float Rect::x() const noexcept {
        return p1.x();
    }

    constexpr inline float Rect::y() const noexcept {
        return p1.y();
    }

    inline float Rect::width() const noexcept {
        return abs(p2.x() - p1.x());
    }

    inline float Rect::height() const noexcept {
        return abs(p2.y() - p1.y());
    }
}
#endif //MANGO_RECT_H
