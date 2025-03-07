#pragma once

#include <PlatformWindow.h>
#include <Direct2DGraphicsContext.h>
#include <Windows.h>

class Widget;

class Win32Window: public PlatformWindow {
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC hdc;
public:
    explicit Win32Window(Widget* widget = nullptr);
    ~Win32Window() override;

    // Setters
    void setTitle(const std::string &title) override;
    void setBackgroundColor(const std::string &hexColor) override;

    void* nativeObject() override;

    void create() override;
    void show() override;
    void hide() override;
    void maximize() override;
    void resize(int w, int h) override;
    void move(int x, int y) override;
    void update() override;
    void addSubView(PlatformView* subView) override;
};
