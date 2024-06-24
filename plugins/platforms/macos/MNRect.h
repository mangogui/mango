#ifndef MNRect_H
#define MNRect_H

#include "MNPoint.h"
#include <stdlib.h>
#include <ostream>

class MNRect {
public:
    inline MNRect() = default;

    inline MNRect(float x, float y, float w, float h);

    inline void set_x(float x);

    inline void set_y(float y);

    inline void set_width(float w);

    inline void set_height(float h);

    inline float x() const noexcept;

    inline float y() const noexcept;

    inline float width() const noexcept;

    inline float height() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const MNRect& rect) {
        os << "MNRect(x=" << rect.x() << ", y=" << rect.y() << ", width=" << rect.width() << ", height=" << rect.height() << ")";
        return os;
    }

private:
    MNPoint p1, p2;
};

inline MNRect::MNRect(float x, float y, float w, float h) {
    p1.set_x(x);
    p1.set_y(y);
    p2.set_x(x + w);
    p2.set_y(y + h);
}

inline void MNRect::set_x(float x) {
    p1.set_x(x);
}

inline void MNRect::set_y(float y) {
    p1.set_y(y);
}

inline void MNRect::set_width(float w) {
    float x = p1.x() + w;
    p2.set_x(x);
}

inline void MNRect::set_height(float h) {
    float y = p1.y() + h;
    p2.set_y(y);
}

inline float MNRect::x() const noexcept {
    return p1.x();
}

inline float MNRect::y() const noexcept {
    return p1.y();
}

inline float MNRect::width() const noexcept {
    return abs(p2.x() - p1.x());
}

inline float MNRect::height() const noexcept {
    return abs(p2.y() - p1.y());
}
#endif //MNRect_H
