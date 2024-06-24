#ifndef MANGO_COLOR_H
#define MANGO_COLOR_H

#include <string>

class Color {
public:
    Color(int r, int g, int b, int a = 255): r(r), g(g), b(b), a(a) {}
    Color(): r(102), g(255), b(102), a(1.0) {}
    explicit Color(const std::string& hexColor);

    int red() const { return r; }
    int green() const { return g; }
    int blue() const { return b; }
    int alpha() const {return a; }

    void setRGB(int r, int g, int b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
private:
    int r, g, b, a;
};

#endif //MANGO_COLOR_H
