//
// Created by Parham Oyan on 23/01/2023.
//

#include "Timer.h"
#import "Cocoa/Cocoa.h"

@interface TimerBridge : NSObject {
    GUI::Timer *_timer;
}
- (id)initWithTimer:(GUI::Timer *)timer;

- (void)timerCallback:(NSTimer *)t;
@end

@implementation TimerBridge

- (id)initWithTimer:(GUI::Timer *)timer {
    if (self = [super init]) {
        _timer = timer;
    }
    return self;
}

- (void)timerCallback:(NSTimer *)t {
    _timer->timerCallback();
}

@end

namespace GUI {
    struct NSTimerWrapper {
        NSTimer *wrapped;
    };

    Timer::Timer() : timer_wrapper(new NSTimerWrapper()) {}

    void Timer::start() {
        @autoreleasepool {
            TimerBridge *bridge = [[TimerBridge alloc] initWithTimer:this];
            timer_wrapper->wrapped = [NSTimer scheduledTimerWithTimeInterval:0.001
                                                                      target:bridge
                                                                    selector:@selector(timerCallback:)
                                                                    userInfo:nil
                                                                     repeats:YES];
        }
    }

    void Timer::timerCallback() {
        if (callBack) {
            (*callBack)();
        }
    }

    void Timer::setCallback(std::function<void()> cb) {
        callBack = new std::function<void()>(cb);
    }

    void Timer::stop() {
        [timer_wrapper->wrapped invalidate];
        timer_wrapper->wrapped = nil;
    }

    Timer::~Timer() {
        [timer_wrapper->wrapped release];
    }

}