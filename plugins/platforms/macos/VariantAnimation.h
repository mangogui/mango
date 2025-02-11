#pragma once

#include <functional>
#include <chrono>
#include "EasingCurve.h"
#include "Variant.h"

class VariantAnimation : public std::enable_shared_from_this<VariantAnimation>  {
public:
    using Callback = std::function<void(Variant)>;

    VariantAnimation(const Variant& start, const Variant& end, int duration, Callback updateCallback);
    virtual ~VariantAnimation();

    void start();
    void stop();
    void restart();

    // Setters
    void setStartValue(const Variant& value) { startValue = value; }
    void setEndValue(const Variant& value) { endValue = value; }
    void setDuration(int value) { duration = value; }

    void setCurrentTime(int msecs) {
        m_currentTime = msecs;
        updateProgress();
        updateCurrentTime(m_currentTime);
    }

    // Getters
    [[nodiscard]] Variant getStartValue() const { return startValue; }
    [[nodiscard]] Variant getEndValue() const { return endValue; }
    [[nodiscard]] int getDuration() const { return duration; }
    [[nodiscard]] bool isRunning() const { return running; }
    [[nodiscard]] int getCurrentTime() const { return m_currentTime; }
    [[nodiscard]] float getProgress() const { return m_progress; }
    std::shared_ptr<VariantAnimation> getSharedPtr() { return shared_from_this(); }

protected:
    EasingCurve m_easingCurve;
    Variant startValue, endValue;
    int duration;
    float m_progress;
    bool running;
    Callback updateCallback;
    float startTime;
    int m_currentTime;

    virtual void updateCurrentTime(int msecs) = 0;
private:
    void updateProgress() {
        float progress = (float)m_currentTime / (float)duration;
        if (progress > 1) {
            progress = 1;
            running = false;
        }
        m_progress = progress;
    }
};