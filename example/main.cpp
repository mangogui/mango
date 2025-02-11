#include "Application.h"
#include "Widget.h"
#include "Switch.h"


int main() {
    Application& application = Application::instance();

    auto* outerWidget = new Widget();
    auto* toggleSwitch = new Switch(outerWidget);

    outerWidget->resize(600, 600);
    outerWidget->move(0, 0);
    outerWidget->setBackgroundColor("#ffffff");

    toggleSwitch->move(200, 230);

    application.run();

    return 0;
}

