#include <Application.h>
#include <Widget.h>
#include "Switch.h"


int main(int argc, char* argv[]) {
    Application &application = Application::instance();

    auto *outerWidget = new Widget();
    outerWidget->setObjectName("Window");

    auto *toggleSwitch = new Switch(outerWidget);
    toggleSwitch->setObjectName("Switch");

    outerWidget->resize(600, 600);
    outerWidget->move(400, 400);
    outerWidget->setBackgroundColor("#ffffff");

    toggleSwitch->move(200, 230);

    outerWidget->display();

    application.run();

    return 0;
}

