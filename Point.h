#pragma once

#include <iostream>

namespace GUI {
    class Point {
    public:
        constexpr inline Point() noexcept;

        constexpr inline Point(float x, float y);

        constexpr inline void set_x(float x);

        constexpr inline void set_y(float y);

        constexpr inline float x() const noexcept;

        constexpr inline float y() const noexcept;

        float length() const {
            return std::sqrt(xp * xp + yp * yp);
        }

        inline Point normalized() const;

        constexpr inline Point operator*(float factor) const {
            return Point(xp * factor, yp * factor);
        }

        constexpr inline Point operator*(double factor) const {
            return Point(xp * factor, yp * factor);
        }

        constexpr inline Point operator+(const Point& other) const {
            return Point(xp + other.xp, yp + other.yp);
        }

        constexpr inline Point operator-(const Point& other) const {
            return Point(xp - other.xp, yp - other.yp);
        }

        constexpr inline bool operator==(const Point& _point) {
            return _point.x() == this->xp && _point.y() == this->yp;
        }

        constexpr inline bool operator!=(const Point& _point) {
            return _point.x() != this->xp || _point.y() != this->yp;
        }

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

    Point Point::normalized() const {
        float len = length();
        if (len != 0) {
            return Point(xp / len, yp / len);
        } else {
            // Handle the case of a zero-length vector (to avoid division by zero)
            return *this; // Return the original vector
        }
    }
}
