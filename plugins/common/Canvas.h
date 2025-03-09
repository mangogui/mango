#pragma once

#include <PainterPath.h>
#include <AbstractWidget.h>
#include <Color.h>
#include <FillStyle.h>
#include <GraphicsContext.h>


class Canvas {
    AbstractWidget *_widget;
    std::unique_ptr<FillStyle> fillStyle;
public:
    explicit Canvas(AbstractWidget *widget);

    void drawRect(const MNRect &rect);

    void drawEllipse(const MNRect &rect);

    void drawRoundedRect(const MNRect &rect, int x_r, int y_r);

    void setFillStyle(const FillStyle &style);

    void begin();

    void end();

    ~Canvas();

private:
    GraphicsContext* context;
};
