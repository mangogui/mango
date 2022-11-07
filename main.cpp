#include "Win32Window.h"
#include "Win32Application.h"


int main() {

    Win32Application win32Application;
    win32Application.run();
    Window window = Window();
    window.set_window_title("GUI test");
    window.show();

    return 0;
}