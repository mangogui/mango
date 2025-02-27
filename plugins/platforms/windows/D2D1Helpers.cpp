#include "D2D1Helpers.h"

D2D_RECT_F convertD2D1RectToRectF(const MNRect& rect) {
    return D2D1::RectF(rect.left(), rect.top(), rect.right(), rect.bottom());
}

D2D1_ROUNDED_RECT createD2D1RoundedRect(const MNRect &rect, int x_r, int y_r) {
    return D2D1::RoundedRect(convertD2D1RectToRectF(rect), static_cast<float>(x_r), static_cast<float>(y_r));
}

D2D1_COLOR_F convertColorToD2DColorF(const Color &color) {
    return D2D1::ColorF(
            static_cast<float>(color.red()) / 255.0f,
            static_cast<float>(color.green()) / 255.0f,
            static_cast<float>(color.blue()) / 255.0f,
            static_cast<float>(color.alpha()) / 255.0f
    );
}