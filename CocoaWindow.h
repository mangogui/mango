#include "Size.h"

namespace GUI {

    struct CocoaWindowWrapper;

    class CocoaWindow {
    public:
        CocoaWindow();

        ~CocoaWindow();

        void center();

        void display();

        const char *windowTitle() const;

        void setWindowTitle(const char *title);

        void resize(int width, int height);

        Size size() const;

        void maximize();
        void fullscreen();

    private:
        CocoaWindowWrapper *wrapper;
    };


}