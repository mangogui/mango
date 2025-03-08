#pragma once

#include <iostream>

class MNSize {
public:
    inline MNSize();

    inline MNSize(int w, int h);

    inline void setWidth(int w);

    inline void setHeight(int w);

    [[nodiscard]] inline int getWidth() const;

    [[nodiscard]] inline int getHeight() const;

    inline friend std::ostream &operator<<(std::ostream &os, const MNSize &size);

private:
    int m_width, m_height;
};

inline MNSize::MNSize() : m_width(-1), m_height(-1) {}

inline MNSize::MNSize(int w, int h) : m_width(w), m_height(h) {}

inline void MNSize::setWidth(int w) { m_width = w; }

inline void MNSize::setHeight(int h) { m_height = h; }

inline int MNSize::getWidth() const { return m_width; }

inline int MNSize::getHeight() const { return m_height; }

std::ostream &operator<<(std::ostream &os, const MNSize &size) {
    os << "Size" << '(' << size.m_width << ", " << size.m_height << ')';
    return os;
}
