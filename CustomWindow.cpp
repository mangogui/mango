#include "CustomWindow.h"
#include "platforms/cocoa/Painter.h"
#include "Rect.h"
#include "Color.h"

namespace GUI {
    void CustomWindow::paintEvent() {
        Painter painter(this);
        painter.setFillColor(Color(89, 77, 157));
        painter.drawRoundedRect(Rect(100, 400, 800, 800), 200, 200);
        painter.setFillColor(Color(143, 117, 255));
        painter.drawEllipse(Rect(400, 400, 1200, 800));
    }
}
