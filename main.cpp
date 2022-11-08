
#include "platforms/platform.h"

int main() {

    GUI::Application application;
    GUI::Win32Window window;
    window.center();
    window.display();
    application.run();

    return 0;
}