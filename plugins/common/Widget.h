#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <memory>

#include <MNSize.h>
#include <Event.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <ResizeEvent.h>
#include <PlatformWindow.h>
#include <PlatformView.h>
#include <Object.h>


class Widget: public Object {
    PlatformWindow* m_window;
    PlatformView* m_view;
    void* m_graphicsContext;
public:
    explicit Widget(Widget *parent = nullptr);
    ~Widget();

    void addChild(Widget* child);
    void setParent(Widget* parent);

    void center();
    void move(int x, int y);
    void resize(int width, int height);
    void maximize();
    void fullscreen();
    void display();
    void update();
    void create();

    // Setters
    void setWindowTitle(const std::string& title);
    void setBackgroundColor(const std::string &hexColor);
    void setNativeContext(void* context);

    // Getters
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] MNRect rect() const;
    [[nodiscard]] std::string getWindowTitle() const;
    [[nodiscard]] const MNRect& geometry();
    [[nodiscard]] MNSize size() const;
    [[nodiscard]] bool isCreated() const { return m_isCreated; }

    // Events
    virtual void handleEvent(Event *event);
    virtual void mousePressEvent(MouseEvent *event);
    virtual void mouseReleaseEvent(MouseEvent *event);
    virtual void resizeEvent(ResizeEvent *event);
    virtual void paintEvent(PaintEvent *event);

    [[nodiscard]] Widget* parent() {
        return m_parent;
    }

    [[nodiscard]] void* getWinId() const {
        if (isTopLevel())
            return m_window->nativeObject();
        else
            return m_view->nativeObject();
    }

    [[nodiscard]] PlatformWindow* window() const {
        return m_window;
    }

    [[nodiscard]] PlatformView* view() const {
        return m_view;
    }

    [[nodiscard]] bool isTopLevel() const {
        return m_isTopLevel;
    }

    void setWindow(PlatformWindow* win) {
        m_window = win;
    }

    void setGraphicsContext(void* context) {
        m_graphicsContext = context;
    }

    void* getGraphicsContext() {
        return m_graphicsContext;
    }

private:
    Widget *m_parent;
    std::string m_windowTitle;
    std::vector<Widget*> m_children;
    bool m_isCreated;
    bool m_isTopLevel;
    MNRect m_geometry;
};

