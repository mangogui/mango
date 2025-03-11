#pragma once

#include <string>
#include <MNRect.h>
#include <Object.h>
#include <Event.h>

#include <ResizeEvent.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <GraphicsContext.h>
#include <Color.h>

class AbstractWidget;
class PlatformView;


class PlatformWindow: public Object {
protected:
    AbstractWidget* m_widget;
    void* m_nativeContext;
public:
    explicit PlatformWindow(AbstractWidget *widget = nullptr): Object(), m_widget(widget) {};

    virtual ~PlatformWindow() = default;

    // Setters
    virtual void setTitle(const std::string &title) = 0;
    virtual void setBackgroundColor(const std::string &hexColor) = 0;
    virtual void setBackgroundColor(const Color& color) = 0;

    virtual void* nativeObject() = 0;

    virtual void create() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void maximize() = 0;
    virtual void update() = 0;

    virtual void addSubView(PlatformView* subView) = 0;

    [[nodiscard]] const MNRect& geometry() const;

    void setNativeContext(void* context) {
        m_nativeContext = context;
    }

    void* getNativeContext() {
        return m_nativeContext;
    }

    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;

    virtual void resize(int w, int h) = 0;
    virtual void move(int x, int y) = 0;
};

