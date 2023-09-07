#ifndef MANGO_APPLICATION_H
#define MANGO_APPLICATION_H


#if __APPLE__
#include "cocoa/CocoaApplication.h"
typedef GUI::CocoaApplication NativeApplication;
#elif _WIN32
#include "win32/Win32Application.h"
typedef GUI::Win32Application NativeApplication;
#endif

namespace GUI {
    class Application: public NativeApplication {

    };
}


#endif //MANGO_APPLICATION_H
