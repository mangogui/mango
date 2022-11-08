#include "CocoaWindowInterface.h"

namespace GUI {
    extern "C" CCocoaWindow* CocoaWindow_create() {
        return reinterpret_cast<CCocoaWindow*>(new CocoaWindow());
    }

    extern "C" void CocoaWindow_center(CCocoaWindow* self) {
        return reinterpret_cast<CocoaWindow*>(self)->center();
    }

    extern "C" void CocoaWindow_display(CCocoaWindow* self) {
        return reinterpret_cast<CocoaWindow*>(self)->display();
    }

// Getters
    extern "C" const char *CocoaWindow_windowTitle(CCocoaWindow* self) {
        return reinterpret_cast<CocoaWindow*>(self)->windowTitle();
    }

// Setters
    extern "C" void CocoaWindow_setWindowTitle(CCocoaWindow* self, char *title) {
        return reinterpret_cast<CocoaWindow*>(self)->setWindowTitle(title);
    }

    extern "C" void CocoaWindow_resize(CCocoaWindow* self, int width, int height) {
        return reinterpret_cast<CocoaWindow*>(self)->resize(width, height);
    }

    extern "C" void CocoaWindow_maximize(CCocoaWindow* self) {
        return reinterpret_cast<CocoaWindow*>(self)->maximize();
    }

    extern "C" void CocoaWindow_fullscreen(CCocoaWindow* self) {
        return reinterpret_cast<CocoaWindow*>(self)->fullscreen();
    }
}

