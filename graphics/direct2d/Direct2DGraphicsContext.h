#pragma once

#include <memory>

#include <d2d1.h>
#include <dwrite.h>
#include "core/FillStyle.h"
#include "core/MNRect.h"
#include <GraphicsContext.h>


class Direct2DGraphicsContext: public GraphicsContext {
public:
    explicit Direct2DGraphicsContext(void* winId);
    ~Direct2DGraphicsContext() override;

    void beginDraw() override;
    void endDraw() override;

    void clear(const Color &color) override;
    void drawEllipse(const MNRect &rect) override;
    void drawRectangle(const MNRect &rect) override;
    void drawRoundedRect(const MNRect &rect, int x_r, int y_r) override;
    void setFillStyle(const FillStyle &style) override;

    void resizeContext(int width, int height) override;

private:
    void* m_hWnd;

    ID2D1Factory *m_pD2DFactory = nullptr;
    ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;
    ID2D1BitmapRenderTarget *m_pBackBuffer = nullptr;
    ID2D1SolidColorBrush *m_pBrush = nullptr;
    IDWriteFactory *m_pDWriteFactory = nullptr;
    IDWriteTextFormat *m_pTextFormat = nullptr;

    void initFactory();
    void initRenderTarget();
    void initBackBuffer();
    void initWriteFactory();
    void initTextFormat();
    void initBrush();
};