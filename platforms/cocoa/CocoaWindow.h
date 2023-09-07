#pragma once

#include "../../Size.h"
#include "PainterPath.h"
#include <map>
#include "../../Color.h"
#include "Screen.h"

namespace GUI {

    struct CocoaWindowWrapper;
    struct CocoaViewWrapper;

    class CocoaWindow {
    public:
        struct State {
            Color color;
        };

        CocoaWindow();

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

        virtual void paintEvent();

        PainterPath& painterPath() { return  _painterPath; }

        Screen screen();

        std::map<int, State>& states() { return _states; }

        const Color& fillColorAtIndex(int index) { return states().at(index).color; }

    protected:
        CocoaWindowWrapper *wrapper;
        PainterPath _painterPath;
        std::map<int, State> _states;
    };

}

#endif