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

#include "Direct2DGraphicsContext.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM uParam, LPARAM lParam);

class Object {
public:
    std::string objectName() { return m_objectName; };

    void setObjectName(std::string objectName) {
        m_objectName = std::move(objectName);
    }
private:
    std::string m_objectName;
};

class Widget: public Object {
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
    [[nodiscard]] HWND getWinId() const { return m_hWnd; }

    void addChild(Widget* child);
    void setParent(Widget* parent);

    Direct2DGraphicsContext* getGraphicsContext() {
        return graphics.get();
    }

    void update();

    ~Widget();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC hdc;
    std::string window_title;
    Widget *m_parent;
    MNRect geometry;

    std::vector<Widget*> m_children;

    std::unique_ptr<Direct2DGraphicsContext> graphics;

    void createWindow();
};

