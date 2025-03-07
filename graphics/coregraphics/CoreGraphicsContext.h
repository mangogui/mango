#pragma once

#include <CoreGraphics/CoreGraphics.h>
#include <GraphicsContext.h>

class CoreGraphicsContext : public GraphicsContext {
public:
    explicit CoreGraphicsContext(void* context);
    ~CoreGraphicsContext() override;

    void beginDraw() override;
    void endDraw() override;

    void clear(const Color &color) override;
    void drawEllipse(const MNRect &rect) override;
    void drawRectangle(const MNRect &rect) override;
    void drawRoundedRect(const MNRect &rect, int x_r, int y_r) override;
    void setFillStyle(const FillStyle &style) override;

    void resizeContext(int width, int height) override {
        // TODO: Important
        // On Windows, we need to resize the render target however on macOS, we don't need to do anything
    }

private:
    std::unique_ptr<FillStyle> m_fillStyle;
    CGColorRef m_pColor;
};

