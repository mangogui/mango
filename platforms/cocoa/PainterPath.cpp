#include "PainterPath.h"
#include <iostream>

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

    void PainterPath::addRoundedRect(const Rect &r, float radius) {
        const float kappa = 0.5522847498f;
        float rx = std::min(radius, r.width() * 0.5f);
        float ry = std::min(radius, r.height() * 0.5f);
        float ox = rx * kappa;
        float oy = ry * kappa;

        elements.push_back({r.x() + rx, r.y(), MoveTo});

        // Top-right corner
        CubicBezierCurve topRightCurve;
        topRightCurve.controlPoints[0] = {r.x() + r.width() - rx, r.y()};
        topRightCurve.controlPoints[1] = {r.x() + r.width() - rx + ox, r.y()};
        topRightCurve.controlPoints[2] = {r.x() + r.width(), r.y() + ry - oy};
        topRightCurve.controlPoints[3] = {r.x() + r.width(), r.y() + ry};
        approximateCubicBezierCurve(topRightCurve);

        // Bottom-right corner
        CubicBezierCurve bottomRightCurve;
        bottomRightCurve.controlPoints[0] = {r.x() + r.width(), r.y() + r.height() - ry};
        bottomRightCurve.controlPoints[1] = {r.x() + r.width(), r.y() + r.height() - ry + oy};
        bottomRightCurve.controlPoints[2] = {r.x() + r.width() - rx + ox, r.y() + r.height()};
        bottomRightCurve.controlPoints[3] = {r.x() + r.width() - rx, r.y() + r.height()};
        approximateCubicBezierCurve(bottomRightCurve);

        // Bottom-left corner
        CubicBezierCurve bottomLeftCurve;
        bottomLeftCurve.controlPoints[0] = {r.x() + rx, r.y() + r.height()};
        bottomLeftCurve.controlPoints[1] = {r.x() + rx - ox, r.y() + r.height()};
        bottomLeftCurve.controlPoints[2] = {r.x(), r.y() + r.height() - ry + oy};
        bottomLeftCurve.controlPoints[3] = {r.x(), r.y() + r.height() - ry};
        approximateCubicBezierCurve(bottomLeftCurve);

        // Top-left corner
        CubicBezierCurve topLeftCurve;
        topLeftCurve.controlPoints[0] = {r.x(), r.y() + ry};
        topLeftCurve.controlPoints[1] = {r.x(), r.y() + ry - oy};
        topLeftCurve.controlPoints[2] = {r.x() + rx - ox, r.y()};
        topLeftCurve.controlPoints[3] = {r.x() + rx, r.y()};
        approximateCubicBezierCurve(topLeftCurve);
    }

    void PainterPath::approximateCubicBezierCurve(const CubicBezierCurve& curve) {
        elements.push_back({curve.controlPoints[0].x(), curve.controlPoints[0].y(), LineTo});
        const int segments = 16; // Number of line segments used to approximate the curve

        float tStep = 1.0f / segments;
        float t = tStep;

        for (int i = 1; i <= segments; ++i) {
            float oneMinusT = 1.0f - t;
            float oneMinusTSquared = oneMinusT * oneMinusT;
            float tSquared = t * t;

            float x = oneMinusTSquared * oneMinusT * curve.controlPoints[0].x()
                      + 3.0f * oneMinusTSquared * t * curve.controlPoints[1].x()
                      + 3.0f * oneMinusT * tSquared * curve.controlPoints[2].x()
                      + tSquared * t * curve.controlPoints[3].x();

            float y = oneMinusTSquared * oneMinusT * curve.controlPoints[0].y()
                      + 3.0f * oneMinusTSquared * t * curve.controlPoints[1].y()
                      + 3.0f * oneMinusT * tSquared * curve.controlPoints[2].y()
                      + tSquared * t * curve.controlPoints[3].y();

            elements.push_back({x, y, LineTo});

            t += tStep;
        }
    }
}