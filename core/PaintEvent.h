#pragma once

#include "MNRect.h"


class PaintEvent {
public:
    PaintEvent() = default;

    void setRect(const MNRect &r) {
        this->_rect = r;
    }

    [[nodiscard]] const MNRect &rect() const { return _rect; }

private:
    MNRect _rect;
};


