#include "Application.h"
#include "Widget.h"

int main() {
    Application application;

    auto* outerWidget = new Widget();
    auto* subWidget = new Widget(outerWidget);

    outerWidget->resize(600, 600);
    outerWidget->move(0, 0);
    outerWidget->setBackgroundColor("#ffffff");

    subWidget->resize(400, 400);
    subWidget->move(100, 300);
    subWidget->setBackgroundColor("#257E5F");

    application.run();

    return 0;
}

