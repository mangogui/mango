#pragma once

#include <string>
#include <MNRect.h>
#include <Object.h>
#include <Event.h>

#include <ResizeEvent.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <GraphicsContext.h>

class Widget;
class PlatformView;


class PlatformWindow: public Object {
protected:
    void* m_nativeObject;
    Widget* m_widget;
    void* m_nativeContext;
    void* m_graphicsContext;
public:
    explicit PlatformWindow(Widget *widget): Object(), m_widget(widget), m_nativeObject(nullptr) {};

    virtual ~PlatformWindow() = default;

    // Setters
    virtual void setTitle(const std::string &title) = 0;
    virtual void setBackgroundColor(const std::string &hexColor) = 0;

    void* getNativeObject() {
        return m_nativeObject;
    }

    virtual void create() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void maximize() = 0;
    virtual void update() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void move(int x, int y) = 0;
    virtual void addSubView(PlatformView* subView) = 0;

    [[nodiscard]] const MNRect& geometry() const;

    void setNativeContext(void* context) {
        m_nativeContext = context;
    }

    void* getNativeContext() {
        return m_nativeContext;
    }

    void setGraphicsContext(void* context) {
        m_graphicsContext = context;
    }

    void* getGraphicsContext() {
        return m_graphicsContext;
    }

    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
};

