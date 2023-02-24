//
// Created by Parham Oyan on 18/01/2023.
//

#ifndef MANGO_TOGGLEBUTTON_H
#define MANGO_TOGGLEBUTTON_H

#include "CocoaWindow.h"
#include "Timer.h"


namespace GUI {
    class ToggleButton : public CocoaWindow {
    public:
        explicit ToggleButton(CocoaWindow *parent = nullptr): CocoaWindow(parent) {
            toggle_indicator_x_pos = size().get_height()*.1;
        }

        void mousePressEvent() override;
        void paintEvent() override;
        void resizeEvent() override;

    protected:
        bool toggle_state = false;
        float toggle_indicator_x_pos;
        Timer* timer = nullptr;
        void updatePosition();
    };
}

#endif //MANGO_TOGGLEBUTTON_H
