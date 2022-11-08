#ifndef GUI_COCOAAPPLICATION_H
#define GUI_COCOAAPPLICATION_H

namespace GUI {

    struct CocoaApplicationWrapper;

    class CocoaApplication {
    public:
        CocoaApplication();

        ~CocoaApplication();

        void run();

    private:
        CocoaApplicationWrapper *wrapper;
    };

    // ========== C-interface
    struct CCocoaApplication;
    typedef struct CCocoaApplication CCocoaApplication;

    extern "C" CCocoaApplication* create();
    extern "C" void run(CCocoaApplication*);

}

#endif //GUI_COCOAAPPLICATION_H
