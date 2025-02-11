#pragma once

#include <functional>
#include <chrono>
#include <iostream>
#include <memory>

#include "EasingCurve.h"
#include "VariantAnimation.h"


class NumberAnimation: public VariantAnimation {
public:
    using Callback = std::function<void(int)>;
    NumberAnimation(const int &start, const int &end, int duration, NumberAnimation::Callback updateCallback):
            VariantAnimation(Variant(start), Variant(end), duration, std::move(updateCallback)) {}
    ~NumberAnimation() override = default;
protected:
    void updateCurrentTime(int msecs) override;
};
