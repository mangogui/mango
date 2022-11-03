#ifndef GUI_SIZE_H
#define GUI_SIZE_H
#include <iostream>

namespace GUI {

    class Size {
    public:
        constexpr inline Size();

        constexpr inline Size(int w, int h);

        constexpr inline void set_width(int w);

        constexpr inline void set_height(int w);

        constexpr inline int get_width() const;

        constexpr inline int get_height() const;

        inline friend std::ostream &operator<<(std::ostream &os, const Size &size);

    private:
        int width, height;
    };

    constexpr inline Size::Size() : width(-1), height(-1) {}

    constexpr inline Size::Size(int w, int h) : width(w), height(h) {}

    constexpr inline void Size::set_width(int w) { width = w; }

    constexpr inline void Size::set_height(int h) { height = h; }

    constexpr inline int Size::get_width() const { return width; }

    constexpr inline int Size::get_height() const { return height; }

    std::ostream &operator<<(std::ostream &os, const Size &size) {
        os << "Size" << '(' << size.width << ", " << size.height << ')';
        return os;
    }

}

#endif //GUI_SIZE_H
