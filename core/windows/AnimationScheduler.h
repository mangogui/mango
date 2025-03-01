#pragma once
#include <Windows.h>

class AnimationScheduler {
public:
    static AnimationScheduler &instance();
    AnimationScheduler(const AnimationScheduler &) = delete;
    void updateAnimations();
private:
    LARGE_INTEGER lastUpdate;
    LARGE_INTEGER frequency; // To store the frequency of the high-resolution timer
    AnimationScheduler();
};

