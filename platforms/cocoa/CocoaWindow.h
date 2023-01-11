#include "../../Size.h"
#include "../../Rect.h"

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

        void initMTKView();

        void drawRectangle(const Rect& rect);

        void paintEvent();

    private:
        CocoaWindowWrapper *wrapper;
    };


}