#include "AnimationScheduler.h"
#include <AnimationRegistry.h>

// TODO: Optimise this function
void AnimationScheduler::updateAnimations() {
    using Clock = std::chrono::steady_clock;

    auto now = Clock::now();

    // Initialize lastUpdate if uninitialized
    if (lastUpdate == Clock::time_point{}) {
        lastUpdate = now;
    }

    // Calculate time difference (delta time in milliseconds)
    auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count();
    lastUpdate = now;

    auto animations = AnimationRegistry::instance().getActiveAnimations();

    // Remove expired or non-running animations and update active ones
    AnimationRegistry::instance().removeFinishedAnimation();
    AnimationRegistry::instance().syncAnimations(deltaTime);
}

AnimationScheduler &AnimationScheduler::instance() {
    static AnimationScheduler instance;
    return instance;
}
