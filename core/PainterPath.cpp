#include "PainterPath.h"

namespace GUI {
    void PainterPath::addEllipse(const MNRect &r) {
        float rx = r.width() * 0.5f;
        float ry = r.height() * 0.5f;
        float cx = r.x() + rx;
        float cy = r.y() + ry;

        // Define kappa as needed
        float kappa = 0.5522847498f;

        // Top right
        elements.push_back({cx + rx, cy, MoveTo});

        elements.push_back({cx + rx, cy - ry * kappa, CurveTo});
        elements.push_back({cx + rx * kappa, cy - ry, CurveToData});
        elements.push_back({cx, cy - ry, CurveToData});

        // Top left
        elements.push_back({cx - rx * kappa, cy - ry, CurveTo});
        elements.push_back({cx - rx, cy - ry * kappa, CurveToData});
        elements.push_back({cx - rx, cy, CurveToData});

        // Bottom left
        elements.push_back({cx - rx, cy + ry * kappa, CurveTo});
        elements.push_back({cx - rx * kappa, cy + ry, CurveToData});
        elements.push_back({cx, cy + ry, CurveToData});

        // Bottom right
        elements.push_back({cx + rx * kappa, cy + ry, CurveTo});
        elements.push_back({cx + rx, cy + ry * kappa, CurveToData});
        elements.push_back({cx + rx, cy, CurveToData});

        elements.push_back({cx + rx, cy, LineTo});
    }

    void PainterPath::addRect(const MNRect &r) {
        elements.push_back({r.x(), r.y(), MoveTo});
        Element l1 = {r.x() + r.width(), r.y(), LineTo};
        Element l2 = {r.x() + r.width(), r.y() + r.height(), LineTo};
        Element l3 = {r.x(), r.y() + r.height(), LineTo};
        Element l4 = {r.x(), r.y(), LineTo};
        elements.push_back(l1);
        elements.push_back(l2);
        elements.push_back(l3);
        elements.push_back(l4);
    }

    void PainterPath::addRoundedRect(const MNRect &r, float x_radius, float y_radius) {
        const float kappa = 0.5522847498f;
        float rx = std::min(x_radius, r.width() * 0.5f);
        float ry = std::min(y_radius, r.height() * 0.5f);
        float ox = rx * kappa;
        float oy = ry * kappa;

        // Top-left corner
        elements.push_back({r.x(), r.y() + ry, MoveTo});
        elements.push_back({r.x(), r.y() + ry - oy, CurveTo});
        elements.push_back({r.x() + rx - ox, r.y(), CurveToData});
        elements.push_back({r.x() + rx, r.y(), CurveToData});

        // Top-right corner
        elements.push_back({r.x() + r.width() - rx, r.y(), LineTo});
        elements.push_back({r.x() + r.width() - rx + ox, r.y(), CurveTo});
        elements.push_back({r.x() + r.width(), r.y() + ry - oy, CurveToData});
        elements.push_back({r.x() + r.width(), r.y() + ry, CurveToData});


        // Bottom-right corner
        elements.push_back({r.x() + r.width(), r.y() + r.height() - ry, LineTo});
        elements.push_back({r.x() + r.width(), r.y() + r.height() - ry + oy, CurveTo});
        elements.push_back({r.x() + r.width() - rx + ox, r.y() + r.height(), CurveToData});
        elements.push_back({r.x() + r.width() - rx, r.y() + r.height(), CurveToData});


        // Bottom-left corner
        elements.push_back({r.x() + rx, r.y() + r.height(), LineTo});
        elements.push_back({r.x() + rx - ox, r.y() + r.height(), CurveTo});
        elements.push_back({r.x(), r.y() + r.height() - ry + oy, CurveToData});
        elements.push_back({r.x(), r.y() + r.height() - ry, CurveToData});
    }

    void PainterPath::lineTo(int x, int y) {
        // TODO: fix this casting
        addElement(PainterPath::Element({static_cast<float>(x), static_cast<float>(y), LineTo}));
    }

    void PainterPath::moveTo(int x, int y) {
        // TODO: fix this casting
        addElement(PainterPath::Element({static_cast<float>(x), static_cast<float>(y), MoveTo}));
    }

    void PainterPath::moveTo(const Point &p) {
        // TODO: fix this casting
        moveTo(p.x(), p.y());
    }

    void PainterPath::cubicTo(const Point &c1, const Point &c2, const Point &endPoint) {
        elements.push_back({c1.x(), c1.y(), CurveTo});
        elements.push_back({c2.x(), c2.y(), CurveToData});
        elements.push_back({endPoint.x(), endPoint.y(), CurveToData});
    }

    void PainterPath::lineTo(const Point &p) {
        lineTo(p.x(), p.y());
    }

    PainterPath &PainterPath::operator<<(const PainterPath::Element &element) {
        addElement(element);
        return *this;
    }
}