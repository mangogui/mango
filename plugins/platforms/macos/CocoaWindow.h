#pragma once

#include <PlatformWindow.h>


class CocoaWindow : public PlatformWindow {
    void* m_nativeObject;
public:
    explicit CocoaWindow(AbstractWidget *widget = nullptr);
    ~CocoaWindow() override;

    // Setters
    void setTitle(const std::string &title) override;
    void setBackgroundColor(const std::string &hexColor) override;

    void create() override;
    void show() override;
    void hide() override;
    void maximize() override;
    void resize(int w, int h) override;
    void move(int x, int y) override;
    void update() override;

    void addSubView(PlatformView* subView) override;

    void* nativeObject() override;
};
