#include "ToggleButton.h"
#include "../platforms/cocoa/Painter.h"


namespace GUI {
    void ToggleButton::mousePressEvent() {
        toggle_state = !toggle_state;
        update();
    }

    void ToggleButton::paintEvent(const PaintEvent &event) {
        Painter painter(this);
        painter.setFillColor(Color(65, 65, 65));

        const Rect& b_rect = event.rect(); // bounding rect
        int r = b_rect.height()/2;
        painter.drawRoundedRect(b_rect, r, r);

        int y = b_rect.height() * .1;
        r = b_rect.height() * .8;
        int x = toggle_state ? b_rect.width() - r - y: b_rect.height() * .1;
        painter.setFillColor(Color(202, 202, 202));
        painter.drawEllipse(Rect(x, y, r, r));
    }
}