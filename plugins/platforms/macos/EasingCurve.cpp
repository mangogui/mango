#include "EasingCurve.h"
#include <cmath>


float EasingCurve::valueForProgress(float progress) {
    float t = progress;
    switch (easingType) {
        case EasingType::Linear:
            return t;
        case EasingType::EaseInQuad:
            return t * t;
        case EasingType::EaseOutQuad:
            return 1 - (1 - t) * (1 - t);
        case EasingType::EaseInOutQuad:
            return (t < 0.5f) ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
        case EasingType::EaseInCubic:
            return t * t * t;
        case EasingType::EaseOutCubic:
            return 1.0 - pow(1 - t, 3);
        case EasingType::EaseInOutCubic:
            return (t < 0.5f) ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
        default:
            return t;
    }
}
