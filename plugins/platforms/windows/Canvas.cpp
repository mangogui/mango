#include "Canvas.h"
#include <d2d1helper.h>
#include <dwrite.h>
#include <windows.h>
#include "D2D1Helpers.h"


Canvas::Canvas(Widget *widget) : _widget(widget) {

}

void Canvas::createRenderTarget() {


    setFillStyle(FillStyle(Color::White));
}

void Canvas::begin() {
    if (!_widget->getBackBuffer()) return;
    _widget->getBackBuffer()->BeginDraw();
    _widget->getBackBuffer()->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void Canvas::end() {
    if (!_widget->getRenderTarget() || !_widget->getBackBuffer()) return;

    HRESULT hr = _widget->getBackBuffer()->EndDraw();
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to end back buffer draw", L"Error", MB_OK);
        return;
    }

    ID2D1Bitmap *pBitmap = nullptr;
    hr = _widget->getBackBuffer()->GetBitmap(&pBitmap);
    if (SUCCEEDED(hr)) {
        _widget->getRenderTarget()->BeginDraw();
        _widget->getRenderTarget()->DrawBitmap(pBitmap);
        hr = _widget->getRenderTarget()->EndDraw();
        pBitmap->Release();
    }

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to end render target draw", L"Error", MB_OK);
    }
}

void Canvas::drawEllipse(const MNRect &rect) {
    if (!_widget->getBackBuffer()) return;
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
            { (rect.left() + rect.right()) / 2.0f, (rect.top() + rect.bottom()) / 2.0f },
            (rect.right() - rect.left()) / 2.0f, (rect.bottom() - rect.top()) / 2.0f
    );
    _widget->getBackBuffer()->FillEllipse(ellipse, _widget->getBrush());
}

void Canvas::drawRect(const MNRect &rect) {
    if (!_widget->getBackBuffer()) return;
    _widget->getBackBuffer()->FillRectangle(convertD2D1RectToRectF(rect), _widget->getBrush());
}

void Canvas::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    if (!_widget->getBackBuffer()) return;
    _widget->getBackBuffer()->FillRoundedRectangle(createD2D1RoundedRect(rect, x_r, y_r), _widget->getBrush());
}

void Canvas::setFillStyle(const FillStyle &style) {
    // if (fillStyle && fillStyle->getColor() == style.getColor()) return;
    fillStyle = std::make_unique<FillStyle>(style);
    ID2D1SolidColorBrush* brush = _widget->getBrush();
    brush->SetColor(convertColorToD2DColorF(style.getColor()));
}

Canvas::~Canvas() {
}
