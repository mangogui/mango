#include <Application.h>
#include <Widget.h>
#include <GridLayout.h>


int main(int argc, char* argv[]) {
    Application &application = Application::instance();

    auto *outerWidget = new Widget();
    outerWidget->setObjectName("Window");
    outerWidget->resize(600, 600);
    outerWidget->move(400, 400);
    outerWidget->setBackgroundColor("#ffffff");

    auto *widget1 = new Widget(outerWidget);
    widget1->setBackgroundColor(Color::Red);

    auto *widget2 = new Widget(outerWidget);
    widget2->setBackgroundColor(Color::Green);

    auto *widget3 = new Widget(outerWidget);
    widget3->setBackgroundColor(Color::Blue);

    auto *widget4 = new Widget(outerWidget);
    widget4->setBackgroundColor(Color::Yellow);

    auto *layout = new GridLayout(outerWidget, 2, 2);
    outerWidget->setLayout(layout);
    layout->addWidget(widget1);
    layout->addWidget(widget2);
    layout->addWidget(widget3);
    layout->addWidget(widget4);

    outerWidget->display();

    application.run();

    return 0;
}

