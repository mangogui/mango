
#include "platforms/platform.h"

int main() {

    GUI::Application application;
    GUI::Window window;
    window.center();
    window.display();
    application.run();

    return 0;
}