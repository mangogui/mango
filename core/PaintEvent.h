#pragma once

#include "MNRect.h"
#include <Event.h>


class PaintEvent: public Event {
public:
    PaintEvent(): Event(Event::Type::PaintEvent) {}

    void setRect(const MNRect &r) {
        this->_rect = r;
    }

    [[nodiscard]] const MNRect &rect() const { return _rect; }

private:
    MNRect _rect;
};


