#pragma once

#include <PlatformView.h>
class Widget;

class CocoaView : public PlatformView {
private:
    void* view; // NSView*
public:
    explicit CocoaView(Widget *widget);
    ~CocoaView() override;

    void create() override;
    void show() override;
    void update() override;
    void setFrame(int x, int y, int width, int height) override;
    void resize(int width, int height) override;
    void move(int x, int y) override;
    void * getNativeObject() override;

    void addSubView(PlatformView* subView) override;
};

