
#include "Mango.h"
#include "CustomWindow.h"


int main() {
    Application application;
    CustomWindow window;
    window.resize(1400, 1000);
    window.center();
    window.display();
    GUI::ToggleButton toggleButton(&window);
    toggleButton.move(100, 150);
    toggleButton.resize(200, 100);
    application.run();

    return 0;
}