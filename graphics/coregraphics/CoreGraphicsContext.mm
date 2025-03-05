#include "CoreGraphicsContext.h"
#include <CoreGraphicsHelper.h>

CoreGraphicsContext::CoreGraphicsContext(void* context) :
        GraphicsContext(), m_pColor(nullptr) {
    setNativeContext(context);
    if (m_context) {
        CGContextRetain((CGContext*) m_context);
    }
    m_pColor = CGColorCreateGenericRGB(1.0, 1.0, 1.0, 1.0);
}

CoreGraphicsContext::~CoreGraphicsContext() {
    if (m_context) {
        CGContextRelease((CGContext*)m_context);
    }
    if (m_pColor) {
        CGColorRelease(m_pColor);
    }
}

void CoreGraphicsContext::beginDraw() {
    if (!m_context) return;
    CGContextSaveGState((CGContext*)m_context);
    CGContextSetFillColorWithColor((CGContext*)m_context, m_pColor);
    CGContextFillRect((CGContext*)m_context, CGRectMake(0, 0, CGBitmapContextGetWidth((CGContext*)m_context), CGBitmapContextGetHeight((CGContext*)m_context)));
}

void CoreGraphicsContext::endDraw() {
    if (!m_context) return;
    CGContextRestoreGState((CGContext*)m_context);
}

void CoreGraphicsContext::clear(const Color &color) {
    if (!m_context) return;

    CGColorRelease(m_pColor); // Release the old color
    m_pColor = convertColorToCGColor(color);

    CGContextSetFillColorWithColor((CGContext*)m_context, m_pColor);
    CGContextFillRect((CGContext*)m_context, CGRectMake(0, 0, CGBitmapContextGetWidth((CGContext*)m_context), CGBitmapContextGetHeight((CGContext*)m_context)));
}

void CoreGraphicsContext::drawEllipse(const MNRect &rect) {
    if (!m_context) return;

    CGContextSetFillColorWithColor((CGContext*)m_context, m_pColor);
    CGContextFillEllipseInRect((CGContext*)m_context, CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()));
}

void CoreGraphicsContext::drawRectangle(const MNRect &rect) {
    if (!m_context) return;

    CGContextSetFillColorWithColor((CGContext*)m_context, m_pColor);
    CGContextFillRect((CGContext*)m_context, CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()));
}

void CoreGraphicsContext::drawRoundedRect(const MNRect &rect, int x_r, int y_r) {
    if (!m_context) return;

    CGPathRef path = CGPathCreateWithRoundedRect(
            CGRectMake(rect.left(), rect.top(), rect.width(), rect.height()),
            x_r, y_r, nullptr
    );

    CGContextAddPath((CGContext*)m_context, path);
    CGContextSetFillColorWithColor((CGContext*)m_context, m_pColor);
    CGContextFillPath((CGContext*)m_context);

    CGPathRelease(path);
}

void CoreGraphicsContext::setFillStyle(const FillStyle &style) {
    m_fillStyle = std::make_unique<FillStyle>(style);
    if (!m_context) return;

    CGColorRelease(m_pColor); // Release the old color
    m_pColor = convertColorToCGColor(style.getColor());
}


