#ifndef MANGO_CUSTOMWINDOW_H
#define MANGO_CUSTOMWINDOW_H

#include "platforms/cocoa/CocoaWindow.h"

namespace GUI {
    class CustomWindow : public CocoaWindow {
    public:
        void paintEvent() override;
    };
}


#endif //MANGO_CUSTOMWINDOW_H
