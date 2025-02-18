#pragma once

#include <memory>
#include <vector>
#include <VariantAnimation.h>

struct CocoaApplicationWrapper;

class Widget;

class Application {
public:
    static Application &instance();

    void addWidget(Widget *widget);

    // Prevent copying and assignment
    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void run();

    ~Application();

private:
    Application();

    std::unique_ptr<CocoaApplicationWrapper> wrapper;
    std::vector<Widget *> widgets;

    static void processEvents();
};