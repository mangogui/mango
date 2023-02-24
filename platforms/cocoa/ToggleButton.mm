//
// Created by Parham Oyan on 18/01/2023.
//

#include "ToggleButton.h"
#import <QuartzCore/QuartzCore.h>
#import "Cocoa/Cocoa.h"
#include "Painter.h"


namespace GUI {
    void ToggleButton::mousePressEvent() {
        toggle_state = !toggle_state;
        auto _view = (NSView*)getView();

        int left = size().get_height()*.1;
        int right = size().get_width() - size().get_height()*.9;

        timer = new Timer();
        timer->setCallback([this](){ this->updatePosition(); });
        timer->start();

        update();
    }

    void ToggleButton::updatePosition() {
        toggle_indicator_x_pos += 0.1;
        update();
        if (toggle_indicator_x_pos > size().get_width() - size().get_height()*.9) {
            timer->stop();
        }
    }

    void ToggleButton::resizeEvent() {

    }

    void ToggleButton::paintEvent() {
        Painter painter;
        painter.setColor(Color(65, 65, 65));
        painter.drawRoundedRectangle(Rect(0, 0, size().get_width(), size().get_height()), 100/2, 100/2);
        painter.setColor(Color(202, 202, 202));

        int y = size().get_height()*.1;
        int r = size().get_height()*.8;
        painter.drawEllipse(Rect(toggle_indicator_x_pos, y, r, r));
    }

}