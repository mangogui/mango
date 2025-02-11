#pragma once

#include <string>
#include <utility>
#include "MNSize.h"
#include "PaintEvent.h"
#include "MouseEvent.h"
#include "ResizeEvent.h"


class Widget {
public:
    explicit Widget(Widget *parent = nullptr);
    ~Widget();

    void move(int x, int y);
    void resize(int width, int height);

    virtual void mousePressEvent(MouseEvent* event);
    virtual void mouseReleaseEvent(MouseEvent* event);
    virtual void resizeEvent(ResizeEvent* event);
    virtual void paintEvent(const PaintEvent& event);

    void maximize();
    void fullscreen();
    void update();

    // Setters
    void setWindowTitle(const std::string& title);
    void setBackgroundColor(const std::string& hexColor);
    void setObjectName(const std::string& obj_name);

    // Getters
    [[nodiscard]] std::string windowTitle() const;
    [[nodiscard]] float scaleFactor() const;
    [[nodiscard]] MNRect rect() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] const std::string& objectName() const { return object_name; }
    [[nodiscard]] void* getNSViewId() const { return view_wrapper; }
    [[nodiscard]] MNSize size() const;

protected:
    void *window_wrapper; // NSWindow id
    void *view_wrapper{nullptr}; // NSView id
    Widget *parent;
private:
    std::string object_name;

    void init_view();
};

