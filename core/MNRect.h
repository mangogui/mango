#pragma once

#include <Point.h>
#include <cstdlib>
#include <ostream>
#include <MNSize.h>

class MNRect {
public:
    inline MNRect() = default;

    inline MNRect(float x, float y, float w, float h);

    inline void set_x(float x);

    inline void set_y(float y);

    inline void set_width(float w);

    inline void set_height(float h);

    void resize(float width, float height) {
        set_width(width);
        set_height(height);
    }

    [[nodiscard]] inline MNSize size() const;

    inline void move(float new_x, float new_y);

    [[nodiscard]] inline float x() const noexcept;

    [[nodiscard]] inline float y() const noexcept;

    [[nodiscard]] inline float width() const noexcept;

    [[nodiscard]] inline float height() const noexcept;

    [[nodiscard]] inline float left() const noexcept;
    [[nodiscard]] inline float right() const noexcept;
    [[nodiscard]] inline float top() const noexcept;
    [[nodiscard]] inline float bottom() const noexcept;

    friend std::ostream &operator<<(std::ostream &os, const MNRect &rect) {
        os << "MNRect(x=" << rect.x() << ", y=" << rect.y() << ", m_width=" << rect.width() << ", m_height="
           << rect.height() << ")";
        return os;
    }

private:
    GUI::Point p1, p2;
};

inline MNRect::MNRect(float x, float y, float w, float h) : p1(x, y), p2(x + w, y + h) {}

inline void MNRect::set_x(float x) {
    p1.setX(x);
}

inline void MNRect::set_y(float y) {
    p1.setY(y);
}

inline void MNRect::set_width(float w) {
    float x = p1.x() + w;
    p2.setX(x);
}

inline void MNRect::set_height(float h) {
    float y = p1.y() + h;
    p2.setY(y);
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

float MNRect::left() const noexcept {
    return p1.x();
}

float MNRect::right() const noexcept {
    return p2.x();
}

float MNRect::top() const noexcept {
    return p1.y();
}

float MNRect::bottom() const noexcept {
    return p2.y();
}

void MNRect::move(float new_x, float new_y) {
    float w = width();
    float h = height();
    p1.setX(new_x);
    p1.setY(new_y);
    p2.setX(new_x + w);
    p2.setY(new_y + h);
}

MNSize MNRect::size() const {
    return MNSize(width(), height());
}

