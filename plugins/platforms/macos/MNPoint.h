#pragma once

#include <iostream>

class MNPoint {
public:
    inline MNPoint() noexcept;

    inline MNPoint(float x, float y);

    inline void set_x(float x);

    inline void set_y(float y);

    inline float x() const noexcept;

    inline float y() const noexcept;

    float length() const {
        return std::sqrt(xp * xp + yp * yp);
    }

    inline MNPoint normalized() const;

    inline MNPoint operator*(float factor) const {
        return MNPoint(xp * factor, yp * factor);
    }

    inline MNPoint operator*(double factor) const {
        return MNPoint(xp * factor, yp * factor);
    }

    inline MNPoint operator+(const MNPoint& other) const {
        return MNPoint(xp + other.xp, yp + other.yp);
    }

    inline MNPoint operator-(const MNPoint& other) const {
        return MNPoint(xp - other.xp, yp - other.yp);
    }

    inline bool operator==(const MNPoint& _MNPoint) const {
        return _MNPoint.x() == this->xp && _MNPoint.y() == this->yp;
    }

    inline bool operator!=(const MNPoint& _MNPoint) const {
        return _MNPoint.x() != this->xp || _MNPoint.y() != this->yp;
    }

private:
    float xp, yp;
};

inline MNPoint::MNPoint() noexcept: xp(-1), yp(-1) {}

inline MNPoint::MNPoint(float x, float y) : xp(x), yp(y) {}

inline float MNPoint::x() const noexcept {
    return xp;
}

inline float MNPoint::y() const noexcept {
    return yp;
}

inline void MNPoint::set_x(float x) {
    xp = x;
}

inline void MNPoint::set_y(float y) {
    yp = y;
}

MNPoint MNPoint::normalized() const {
    float len = length();
    if (len != 0) {
        return MNPoint(xp / len, yp / len);
    } else {
        // Handle the case of a zero-length vector (to avoid division by zero)
        return *this; // Return the original vector
    }
}
