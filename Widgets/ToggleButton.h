#pragma once

#include "../platforms/cocoa/Widget.h"


namespace GUI {
    class ToggleButton : public Widget {
    public:
        explicit ToggleButton(Widget *parent = nullptr): Widget(parent) {
            toggle_indicator_x_pos = size().get_height()*.1;
        }

        void mousePressEvent() override;
        void paintEvent(const PaintEvent& event) override;

    protected:
        bool toggle_state = false;
        float toggle_indicator_x_pos;
    };
}