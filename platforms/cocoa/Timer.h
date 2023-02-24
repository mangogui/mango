//
// Created by Parham Oyan on 23/01/2023.
//

#ifndef MANGO_TIMER_H
#define MANGO_TIMER_H

#include <functional>


namespace GUI {
    struct NSTimerWrapper;

    class Timer {
    public:
        Timer();

        ~Timer();

        void start();
        void stop();
        void setCallback(std::function<void()> callback);
        void timerCallback();

    private:
        NSTimerWrapper *timer_wrapper;
        std::function<void()>* callBack = nullptr;
    };
}
#endif //MANGO_TIMER_H
