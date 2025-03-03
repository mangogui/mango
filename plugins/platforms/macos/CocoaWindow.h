#pragma once

#include <PlatformWindow.h>

#include <Cocoa/Cocoa.h>

class CocoaWindow : public PlatformWindow {
private:
    NSWindow *window;
public:
    CocoaWindow();
    ~CocoaWindow() override;
    void show() override;
    void hide() override;
    void resize(int width, int height) override;
    void move(int x, int y) override;
    void setTitle(const std::string &title) override;
    void setBackgroundColor(const std::string &hexColor) override;
    void* getNativeWindow() override;
};
