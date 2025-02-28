#pragma once

#include <PainterPath.h>
#include <Widget.h>
#include <Color.h>
#include <memory>

#include <Direct2DGraphicsContext.h>
#include <FillStyle.h>

class Canvas {
    Widget *_widget;

public:
    explicit Canvas(Widget *widget);

    void drawRect(const MNRect &rect);

    void drawEllipse(const MNRect &rect);

    void drawRoundedRect(const MNRect &rect, int x_r, int y_r);

    void setFillStyle(const FillStyle &style);

    void begin();

    void end();

    ~Canvas();

private:
    Direct2DGraphicsContext* context;
    void createRenderTarget();
};
