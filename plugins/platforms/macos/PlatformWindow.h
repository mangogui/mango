#pragma once
#include <string>


class PlatformWindow {
public:
    virtual ~PlatformWindow() = default;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void resize(int width, int height) = 0;
    virtual void move(int x, int y) = 0;
    virtual void setTitle(const std::string &title) = 0;
    virtual void setBackgroundColor(const std::string &hexColor) = 0;
    virtual void* getNativeWindow() = 0;
};

