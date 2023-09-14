#pragma once

#include "platforms/cocoa/Widget.h"
#include "TestWidget.h"

namespace GUI {
    class CustomWidget: public Widget {
    public:
        explicit CustomWidget(Widget* parent = nullptr);
        void paintEvent(const PaintEvent& event) override;
    private:
        TestWidget testWidget;
    };
}
