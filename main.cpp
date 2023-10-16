
#include "Mango.h"
#include "CustomWidget.h"
#include "TestWidget.h"
#include "Widgets/ToggleButton.h"


int main() {
    Application application;
    CustomWidget widget;
//    ToggleButton toggle_button(&widget);
//    toggle_button.setObjectName("TestWidget");
    widget.resize(600, 600);
    widget.center();
    widget.display();
//    toggle_button.move(200, 200);
//    toggle_button.resize(200, 100);
    application.run();

    return 0;
}