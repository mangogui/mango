#pragma once

#include <PlatformView.h>
class AbstractWidget;

class CocoaView : public PlatformView {
private:
    void* view; // NSView*
public:
    explicit CocoaView(AbstractWidget *widget);
    ~CocoaView() override;

    void setBackgroundColor(const std::string &hexColor) override;

    void create() override;
    void show() override;
    void update() override;
    void setFrame(int x, int y, int width, int height) override;
    void resize(int width, int height) override;
    void move(int x, int y) override;
    void* nativeObject() override;

    void addSubView(PlatformView* subView) override;
};

