#pragma once

#include <utility>
#include <vector>

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <Windows.h>

#include <memory>
#include <string>
#include <iostream>
#include <cassert>

#include <MNSize.h>
#include <Event.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <ResizeEvent.h>
#include <PlatformWindow.h>
#include <Object.h>
#include <memory>

#include <Direct2DGraphicsContext.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM uParam, LPARAM lParam);

class Widget: public Object {
    std::unique_ptr<PlatformWindow> window;
public:
    explicit Widget(Widget *parent = nullptr);

    void setWindowTitle(const std::string& title);
    void setBackgroundColor(const std::string &hexColor);

    void center();
    void move(int x, int y);
    void resize(int width, int height);
    void maximize();
    void fullscreen();
    void display();

    // Events
    virtual void handleEvent(Event *event);
    virtual void mousePressEvent(MouseEvent *event);
    virtual void mouseReleaseEvent(MouseEvent *event);
    virtual void resizeEvent(ResizeEvent *event);
    virtual void paintEvent(PaintEvent *event);

    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] MNSize size() const;
    [[nodiscard]] MNRect rect() const;
    [[nodiscard]] std::string getWindowTitle() const;
    [[nodiscard]] HWND getWinId() const { return static_cast<HWND>(window->getNativeWindow()); }

    void addChild(Widget* child);
    void setParent(Widget* parent);

    Direct2DGraphicsContext* getGraphicsContext() {
        Direct2DGraphicsContext* _c = static_cast<Direct2DGraphicsContext*>(window->getGraphicsContext());
        return _c;
    }

    PlatformWindow* getWindow() {
        return window.get();
    }

    void update();

    ~Widget();
private:
    std::string window_title;
    Widget *m_parent;
    std::string m_windowTitle;
    std::vector<Widget*> m_children;
};

