#pragma once

#include "plugins/platforms/common/PlatformView.h"
#include <Widget.h>
#include <Cocoa/Cocoa.h>

class CocoaView : public PlatformView {
private:
    NSView *view;
public:
    explicit CocoaView(NSView *parentView, Widget *widget);
    explicit CocoaView(NSWindow *parentWindow, Widget *widget);
    ~CocoaView() override;

    void update() override;
    void setFrame(int x, int y, int width, int height) override;
    void resize(int width, int height) override;
    void move(int x, int y) override;
    void * getNativeView() override;
};

