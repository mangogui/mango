#include "Direct2DGraphicsContext.h"
#include "D2D1Helpers.h"

Direct2DGraphicsContext::Direct2DGraphicsContext(HWND winId)
        : m_hWnd(winId), m_pBrush(nullptr) {
    initFactory();
    initRenderTarget();
    initBackBuffer();
    initWriteFactory();
    initTextFormat();
    initBrush();
}

void Direct2DGraphicsContext::initFactory() {
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create Direct2D factory", L"Error", MB_OK);
        return;
    }
};

void Direct2DGraphicsContext::initRenderTarget() {
    RECT rc;
    GetClientRect(m_hWnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    HRESULT hr = m_pD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWnd, size),
            &m_pRenderTarget
    );

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create Direct2D render target", L"Error", MB_OK);
        return;
    }
}

void Direct2DGraphicsContext::initBackBuffer() {
    HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(&m_pBackBuffer);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create back buffer", L"Error", MB_OK);
        return;
    }
}

void Direct2DGraphicsContext::initWriteFactory() {
    HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create DirectWrite factory", L"Error", MB_OK);
        return;
    }
}

void Direct2DGraphicsContext::initTextFormat() {
    HRESULT hr = m_pDWriteFactory->CreateTextFormat(
        L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
        36.0f, L"en-us", &m_pTextFormat
    );
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create text format", L"Error", MB_OK);
    }
}

void Direct2DGraphicsContext::initBrush() {
    HRESULT hr = m_pRenderTarget->CreateSolidColorBrush(convertColorToD2DColorF(Color::White), &m_pBrush);
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to create solid color brush", L"Error", MB_OK);
    }
};

Direct2DGraphicsContext::~Direct2DGraphicsContext() {
    if (m_pBrush) {
        m_pBrush->Release();
        m_pBrush = nullptr;
    }

    if (m_pTextFormat) {
        m_pTextFormat->Release();
        m_pTextFormat = nullptr;
    }

    if (m_pDWriteFactory) {
        m_pDWriteFactory->Release();
        m_pDWriteFactory = nullptr;
    }

    if (m_pBackBuffer) {
        m_pBackBuffer->Release();
        m_pBackBuffer = nullptr;
    }

    if (m_pRenderTarget) {
        m_pRenderTarget->Release();
        m_pRenderTarget = nullptr;
    }

    if (m_pD2DFactory) {
        m_pD2DFactory->Release();
        m_pD2DFactory = nullptr;
    }
}

void Direct2DGraphicsContext::beginDraw() {
    if (!m_pBackBuffer) return;
    m_pBackBuffer->BeginDraw();
    m_pBackBuffer->Clear(D2D1::ColorF(D2D1::ColorF::White));
}

void Direct2DGraphicsContext::endDraw() {
    if (!m_pRenderTarget || !m_pBackBuffer) return;

    HRESULT hr = m_pBackBuffer->EndDraw();
    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to end back buffer draw", L"Error", MB_OK);
        return;
    }

    ID2D1Bitmap *pBitmap = nullptr;
    hr = m_pBackBuffer->GetBitmap(&pBitmap);
    if (SUCCEEDED(hr)) {
        m_pRenderTarget->BeginDraw();
        m_pRenderTarget->DrawBitmap(pBitmap);
        hr = m_pRenderTarget->EndDraw();
        pBitmap->Release();
    }

    if (FAILED(hr)) {
        MessageBoxW(nullptr, L"Failed to end render target draw", L"Error", MB_OK);
    }
}

void Direct2DGraphicsContext::resizeContext(int width, int height) {
    if (!m_pRenderTarget) return;  // Ensure the render target exists

    D2D1_SIZE_U newSize = D2D1::SizeU(width, height);
    m_pRenderTarget->Resize(newSize); // Resize render target to match widget

    if (m_pBackBuffer) {
        m_pBackBuffer->Release(); // Release old back buffer
        m_pBackBuffer = nullptr;

        HRESULT hr = m_pRenderTarget->CreateCompatibleRenderTarget(&m_pBackBuffer);
        if (FAILED(hr)) {
            MessageBoxW(nullptr, L"Failed to create resized back buffer", L"Error", MB_OK);
        }
    }
}

void Direct2DGraphicsContext::clear(const Color &color) {
    m_pRenderTarget->Clear(convertColorToD2DColorF(color));
}

void Direct2DGraphicsContext::drawEllipse(const MNRect &rect) {
    if (!m_pBrush) return;
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(
            { (rect.left() + rect.right()) / 2.0f, (rect.top() + rect.bottom()) / 2.0f },
            (rect.right() - rect.left()) / 2.0f, (rect.bottom() - rect.top()) / 2.0f
    );
    m_pBackBuffer->FillEllipse(ellipse, m_pBrush);
}

void Direct2DGraphicsContext::drawRectangle(const MNRect &rect) {
    m_pBackBuffer->FillRectangle(convertD2D1RectToRectF(rect), m_pBrush);
}

void Direct2DGraphicsContext::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    m_pBackBuffer->FillRoundedRectangle(createD2D1RoundedRect(rect, x_r, y_r), m_pBrush);
}

void Direct2DGraphicsContext::setFillStyle(const FillStyle &style) {
    m_fillStyle = std::make_unique<FillStyle>(style);
    m_pBrush->SetColor(convertColorToD2DColorF(style.getColor()));
}

