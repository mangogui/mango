#include "PlatformWindow.h"
#include <Widget.h>

const MNRect& PlatformWindow::geometry() const {
    return m_widget->geometry();
}

int PlatformWindow::x() const {
    return m_widget->x();
}

int PlatformWindow::y() const {
    return m_widget->y();
}

int PlatformWindow::width() const {
    return m_widget->width();
}

int PlatformWindow::height() const {
    return m_widget->height();
}