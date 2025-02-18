#include "NumberAnimation.h"


void NumberAnimation::updateCurrentTime(int msecs) {
    int newValue = startValue.getInt() +
                   (endValue.getInt() - startValue.getInt()) * m_easingCurve.valueForProgress(m_progress);
    updateCallback(newValue);
}
