#ifndef MANGO_COLOR_H
#define MANGO_COLOR_H

namespace GUI {
    class Color {
    public:
        Color(int r, int g, int b, int a = 255): r(r), g(g), b(b), a(a) {}
        int red() const { return r; }
        int green() const { return g; }
        int blue() const { return b; }
        int alpha() const {return a; }
    private:
        int r, g, b, a;
    };
}

#endif //MANGO_COLOR_H
