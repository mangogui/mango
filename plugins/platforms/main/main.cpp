#include "../Widget.h"

int main(int argc, char *argv[]) {
    Widget* outerWidget = new Widget();
    Widget* subWidget = new Widget(outerWidget);

    subWidget->setSize(400, 400);
    subWidget->setPosition(300, 900);
    subWidget->setBackgroundColor("#257E5F");
}