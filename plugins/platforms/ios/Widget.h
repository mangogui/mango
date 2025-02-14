#ifndef MANGO_WIDGET_H
#define MANGO_WIDGET_H


#include <string>
#import <UIKit/UIKit.h>


struct UIKitWindowWrapper;
struct UIKitViewWrapper;

class Widget {
public:
    explicit Widget(Widget *parent = nullptr);

    ~Widget();

    [[nodiscard]] const char *windowTitle() const;

    void setWindowTitle(const char *title);

    void resize(int width, int height);

    virtual void mousePressEvent();

    virtual void resizeEvent();

    void move(int x, int y);

    void maximize();

    void fullscreen();

    void init_view();

    void update();


    void setObjectName(std::string obj_name) {
        this->object_name = obj_name;
    }

    std::string objectName() {
        return object_name;
    }

    float scaleFactor();

protected:
    UIKitWindowWrapper *window_wrapper;
    UIKitViewWrapper *view_wrapper;
    Widget *parent;
private:
    std::string object_name;
};


#endif //MANGO_WIDGET_H
