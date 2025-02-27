#include "AnimationRegistry.h"
#include <algorithm>

void AnimationRegistry::addAnimation(const std::shared_ptr<VariantAnimation> &animation) {
    if (!animation) return;
    animations.push_back(animation);
}

AnimationRegistry &AnimationRegistry::instance() {
    static AnimationRegistry instance;
    return instance;
}

const std::vector<std::weak_ptr<VariantAnimation>>& AnimationRegistry::getActiveAnimations() {
    return animations;
}

void AnimationRegistry::removeFinishedAnimation() {
    animations.erase(std::remove_if(animations.begin(), animations.end(),
                                    [](std::weak_ptr<VariantAnimation> &weakAnim) {
                                        if (auto anim = weakAnim.lock()) {  // Lock weak_ptr to check validity
                                            return !anim->isRunning();  // Remove if animation is not running
                                        }
                                        return true;  // Remove expired weak_ptr
                                    }), animations.end());
}

void AnimationRegistry::syncAnimations(int msec) {
    for (auto &weakAnim : animations) {
        if (auto anim = weakAnim.lock()) {
            anim->setCurrentTime(anim->getCurrentTime() + msec);
        }
    }
}