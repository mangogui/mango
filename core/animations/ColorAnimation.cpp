#include "ColorAnimation.h"


void ColorAnimation::updateCurrentTime(int msecs) {
    const Color &startColor = startValue.getColor();
    const Color &endColor = endValue.getColor();
    int newRed = startColor.red() + (endColor.red() - startColor.red()) * m_easingCurve.valueForProgress(m_progress);
    int newGreen =
            startColor.green() + (endColor.green() - startColor.green()) * m_easingCurve.valueForProgress(m_progress);
    int newBlue =
            startColor.blue() + (endColor.blue() - startColor.blue()) * m_easingCurve.valueForProgress(m_progress);
    int newAlpha =
            startColor.alpha() + (endColor.alpha() - startColor.alpha()) * m_easingCurve.valueForProgress(m_progress);
    updateCallback(Color(newRed, newGreen, newBlue, newAlpha));
}