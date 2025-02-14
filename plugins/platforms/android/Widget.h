#ifndef MY_APPLICATION_WIDGET_H
#define MY_APPLICATION_WIDGET_H

#include <vector>
#include "JniObject.h"

#define LOG_TAG "Widget"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


enum MouseEventType {
    MousePress,
    MouseRelease,
    MouseMove,
    MouseCancel
};

struct MouseEvent {
    MouseEventType type;
    float x;
    float y;
};

class Widget {
public:
    explicit Widget(Widget *parent = nullptr);

    ~Widget();

    void handleMouseEvent(const MouseEvent &event);

    void createView();

    void createView(Widget *parent);

    void move(int x, int y);

    void resize(int width, int height);

    void setBackgroundColor(const std::string &colorString);

private:
    JniObject *m_viewObject;
    Widget *m_parent;
    std::vector<Widget *> m_children;
};


#endif //MY_APPLICATION_WIDGET_H