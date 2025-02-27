#pragma once

#include <utility>
#include <vector>

#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <Windows.h>

#include <string>
#include <iostream>
#include <cassert>

#include <MNSize.h>
#include <Event.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <ResizeEvent.h>


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

    // Getters for Direct2D resources
    ID2D1Factory* getD2DFactory() { return m_pD2DFactory; }
    ID2D1HwndRenderTarget* getRenderTarget() { return m_pRenderTarget; }
    ID2D1BitmapRenderTarget* getBackBuffer() { return m_pBackBuffer; }
    ID2D1SolidColorBrush* getBrush() { return m_pBrush; }

    // Getters for DirectWrite resources
    IDWriteFactory* getDWriteFactory() { return m_pDWriteFactory; }
    IDWriteTextFormat* getTextFormat() { return m_pTextFormat; }

    // Setters for Direct2D resources
    void setD2DFactory(ID2D1Factory* factory) { m_pD2DFactory = factory; }
    void setRenderTarget(ID2D1HwndRenderTarget* renderTarget) { m_pRenderTarget = renderTarget; }
    void setBackBuffer(ID2D1BitmapRenderTarget* backBuffer) { m_pBackBuffer = backBuffer; }
    void setBrush(ID2D1SolidColorBrush* brush) { m_pBrush = brush; }

    // Setters for DirectWrite resources
    void setDWriteFactory(IDWriteFactory* writeFactory) { m_pDWriteFactory = writeFactory; }
    void setTextFormat(IDWriteTextFormat* textFormat) { m_pTextFormat = textFormat; }

    void resizeRenderTarget(int width, int height);

    void addChild(Widget* child);
    void setParent(Widget* parent);

    void update();

    ~Widget();
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC hdc;
    std::string window_title;
    Widget *m_parent;
    MNRect geometry;

    ID2D1Factory *m_pD2DFactory = nullptr;
    ID2D1HwndRenderTarget *m_pRenderTarget = nullptr;
    ID2D1BitmapRenderTarget *m_pBackBuffer = nullptr;
    ID2D1SolidColorBrush *m_pBrush = nullptr;
    IDWriteFactory *m_pDWriteFactory = nullptr;
    IDWriteTextFormat *m_pTextFormat = nullptr;

    std::vector<Widget*> m_children;


    void createWindow();
};

