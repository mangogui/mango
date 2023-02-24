#include "platforms/platform.h"
#include "platforms/cocoa/ToggleButton.h"


int main() {

    GUI::Application application;
    GUI::CocoaWindow window;
    window.resize(400, 400);
    window.center();
    window.display();
    GUI::ToggleButton toggleButton(&window);
    toggleButton.move(100, 150);
    toggleButton.resize(200, 100);
    application.run();

    return 0;
}