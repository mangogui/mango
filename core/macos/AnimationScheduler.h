#pragma once
#include <chrono>

class AnimationScheduler {
public:
    static AnimationScheduler &instance();
    AnimationScheduler(const AnimationScheduler &) = delete;
    void updateAnimations();
private:
    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
    AnimationScheduler() = default;
};