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
#include <Object.h>

#ifdef _WIN64
    #include <Direct2DGraphicsContext.h>
    #include <Windows.h>
    #pragma comment(lib, "d2d1.lib")
    #pragma comment(lib, "dwrite.lib")
    #include <d2d1.h>
    #include <dwrite.h>
#elif __APPLE__
    #include <CoreGraphicsContext.h>
#endif


class Widget: public Object {
    std::unique_ptr<PlatformWindow> window;
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

    // Setters
    void setWindowTitle(const std::string& title);
    void setBackgroundColor(const std::string &hexColor);

    // Getters
    GraphicsContext* getGraphicsContext();
    PlatformWindow* getWindow();
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] MNSize size() const;
    [[nodiscard]] MNRect rect() const;
    [[nodiscard]] std::string getWindowTitle() const;

    // Events
    virtual void handleEvent(Event *event);
    virtual void mousePressEvent(MouseEvent *event);
    virtual void mouseReleaseEvent(MouseEvent *event);
    virtual void resizeEvent(ResizeEvent *event);
    virtual void paintEvent(PaintEvent *event);

#ifdef _WIN32
    [[nodiscard]] HWND getWinId() const { return static_cast<HWND>(window->getNativeWindow()); }
#endif

private:
    Widget *m_parent;
    std::string m_windowTitle;
    std::vector<Widget*> m_children;
};

