#pragma once

#include <string>
#include <utility>
#include <memory>

#include <MNSize.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <ResizeEvent.h>
#include <CoreGraphicsContext.h>
#include <PlatformWindow.h>
#include <PlatformView.h>


class Widget {
    std::unique_ptr<PlatformWindow> window;
    std::unique_ptr<PlatformView> view;
public:
    explicit Widget(Widget *parent = nullptr);

    PlatformView *getView() {
        return view.get();
    }

    PlatformWindow *getWindow() {
        return window.get();
    }

    ~Widget();

    void move(int x, int y);

    void resize(int width, int height);

    virtual void mousePressEvent(MouseEvent *event);
    virtual void mouseReleaseEvent(MouseEvent *event);
    virtual void resizeEvent(ResizeEvent *event);
    virtual void paintEvent(PaintEvent *event);

    void maximize();

    void fullscreen();

    void update();

    // Setters
    void setWindowTitle(const std::string &title);

    void setBackgroundColor(const std::string &hexColor);

    void setObjectName(const std::string &obj_name);

    // Getters
    [[nodiscard]] std::string windowTitle() const;

    [[nodiscard]] float scaleFactor() const;

    [[nodiscard]] MNRect rect() const;

    [[nodiscard]] int width() const;

    [[nodiscard]] int height() const;

    [[nodiscard]] int x() const;

    [[nodiscard]] int y() const;

    [[nodiscard]] const std::string &objectName() const { return object_name; }

    [[nodiscard]] void *getNSViewId() const { return view.get(); }

    [[nodiscard]] MNSize size() const;

    CoreGraphicsContext* getGraphicsContext() {
        return graphics.get();
    }

    void setCGContextRef(CGContextRef _context) {
        context = _context;
        graphics->setCGContextRef(_context);
    }

    void display();

protected:
    CGContextRef context;
    std::unique_ptr<CoreGraphicsContext> graphics;
    Widget *parent;
private:
    std::string object_name;
    std::string m_windowTitle;
    MNRect m_geometry;
};

