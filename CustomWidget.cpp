#include "CustomWidget.h"
#include "platforms/cocoa/Painter.h"
#include "Rect.h"
#include "Color.h"


namespace GUI {
    void CustomWidget::paintEvent(const PaintEvent& event) {
        Painter painter(this);
        painter.setFillColor(Color(143, 117, 100));
        painter.drawRoundedRect(Rect(400, 400, 400, 400), 100, 100);
//        painter.drawEllipse(Rect(800, 400, 400, 400));
//        painter.drawRect(Rect(400, 400, 400, 400));
//        painter.moveTo(Point(830, 540));
//        painter.drawCubicBezierCurve(Point(510, 722), Point(30, 1550), Point(674, 1774));
//        painter.drawCubicBezierCurve(Point(1377, 2056), Point(2638, 1611), Point(2199, 1086));
//        painter.drawLine(Point(468, 341), Point(220, 752));
//        painter.drawLine(Point(220, 752), Point(580, 1100));
//        painter.drawLine(Point(580, 1100), Point(944, 988));

    }

    CustomWidget::CustomWidget(Widget* parent): Widget(parent) {
        setObjectName("CustomWidget");
    }
}
