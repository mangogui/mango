#include "PlatformView.h"
#include <Widget.h>


int PlatformView::x() const {
    return m_widget->x();
}

int PlatformView::y() const {
    return m_widget->y();
}

int PlatformView::width() const {
    return m_widget->width();
}

int PlatformView::height() const {
    return m_widget->height();
}
