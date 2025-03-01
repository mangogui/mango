#include "AnimationScheduler.h"
#include <AnimationRegistry.h>



void AnimationScheduler::updateAnimations() {
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    // Initialize lastUpdate if uninitialized
    if (lastUpdate.QuadPart == 0) {
        lastUpdate = now;
    }

    // Calculate time difference (delta time in seconds)
    // Convert the counter difference to seconds (since frequency is counts per second)
    double deltaTime = static_cast<double>(now.QuadPart - lastUpdate.QuadPart) / frequency.QuadPart;
    lastUpdate = now;

    // You can convert deltaTime to milliseconds by multiplying by 1000 if needed
    deltaTime *= 1000;

    // Update the animations using the deltaTime
    auto animations = AnimationRegistry::instance().getActiveAnimations();
    AnimationRegistry::instance().removeFinishedAnimation();
    AnimationRegistry::instance().syncAnimations(deltaTime);
}

AnimationScheduler &AnimationScheduler::instance() {
    static AnimationScheduler instance;
    return instance;
}

AnimationScheduler::AnimationScheduler() {
    // Initialize the frequency on the first use
    QueryPerformanceFrequency(&frequency);
}