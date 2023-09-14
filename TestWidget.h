#pragma once
#include "platforms/PaintEvent.h"
#include "platforms/cocoa/Widget.h"


namespace GUI {
    class TestWidget: public Widget {
    public:
        explicit TestWidget(Widget* parent = nullptr): Widget(parent) {}
        void paintEvent(const PaintEvent& event) override;
    };
}

