#pragma once
#include <VariantAnimation.h>


class AnimationRegistry {
public:
    static AnimationRegistry &instance();
    AnimationRegistry(const AnimationRegistry &) = delete;
    const std::vector<std::weak_ptr<VariantAnimation>>& getActiveAnimations();
    void addAnimation(const std::shared_ptr<VariantAnimation> &animation);
    void removeFinishedAnimation();
    void syncAnimations(int msec);
private:
    AnimationRegistry() = default;
    std::vector<std::weak_ptr<VariantAnimation>> animations{};
};

