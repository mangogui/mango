#ifndef MANGO_WINDOW_H
#define MANGO_WINDOW_H


#if __APPLE__
#include "cocoa/CocoaWindow.h"
typedef GUI::CocoaWindow NativeWindow;
#elif _WIN32
#include "win32/Win32Window.h"
typedef GUI::Win32Window NativeWindow;
#endif

namespace GUI {

    class Window : public NativeWindow {

    };

}

#endif //MANGO_WINDOW_H
