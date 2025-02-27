#pragma once

#include <iostream>
#include <cmath>

namespace GUI {
    class Point {
    public:
        constexpr inline Point() noexcept;

        constexpr inline Point(float x, float y);

        constexpr inline void setX(float x);

        constexpr inline void setY(float y);

        [[nodiscard]] constexpr inline float x() const noexcept;

        [[nodiscard]] constexpr inline float y() const noexcept;

        [[nodiscard]] float length() const {
            return std::sqrt(xp * xp + yp * yp);
        }

        inline Point normalized() const;

        constexpr inline Point operator*(float factor) const {
            return {xp * factor, yp * factor};
        }

        constexpr inline Point operator*(double factor) const {
            return Point(xp * factor, yp * factor);
        }

        constexpr inline Point operator+(const Point &other) const {
            return {xp + other.xp, yp + other.yp};
        }

        constexpr inline Point operator-(const Point &other) const {
            return {xp - other.xp, yp - other.yp};
        }

        constexpr inline bool operator==(const Point &_point) const {
            return _point.x() == this->xp && _point.y() == this->yp;
        }

        constexpr inline bool operator!=(const Point &_point) const {
            return _point.x() != this->xp || _point.y() != this->yp;
        }

        friend std::ostream &operator<<(std::ostream &os, const Point &point) {
            os << "Point(" << point.x() << ", " << point.y() << ")";
            return os;
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

    constexpr inline void Point::setX(float x) {
        xp = x;
    }

    constexpr inline void Point::setY(float y) {
        yp = y;
    }

    Point Point::normalized() const {
        float len = length();
        if (len != 0) {
            return {xp / len, yp / len};
        } else {
            // Handle the case of _a zero-length vector (to avoid division by zero)
            return *this; // Return the original vector
        }
    }
}
