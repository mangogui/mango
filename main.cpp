#include "Application.h"
#include "Widget.h"

int main() {
    Application application;

    auto *outerWidget = new Widget();
    auto *subWidget = new Widget(outerWidget);

    outerWidget->resize(600, 600);
    outerWidget->move(0, 0);
    outerWidget->setBackgroundColor("#ffffff");

    return 0;
}

