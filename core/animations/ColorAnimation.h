#pragma once

#include <Color.h>
#include <VariantAnimation.h>

class ColorAnimation : public VariantAnimation {
public:
    using Callback = std::function<void(Color)>;

    ColorAnimation(const Color &start, const Color &end, int duration, ColorAnimation::Callback updateCallback) :
            VariantAnimation(Variant(start), Variant(end), duration, std::move(updateCallback)) {}

    ~ColorAnimation() override = default;

protected:
    void updateCurrentTime(int msecs) override;
};