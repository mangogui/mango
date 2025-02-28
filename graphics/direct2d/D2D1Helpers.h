#include "MNRect.h"
#include "Color.h"
#include <d2d1.h>         // Direct2D core API
#include <d2d1helper.h>   // Direct2D helper functions
#include <d2d1_1.h>       // Direct2D 1.1 API (optional, for newer features)
#include <dwrite.h>       // DirectWrite (if text rendering is needed)
#include <wrl/client.h>   // Microsoft WRL (smart pointers for COM objects)


D2D_RECT_F convertD2D1RectToRectF(const MNRect& rect);
D2D1_ROUNDED_RECT createD2D1RoundedRect(const MNRect &rect, int x_r, int y_r);
D2D1_COLOR_F convertColorToD2DColorF(const Color &color);
