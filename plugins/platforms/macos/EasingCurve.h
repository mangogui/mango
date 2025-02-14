#pragma once

class EasingCurve {
public:
    enum class EasingType {
        Linear,
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic
    };

    explicit EasingCurve(EasingCurve::EasingType type = EasingCurve::EasingType::Linear) : easingType(type) {};

    virtual ~EasingCurve() = default;

    virtual float valueForProgress(float progress);

private:
    EasingType easingType;
};


