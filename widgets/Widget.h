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
#include <AbstractWidget.h>
#include <Layout.h>
#include <Color.h>


class Widget: public AbstractWidget, public Object {
    PlatformWindow* m_window;
    PlatformView* m_view;
    Layout* m_layout;
    GraphicsContext* m_graphicsContext;
public:
    explicit Widget(Widget *parent = nullptr);
    ~Widget();

    void addChild(Widget* child);
    void setParent(Widget* parent);

    void center() override;
    void move(int x, int y) override;
    void resize(int width, int height) override;
    void maximize() override;
    void fullscreen() override;
    void display() override;
    void update() override;
    void create() override;

    // Setters
    void setWindowTitle(const std::string& title) override;
    void setBackgroundColor(const std::string &hexColor) override;
    void setNativeContext(void* context) override;

    // Getters
    [[nodiscard]] int x() const override;
    [[nodiscard]] int y() const override;
    [[nodiscard]] int width() const override;
    [[nodiscard]] int height() const override;
    [[nodiscard]] MNRect rect() const override;
    [[nodiscard]] std::string getWindowTitle() const override;
    [[nodiscard]] const MNRect& geometry() override;
    [[nodiscard]] MNSize size() const override;
    [[nodiscard]] bool isCreated() const override { return m_isCreated; }
    [[nodiscard]] const Color& backgroundColor() const override { return m_backgroundColor; }

    // Events
    void handleEvent(Event *event) override;
    void mousePressEvent(MouseEvent *event) override;
    void mouseReleaseEvent(MouseEvent *event) override;
    void resizeEvent(ResizeEvent *event) override;
    void paintEvent(PaintEvent *event) override;

    [[nodiscard]] void* getWinId() const override {
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

    [[nodiscard]] bool isTopLevel() const override {
        return m_isTopLevel;
    }

    void setWindow(PlatformWindow* win) {
        m_window = win;
    }

    void setGraphicsContext(GraphicsContext* context) override {
        m_graphicsContext = context;
    }

    GraphicsContext* getGraphicsContext() override {
        return m_graphicsContext;
    }

    void setLayout(Layout* layout) override {
        m_layout = layout;
    }

    [[nodiscard]] Layout* layout() override {
        return m_layout;
    }

    const std::vector<Widget*>& children() const {
        return m_children;
    };

private:
    Widget *m_parent;
    std::string m_windowTitle;
    std::vector<Widget*> m_children;
    bool m_isCreated;
    bool m_isTopLevel;
    MNRect m_geometry;
    Color m_backgroundColor;
};

