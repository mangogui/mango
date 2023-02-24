#ifndef MANGO_COCOAWINDOW_H
#define MANGO_COCOAWINDOW_H

#include "../../Size.h"
#include "../../Rect.h"

namespace GUI {

    struct CocoaWindowWrapper;
    struct CocoaViewWrapper;

    class CocoaWindow {
    public:
        explicit CocoaWindow(CocoaWindow *parent = nullptr);

        virtual ~CocoaWindow();

        void center();

        void display();

        const char *windowTitle() const;

        void setWindowTitle(const char *title);

        void resize(int width, int height);

        Size size() const;

        void maximize();
        void move(int x, int y);
        void fullscreen();

        virtual void paintEvent();
        virtual void mousePressEvent();
        virtual void resizeEvent();
        void update();

        CocoaWindowWrapper* getWrapper() { return window_wrapper; }

    protected:
        CocoaWindowWrapper *window_wrapper;
        CocoaViewWrapper *view_wrapper;
        bool embedded;

        void* getWindow();

        void* getView();
    };

}

#endif