#include "Application.h"
#include "Widget.h"


int main() {
    Application application;

    auto* outerWidget = new Widget();
    auto* subWidget = new Widget(outerWidget);

    outerWidget->resize(600, 600);
    outerWidget->move(0, 0);
    outerWidget->setBackgroundColor("#257E5F");

    subWidget->resize(200, 200);
    subWidget->move(0, 0);
    subWidget->setBackgroundColor("#257E5F");

    application.run();

    return 0;
}