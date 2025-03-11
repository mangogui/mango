#include <Application.h>
#include <Widget.h>
#include <BoxLayout.h>

#include "Switch.h"


int main(int argc, char* argv[]) {
    Application &application = Application::instance();

    auto *outerWidget = new Widget();
    outerWidget->setObjectName("Window");
    outerWidget->resize(600, 600);
    outerWidget->move(400, 400);
    outerWidget->setBackgroundColor("#ffffff");

    auto *widget = new Widget(outerWidget);
    widget->setObjectName("Switch");
    widget->move(0, 0);
    widget->resize(600, 300);
    widget->setBackgroundColor("#333333");

    auto *widget2 = new Widget(outerWidget);
    widget2->setObjectName("Switch");
    widget2->move(0, 300);
    widget2->resize(600, 300);
    widget2->setBackgroundColor("#330011");

    auto *layout = new BoxLayout(outerWidget, BoxLayout::Orientation::HORIZONTAL);
    outerWidget->setLayout(layout);
    layout->addWidget(widget);
    layout->addWidget(widget2);

    outerWidget->display();

    application.run();

    return 0;
}

