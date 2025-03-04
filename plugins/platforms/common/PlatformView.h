#pragma once

class PlatformView {
public:
    virtual ~PlatformView() = default;
    virtual void update() = 0;
    virtual void setFrame(int x, int y, int width, int height) = 0;
    virtual void move(int x, int y) = 0;
    virtual void* getNativeView() = 0;
    virtual void resize(int width, int height) = 0;
};