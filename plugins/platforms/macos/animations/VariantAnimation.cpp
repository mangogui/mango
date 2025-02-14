#include "VariantAnimation.h"
#include "Application.h"

VariantAnimation::VariantAnimation(const Variant &start, const Variant &end, int duration,
                                   VariantAnimation::Callback updateCallback) :
        startValue(start),
        endValue(end),
        duration(duration),
        running(false),
        m_currentTime(0),
        updateCallback(std::move(updateCallback)) {
}

VariantAnimation::~VariantAnimation() {
    stop();
}

void VariantAnimation::start() {
    Application::instance().addAnimation(this->getSharedPtr());
    running = true;
    startTime = 0;
}

void VariantAnimation::stop() {
    running = false;
}

void VariantAnimation::restart() {
    stop();
    start();
}