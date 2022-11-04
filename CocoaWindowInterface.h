#ifndef MANGO_COCOAWINDOWINTERFACE_H
#define MANGO_COCOAWINDOWINTERFACE_H

#include "CocoaWindow.h"

namespace GUI {
    struct CCocoaWindow;
    typedef struct CCocoaWindow CCocoaWindow;

    extern "C" CCocoaWindow* CocoaWindow_create();
    extern "C" void CocoaWindow_center(CCocoaWindow*);
    extern "C" void CocoaWindow_display(CCocoaWindow*);
// Getters
    extern "C" const char *CocoaWindow_windowTitle(CCocoaWindow*);
// Setters
    extern "C" void CocoaWindow_setWindowTitle(CCocoaWindow* self, char *title);
    extern "C" void CocoaWindow_resize(CCocoaWindow* self, int width, int height);
    extern "C" void CocoaWindow_maximize(CCocoaWindow*);
}



#endif //MANGO_COCOAWINDOWINTERFACE_H
