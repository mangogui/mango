#include "PainterPath.h"

namespace GUI {
    void PainterPath::addEllipse(const Rect &r) {
        elements.push_back({r.x(), r.y(), MoveTo});
        Element l1 = { r.x() + r.width(), r.y(), CurveTo };
        Element l2 = { r.x() + r.width(), r.y() + r.height(), CurveTo };
        Element l3 = { r.x(), r.y() + r.height(), CurveTo };
        Element l4 = { r.x(), r.y(), CurveTo };
        elements.push_back(l1);
        elements.push_back(l2);
        elements.push_back(l3);
        elements.push_back(l4);
    }

    void PainterPath::addRect(const Rect &r) {
        elements.push_back({r.x(), r.y(), MoveTo});
        Element l1 = { r.x() + r.width(), r.y(), LineTo };
        Element l2 = { r.x() + r.width(), r.y() + r.height(), LineTo };
        Element l3 = { r.x(), r.y() + r.height(), LineTo };
        Element l4 = { r.x(), r.y(), LineTo };
        elements.push_back(l1);
        elements.push_back(l2);
        elements.push_back(l3);
        elements.push_back(l4);
    }
}