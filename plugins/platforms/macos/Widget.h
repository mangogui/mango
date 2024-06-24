#ifndef WIDGET_H
#define WIDGET_H

#include <string>
#include "MNSize.h"
#include "PaintEvent.h"


struct CocoaWindowWrapper;
struct ViewWrapper;

class Widget {
public:
    explicit Widget(Widget *parent = nullptr);

    ~Widget();

    const char *windowTitle() const;

    void setWindowTitle(const char *title);

    void setBackgroundColor(std::string hexColor);

    void resize(int width, int height);

    virtual void mousePressEvent();
    virtual void resizeEvent();

    void move(int x, int y);

    MNSize size() const;

    void maximize();
    void fullscreen();

    void init_view();

    virtual void paintEvent(const PaintEvent& event);

    void update();


    void setObjectName(std::string obj_name) {
        this->object_name = obj_name;
    }

    std::string objectName() {
        return object_name;
    }

    float scaleFactor();

protected:
    CocoaWindowWrapper *window_wrapper;
    ViewWrapper *view_wrapper;
    Widget *parent;
private:
    std::string object_name;
};

#endif