#pragma once

#include <Color.h>
#include <CoreGraphics/CoreGraphics.h>

CGColorRef convertColorToCGColor(const Color &color) {
    return CGColorCreateGenericRGB(color.red()/255.0, color.green()/255.0, color.blue()/255.0, color.alpha()/255.0);
}

