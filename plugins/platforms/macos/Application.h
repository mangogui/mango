#pragma once

#include <memory>
#include <vector>
#include "animations/VariantAnimation.h"

struct CocoaApplicationWrapper;

class Widget;

class Application {
public:
    static Application &instance();

    void addWidget(Widget *widget);

    void addAnimation(const std::shared_ptr<VariantAnimation> &animation);

    // Prevent copying and assignment
    Application(const Application &) = delete;

    Application &operator=(const Application &) = delete;

    void run();

    ~Application();

private:
    Application();

    std::unique_ptr<CocoaApplicationWrapper> wrapper;
    std::vector<Widget *> widgets;
    std::vector<std::weak_ptr<VariantAnimation>> animations{};
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

    static void processEvents();

    void updateAnimations();
};