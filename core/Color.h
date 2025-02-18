#pragma once

#include <string>


class Color {
public:
    Color(int r, int g, int b, int a = 255) : _r(r), _g(g), _b(b), _a(a) {}

    Color() : _r(102), _g(255), _b(102), _a(1.0) {}

    explicit Color(const std::string &hexColor);

    [[nodiscard]] int red() const { return _r; }

    [[nodiscard]] int green() const { return _g; }

    [[nodiscard]] int blue() const { return _b; }

    [[nodiscard]] int alpha() const { return _a; }

    void setRed(int r) { this->_r = r; }

    void setGreen(int g) { this->_g = g; }

    void setBlue(int b) { this->_b = b; }

    void setAlpha(int a) { this->_a = a; }

    void setRGB(int r, int g, int b) {
        this->_r = r;
        this->_g = g;
        this->_b = b;
    }

    void setRGBA(int r, int g, int b, int a) {
        this->_r = r;
        this->_g = g;
        this->_b = b;
        this->_a = a;
    }

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Gray;
    static const Color DarkGray;
    static const Color LightGray;
    static const Color Orange;
    static const Color Purple;
    static const Color Brown;
    static const Color NoColor;

private:
    int _r, _g, _b, _a;
};