
#include "Mango.h"
#include "CustomWindow.h"

int main() {

    GUI::Application application;
    GUI::CustomWindow window;
    window.resize(600, 600);
    window.center();
    window.display();
    application.run();

    return 0;
}