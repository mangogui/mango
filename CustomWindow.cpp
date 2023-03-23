#include "CustomWindow.h"
#include "platforms/cocoa/Painter.h"
#include "Rect.h"
#include "Color.h"

namespace GUI {
    void CustomWindow::paintEvent() {
        Painter painter(this);
        painter.setFillColor(Color(102, 10, 102));
        painter.drawEllipse(Rect(100, 100, 200, 200));
        painter.setFillColor(Color(10, 255, 102));
        painter.drawRectangle(Rect(400, 400, 200, 200));
    }
}
