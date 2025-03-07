#pragma once

#include <PlatformView.h>
#include <Windows.h>

class Widget;

class Win32View: public PlatformView {
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    HDC hdc;
public:
    explicit Win32View(Widget *widget = nullptr);
    ~Win32View() override;

    void update() override;
    void create() override;
    void setFrame(int x, int y, int width, int height) override;
    void move(int x, int y) override;
    void show() override;
    void* nativeObject() override;
    void resize(int width, int height) override;
    void addSubView(PlatformView* subView) override;
};
