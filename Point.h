//
// Created by Parham Oyan on 29/12/2022.
//

#ifndef MANGO_POINT_H
#define MANGO_POINT_H

namespace GUI {
    class Point {
    public:
        constexpr inline Point() noexcept;

        constexpr inline Point(float x, float y);

        constexpr inline void set_x(float x);

        constexpr inline void set_y(float y);

        constexpr inline float x() const noexcept;

        constexpr inline float y() const noexcept;

    private:
        float xp, yp;
    };

    constexpr inline Point::Point() noexcept: xp(-1), yp(-1) {}

    constexpr inline Point::Point(float x, float y) : xp(x), yp(y) {}

    constexpr inline float Point::x() const noexcept {
        return xp;
    }

    constexpr inline float Point::y() const noexcept {
        return yp;
    }

    constexpr inline void Point::set_x(float x) {
        xp = x;
    }

    constexpr inline void Point::set_y(float y) {
        yp = y;
    }
}

#endif //MANGO_POINT_H
