#include "Color.h"
#include <string>
#include <cctype>  // for std::toupper

static inline int hex2int(const char *s, int n) {
    if (n <= 0) return -1;  // Validate input

    int res = 0;
    for (int i = 0; i < n; ++i) {
        char c = std::toupper(s[i]);  // Convert to uppercase
        int value = 0;

        if (c >= '0' && c <= '9') {
            value = c - '0';  // Convert '0'-'9' to 0-9
        } else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;  // Convert 'A'-'F' to 10-15
        } else {
            return -1;  // Invalid hex character
        }

        res = (res << 4) | value;  // Multiply by 16 (bit shift) and add new value
    }

    return res;
}

static inline bool fromHex(const char *name, int len, Color &color) {
    // Check if the input string starts with '#'
    if (name[0] != '#') return false;

    // Move `name` pointer forward to skip `#`
    name++;

    if (len == 12) {  // RRGGBBAA
        int r = hex2int(name, 2);
        int g = hex2int(name + 2, 2);
        int b = hex2int(name + 4, 2);
        int a = hex2int(name + 6, 2);

        if (r < 0 || g < 0 || b < 0 || a < 0) return false;
        color.setRGBA(r, g, b, a);
    } else if (len == 9) {  // RRGGBB (no alpha)
        int r = hex2int(name, 2);
        int g = hex2int(name + 2, 2);
        int b = hex2int(name + 4, 2);

        if (r < 0 || g < 0 || b < 0) return false;
        color.setRGB(r, g, b);
        color.setAlpha(255);  // Default alpha to 255
    } else if (len == 8) {  // ARGB
        int a = hex2int(name, 2);
        int r = hex2int(name + 2, 2);
        int g = hex2int(name + 4, 2);
        int b = hex2int(name + 6, 2);

        if (r < 0 || g < 0 || b < 0 || a < 0) return false;
        color.setRGBA(r, g, b, a);
    } else if (len == 6) {  // RGB
        int r = hex2int(name, 2);
        int g = hex2int(name + 2, 2);
        int b = hex2int(name + 4, 2);

        if (r < 0 || g < 0 || b < 0) return false;
        color.setRGB(r, g, b);
        color.setAlpha(255);  // Default alpha to 255
    } else if (len == 3) {  // Short RGB (#RGB)
        int r = hex2int(name, 1);
        int g = hex2int(name + 1, 1);
        int b = hex2int(name + 2, 1);

        if (r < 0 || g < 0 || b < 0) return false;
        color.setRGB(r * 17, g * 17, b * 17);  // Expand 4-bit values (0xF -> 0xFF)
        color.setAlpha(255);
    } else {
        return false;  // Invalid length
    }

    return true;
}

Color::Color(const std::string &hexColor) {
    // Ensure correct length handling and pass full length (including `#`)
    if (!fromHex(hexColor.c_str(), hexColor.length() - 1, *this)) {
        // Handle invalid input (e.g., default to black)
        setRGBA(0, 0, 0, 255);
    }
}

const Color Color::Black(0, 0, 0);          // Black: RGB (0, 0, 0)
const Color Color::White(255, 255, 255);    // White: RGB (255, 255, 255)
const Color Color::Red(255, 0, 0);          // Red: RGB (255, 0, 0)
const Color Color::Green(0, 255, 0);        // Green: RGB (0, 255, 0)
const Color Color::Blue(0, 0, 255);         // Blue: RGB (0, 0, 255)
const Color Color::Yellow(255, 255, 0);     // Yellow: RGB (255, 255, 0)
const Color Color::Cyan(0, 255, 255);       // Cyan: RGB (0, 255, 255)
const Color Color::Magenta(255, 0, 255);    // Magenta: RGB (255, 0, 255)
const Color Color::Gray(128, 128, 128);     // Gray: RGB (128, 128, 128)
const Color Color::DarkGray(169, 169, 169); // DarkGray: RGB (169, 169, 169)
const Color Color::LightGray(211, 211, 211);// LightGray: RGB (211, 211, 211)
const Color Color::Orange(255, 165, 0);     // Orange: RGB (255, 165, 0)
const Color Color::Purple(128, 0, 128);     // Purple: RGB (128, 0, 128)
const Color Color::Brown(165, 42, 42);      // Brown: RGB (165, 42, 42)
const Color Color::NoColor(0, 0, 0, 0);     // NoColor: Transparent (Alpha = 0)
