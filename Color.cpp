#include "Color.h"
#include <string>
#include <iostream>


static inline int hex2int(const char *s, int n)
{
    if (n < 0)
        return -1;

    int res = 0;
    for (int i = n; i > 0; --i) {
        res += (s[i] - 48) * pow(16, n-i);
    }

    return res;
}

static inline bool fromHex(const char *name, int len, GUI::Color& color)
{
    // Check if the input string starts with a '#' character,
    if (name[0] != '#')
        return false;

    // 12-Character Format (RGBA)
    if (len == 12) {

    }
    // 9-Character Format (RGB)
    else if (len == 9) {

    }
    // 8-Character Format (ARGB)
    else if (len == 8) {

    }
    else if (len == 6) {
        int r = hex2int(name, 2);
        int g = hex2int(name + 2, 2);
        int b = hex2int(name + 4, 2);

        color.setRGB(r, g, b);
    }
    // 3-Character Format (RGB)
    else if (len == 3) {

    }

    return true;
}

namespace GUI {
    Color::Color(const std::string& hexColor) {
        fromHex(hexColor.c_str(), hexColor.length()-1, *this);
    }

}
