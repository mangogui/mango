#include "CustomWidget.h"
#include "platforms/cocoa/Painter.h"
#include "Rect.h"
#include "Color.h"


namespace GUI {
    void CustomWidget::paintEvent(const PaintEvent& event) {
        Painter painter(this);
        painter.setFillColor(Color(143, 117, 100));
        painter.drawRect(event.rect());
//        painter.drawRoundedRect(Rect(100, 400, 800, 800), 200, 200);
//        painter.setFillColor(Color(143, 117, 255));
//        painter.drawEllipse(Rect(400, 400, 1200, 800));
    }

    CustomWidget::CustomWidget(Widget* parent): Widget(parent) {
        setObjectName("CustomWidget");
    }
}
