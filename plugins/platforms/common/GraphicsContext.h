#pragma once
#include <string>

#include "core/MNRect.h"
#include "core/Color.h"
#include "core/FillStyle.h"


class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

    virtual void clear(const Color &color) = 0;
    virtual void drawRectangle(const MNRect &rect) = 0;
    virtual void drawRoundedRect(const MNRect &rect, int x_r, int y_r) = 0;
    virtual void drawEllipse(const MNRect &rect) = 0;
    virtual void setFillStyle(const FillStyle &style) = 0;
};
