#include "CocoaWindow.h"
#include "CocoaApplication.h"

int main(int argc, const char * argv[]) {

    GUI::CocoaApplication application;

    GUI::CocoaWindow window;
    window.center();
    window.display();

    application.run();

    return 0;
}
