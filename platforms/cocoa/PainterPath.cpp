#include "PainterPath.h"
#include <iostream>

namespace GUI {
    void PainterPath::addEllipse(const Rect &r) {
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

    void PainterPath::addRoundedRect(const Rect &r, float x_radius, float y_radius) {
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

    void PainterPath::adaptiveApproximateCubicBezier(std::vector<GUI::Point>& points, const CubicBezierCurve& curve, float threshold) {
        float length = calculateCurveLength(curve); // Implement a function to calculate curve length

        if (length <= threshold) {
            if (points.empty() || points.back() != curve.controlPoints[0]) {
                points.push_back(Point(curve.controlPoints[0].x(), curve.controlPoints[0].y()));
            }
        } else {
            CubicBezierCurve leftSubCurve, rightSubCurve;
            splitCurve(curve, 0.5f, leftSubCurve, rightSubCurve); // Implement a function to split the curve

            adaptiveApproximateCubicBezier(points, leftSubCurve, threshold);
            adaptiveApproximateCubicBezier(points, rightSubCurve, threshold);
        }
    }

    float PainterPath::calculateCurveLength(const CubicBezierCurve& curve) {
        const int numSegments = 100; // Number of line segments for approximation
        float length = 0.0f;

        for (int i = 0; i < numSegments; ++i) {
            float t1 = static_cast<float>(i) / numSegments;
            float t2 = static_cast<float>(i + 1) / numSegments;

            Point p1 = evaluateCubicBezier(curve, t1);
            Point p2 = evaluateCubicBezier(curve, t2);

            length += distanceBetweenPoints(p1, p2);
        }

        return length;
    }

    void PainterPath::splitCurve(const CubicBezierCurve& curve, float t, CubicBezierCurve& leftCurve, CubicBezierCurve& rightCurve) {
        Point p01 = interpolate(curve.controlPoints[0], curve.controlPoints[1], t);
        Point p12 = interpolate(curve.controlPoints[1], curve.controlPoints[2], t);
        Point p23 = interpolate(curve.controlPoints[2], curve.controlPoints[3], t);
        Point p012 = interpolate(p01, p12, t);
        Point p123 = interpolate(p12, p23, t);
        Point p0123 = interpolate(p012, p123, t);

        leftCurve.controlPoints[0] = curve.controlPoints[0];
        leftCurve.controlPoints[1] = p01;
        leftCurve.controlPoints[2] = p012;
        leftCurve.controlPoints[3] = p0123;

        rightCurve.controlPoints[0] = p0123;
        rightCurve.controlPoints[1] = p123;
        rightCurve.controlPoints[2] = p23;
        rightCurve.controlPoints[3] = curve.controlPoints[3];
    }

    Point PainterPath::evaluateCubicBezier(const CubicBezierCurve& curve, float t) {
        float oneMinusT = 1.0f - t;
        float oneMinusTSquared = oneMinusT * oneMinusT;
        float tSquared = t * t;

        Point result;
        result.set_x(oneMinusTSquared * oneMinusT * curve.controlPoints[0].x()
                   + 3.0f * oneMinusTSquared * t * curve.controlPoints[1].x()
                   + 3.0f * oneMinusT * tSquared * curve.controlPoints[2].x()
                   + tSquared * t * curve.controlPoints[3].x());

        result.set_y(oneMinusTSquared * oneMinusT * curve.controlPoints[0].y()
                   + 3.0f * oneMinusTSquared * t * curve.controlPoints[1].y()
                   + 3.0f * oneMinusT * tSquared * curve.controlPoints[2].y()
                   + tSquared * t * curve.controlPoints[3].y());

        return result;
    }

    Point PainterPath::interpolate(const Point& p1, const Point& p2, float t) {
        Point result;
        result.set_x((1.0f - t) * p1.x() + t * p2.x());
        result.set_y((1.0f - t) * p1.y() + t * p2.y());
        return result;
    }

    float PainterPath::distanceBetweenPoints(const Point& p1, const Point& p2) {
        float dx = p2.x() - p1.x();
        float dy = p2.y() - p1.y();
        return std::sqrt(dx * dx + dy * dy);
    }

    void PainterPath::lineTo(int x, int y) {
        // TODO: fix this casting
        addElement(PainterPath::Element({static_cast<float>(x), static_cast<float>(y), LineTo}));
    }

    void PainterPath::moveTo(int x, int y) {
        // TODO: fix this casting
        addElement(PainterPath::Element({static_cast<float>(x), static_cast<float>(y), MoveTo}));
    }

    void PainterPath::moveTo(const Point& p) {
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

    Point PainterPath::pointAtBezierCurve(const CubicBezierCurve &curve, float t) {
        Point p01 = interpolate(curve.controlPoints[0], curve.controlPoints[1], t);
        Point p12 = interpolate(curve.controlPoints[1], curve.controlPoints[2], t);
        Point p23 = interpolate(curve.controlPoints[2], curve.controlPoints[3], t);
        Point p012 = interpolate(p01, p12, t);
        Point p123 = interpolate(p12, p23, t);
        Point p0123 = interpolate(p012, p123, t);
        return p0123;
    }
}