#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <memory>

#include <MNSize.h>
#include <Event.h>
#include <PaintEvent.h>
#include <MouseEvent.h>
#include <ResizeEvent.h>
#include <PlatformWindow.h>
#include <PlatformView.h>
#include <Object.h>


class AbstractWidget {
public:
    virtual void center() = 0;
    virtual void move(int x, int y) = 0;
    virtual void resize(int width, int height) = 0;
    virtual void maximize() = 0;
    virtual void fullscreen() = 0;
    virtual void display() = 0;
    virtual void update() = 0;
    virtual void create() = 0;

    // Setters
    virtual void setWindowTitle(const std::string& title) = 0;
    virtual void setBackgroundColor(const std::string &hexColor) = 0;
    virtual void setNativeContext(void* context) = 0;

    // Getters
    [[nodiscard]] virtual int x() const = 0;
    [[nodiscard]] virtual int y() const = 0;
    [[nodiscard]] virtual int width() const = 0;
    [[nodiscard]] virtual int height() const = 0;
    [[nodiscard]] virtual MNRect rect() const = 0;
    [[nodiscard]] virtual std::string getWindowTitle() const = 0;
    [[nodiscard]] virtual const MNRect& geometry() = 0;
    [[nodiscard]] virtual MNSize size() const = 0;
    [[nodiscard]] virtual bool isCreated() const = 0;

    // Events
    virtual void handleEvent(Event *event) = 0;
    virtual void mousePressEvent(MouseEvent *event) = 0;
    virtual void mouseReleaseEvent(MouseEvent *event) = 0;
    virtual void resizeEvent(ResizeEvent *event) = 0;
    virtual void paintEvent(PaintEvent *event) = 0;

    [[nodiscard]] virtual void* getWinId() const = 0;
    [[nodiscard]] virtual bool isTopLevel() const = 0;

    virtual void setGraphicsContext(void* context) = 0;
    virtual void* getGraphicsContext() = 0;
};

