
#include "Mango.h"
#include "CustomWindow.h"


int main() {
    Application application;
    CustomWindow window;
    window.resize(1400, 1000);
    window.center();
    window.display();
    application.run();

    return 0;
}