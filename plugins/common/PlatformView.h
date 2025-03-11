#pragma once

#include <Object.h>

class AbstractWidget;

class PlatformView: public Object {
protected:
    AbstractWidget* m_widget;
public:
    explicit PlatformView(AbstractWidget *widget): Object(), m_widget(widget) {}
    virtual ~PlatformView() = default;

    virtual void setBackgroundColor(const std::string &hexColor) = 0;

    virtual void update() = 0;
    virtual void create() = 0;
    virtual void setFrame(int x, int y, int width, int height) = 0;
    virtual void move(int x, int y) = 0;
    virtual void show() = 0;
    virtual void* nativeObject() = 0;
    virtual void resize(int w, int h) = 0;
    virtual void addSubView(PlatformView* subView) = 0;

    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
};