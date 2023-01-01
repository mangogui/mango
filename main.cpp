
#include "platforms/platform.h"

int main() {

    GUI::Application application;
    GUI::Window window;
    window.resize(600, 600);
    window.center();
    window.display();
    application.run();

    return 0;
}