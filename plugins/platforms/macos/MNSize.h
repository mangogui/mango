#ifndef SIZE_H
#define SIZE_H

#include <iostream>

class MNSize {
public:
    inline MNSize();

    inline MNSize(int w, int h);

    inline void set_width(int w);

    inline void set_height(int w);

    inline int get_width() const;

    inline int get_height() const;

    inline friend std::ostream &operator<<(std::ostream &os, const MNSize &size);

private:
    int width, height;
};

inline MNSize::MNSize() : width(-1), height(-1) {}

inline MNSize::MNSize(int w, int h) : width(w), height(h) {}

inline void MNSize::set_width(int w) { width = w; }

inline void MNSize::set_height(int h) { height = h; }

inline int MNSize::get_width() const { return width; }

inline int MNSize::get_height() const { return height; }

std::ostream &operator<<(std::ostream &os, const MNSize &size) {
    os << "Size" << '(' << size.width << ", " << size.height << ')';
    return os;
}

#endif //SIZE_H
