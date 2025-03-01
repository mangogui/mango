#pragma once

#include <CoreGraphics/CoreGraphics.h>
#include <GraphicsContext.h>

class CoreGraphicsContext : public GraphicsContext {
public:
    explicit CoreGraphicsContext(CGContextRef context);
    ~CoreGraphicsContext() override;

    void beginDraw() override;
    void endDraw() override;

    void clear(const Color &color) override;
    void drawEllipse(const MNRect &rect) override;
    void drawRectangle(const MNRect &rect) override;
    void drawRoundedRect(const MNRect &rect, int x_r, int y_r) override;
    void setFillStyle(const FillStyle &style) override;

    void setCGContextRef(CGContextRef _context) {
        m_pContext = _context;
    }

private:
    std::unique_ptr<FillStyle> m_fillStyle;
    CGContextRef m_pContext;
    CGColorRef m_pColor;
};

