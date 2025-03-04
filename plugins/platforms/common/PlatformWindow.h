#pragma once

#include <string>
#include <MNRect.h>
#include <Object.h>
#include <Event.h>

#include <ResizeEvent.h>
#include <PaintEvent.h>
#include <MouseEvent.h>


class PlatformWindow: public Object {
    MNRect m_geometry;
public:
    explicit PlatformWindow(PlatformWindow* parent = nullptr): Object(), m_geometry() {};
    virtual ~PlatformWindow() = default;

    // Setters
    virtual void setTitle(const std::string &title) = 0;
    virtual void setBackgroundColor(const std::string &hexColor) = 0;

    // Getters
    virtual void* getNativeWindow() = 0;
    virtual void* getGraphicsContext() = 0;
    [[nodiscard]] const MNRect& geometry() const { return m_geometry; }
    [[nodiscard]] MNSize size() const { return m_geometry.size(); }

    virtual void create() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void maximize() = 0;
    virtual void resize(int w, int h) { m_geometry.resize(w, h); }
    virtual void move(int x, int y) { m_geometry.move(x, y); }
    virtual void update() = 0;

    [[nodiscard]] int x() const { return m_geometry.x(); }
    [[nodiscard]] int y() const { return m_geometry.y(); }
    [[nodiscard]] int width() const { return m_geometry.width(); }
    [[nodiscard]] int height() const { return m_geometry.height(); }
};

