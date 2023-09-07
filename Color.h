#ifndef MANGO_COLOR_H
#define MANGO_COLOR_H

#include <simd/simd.h>

namespace GUI {
    class Color {
    public:
        Color(int r, int g, int b, int a = 255): r(r), g(g), b(b), a(a) {}
        Color(): r(102), g(255), b(102), a(1.0) {}
        int red() const { return r; }
        int green() const { return g; }
        int blue() const { return b; }
        int alpha() const {return a; }
        simd_float4 toSimdFloat4() const { return simd_make_float4(float(r)/255, float(g)/255, float(b)/255, float(a)/255); }
    private:
        int r, g, b, a;
    };
}

#endif //MANGO_COLOR_H
