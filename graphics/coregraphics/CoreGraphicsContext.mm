#include "CoreGraphicsContext.h"
#include <CoreGraphicsHelper.h>

CoreGraphicsContext::CoreGraphicsContext(CGContextRef context) : m_pContext(context), m_pColor(nullptr) {
    if (m_pContext) {
        CGContextRetain(m_pContext);
    }
    m_pColor = CGColorCreateGenericRGB(1.0, 1.0, 1.0, 1.0);
}

CoreGraphicsContext::~CoreGraphicsContext() {
    if (m_pContext) {
        CGContextRelease(m_pContext);
    }
    if (m_pColor) {
        CGColorRelease(m_pColor);
    }
}

void CoreGraphicsContext::beginDraw() {
    if (!m_pContext) return;
    CGContextSaveGState(m_pContext);
    CGContextSetFillColorWithColor(m_pContext, m_pColor);
    CGContextFillRect(m_pContext, CGRectMake(0, 0, CGBitmapContextGetWidth(m_pContext), CGBitmapContextGetHeight(m_pContext)));
}

void CoreGraphicsContext::endDraw() {
    if (!m_pContext) return;
    CGContextRestoreGState(m_pContext);
}

void CoreGraphicsContext::clear(const Color &color) {
    if (!m_pContext) return;

    CGColorRelease(m_pColor); // Release the old color
    m_pColor = convertColorToCGColor(color);

    CGContextSetFillColorWithColor(m_pContext, m_pColor);
    CGContextFillRect(m_pContext, CGRectMake(0, 0, CGBitmapContextGetWidth(m_pContext), CGBitmapContextGetHeight(m_pContext)));
}

void CoreGraphicsContext::drawEllipse(const MNRect &rect) {
    if (!m_pContext) return;

    CGContextSetFillColorWithColor(m_pContext, m_pColor);
    CGContextFillEllipseInRect(m_pContext, CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()));
}

void CoreGraphicsContext::drawRectangle(const MNRect &rect) {
    if (!m_pContext) return;

    CGContextSetFillColorWithColor(m_pContext, m_pColor);
    CGContextFillRect(m_pContext, CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()));
}

void CoreGraphicsContext::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    if (!m_pContext) return;

    CGPathRef path = CGPathCreateWithRoundedRect(
        CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()),
        x_r, y_r, nullptr
    );

    CGContextAddPath(m_pContext, path);
    CGContextSetFillColorWithColor(m_pContext, m_pColor);
    CGContextFillPath(m_pContext);

    CGPathRelease(path);
}



void CoreGraphicsContext::setFillStyle(const FillStyle &style) {
    m_fillStyle = std::make_unique<FillStyle>(style);
    if (!m_pContext) return;

    CGColorRelease(m_pColor); // Release the old color
    m_pColor = convertColorToCGColor(style.getColor());
}


