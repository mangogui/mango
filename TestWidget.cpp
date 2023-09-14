#include "TestWidget.h"
#include "platforms/cocoa/Painter.h"


namespace GUI {
    void TestWidget::paintEvent(const GUI::PaintEvent &event) {
        Painter painter(this);
        painter.setFillColor(Color(200, 117, 100));
        painter.drawRect(event.rect());
    }
}
