#pragma once
#include <string>

#include <MNRect.h>
#include <Color.h>
#include <FillStyle.h>


class GraphicsContext {

public:
    GraphicsContext() : m_context(nullptr) {}
    virtual ~GraphicsContext() = default;

    virtual void beginDraw() = 0;
    virtual void endDraw() = 0;

    virtual void clear(const Color &color) = 0;
    virtual void drawRectangle(const MNRect &rect) = 0;
    virtual void drawRoundedRect(const MNRect &rect, int x_r, int y_r) = 0;
    virtual void drawEllipse(const MNRect &rect) = 0;
    virtual void setFillStyle(const FillStyle &style) = 0;

    virtual void resizeContext(int width, int height) = 0;

    void setNativeContext(void* context) {
        m_context = context;
    }

    void* getNativeContext() {
        return m_context;
    }
protected:
    void* m_context;
};
