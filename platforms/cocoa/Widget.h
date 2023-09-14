#pragma once

#include "../../Size.h"
#include "PainterPath.h"
#include <map>
#include "../../Color.h"
#include "Screen.h"
#include "../PaintEvent.h"


namespace GUI {

    struct CocoaWindowWrapper;
    struct MTKViewWrapper;

    class Widget {
    public:
        struct State {
            Color color;
        };

        explicit Widget(Widget *parent = nullptr);

        ~Widget();

        void center();

        void display();

        const char *windowTitle() const;

        void setWindowTitle(const char *title);

        void resize(int width, int height);

        virtual void mousePressEvent();
        virtual void resizeEvent();

        void move(int x, int y);

        Size size() const;

        void maximize();
        void fullscreen();

        void init_mtk_view();

        virtual void paintEvent(const PaintEvent& event);

        void update();

        PainterPath& painterPath() { return  _painterPath; }

        Screen screen();

        std::map<int, State>& states() { return _states; }

        const Color& fillColorAtIndex(int index) { return states().at(index).color; }
        void setObjectName(std::string obj_name) {
            this->object_name = obj_name;
        }

        std::string objectName() {
            return object_name;
        }

    protected:
        CocoaWindowWrapper *window_wrapper;
        MTKViewWrapper *view_wrapper;
        PainterPath _painterPath;
        std::map<int, State> _states;
        Widget *parent;
    private:
        std::string object_name;
    };


}