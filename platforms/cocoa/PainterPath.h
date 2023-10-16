#pragma once

#include "../../Rect.h"
#include <vector>


namespace GUI {
    struct CubicBezierCurve {
        Point controlPoints[4];
    };

    class PainterPath {
    public:
        enum ElementType {
            MoveTo,
            LineTo,
            CurveTo,
            CurveToData
        };

        class Element {
        public:
            float x;
            float y;
            ElementType type;
            bool isMoveTo() const { return type == MoveTo; }
            bool isLineTo() const { return type == LineTo; }
            bool isCurveTo() const { return type == CurveTo; }
            bool isCurveToData() const { return type == CurveToData; }
            Point toPoint() const { return Point(x, y); }
        };

        PainterPath() noexcept = default;

        void addEllipse(const Rect &boundingRect);
        void addRect(const Rect &boundingRect);
        void addRoundedRect(const Rect &boundingRect, float x_radius, float y_radius);

        static void adaptiveApproximateCubicBezier(std::vector<GUI::Point>& points, const CubicBezierCurve& curve, float threshold = 1.0);
        static float calculateCurveLength(const CubicBezierCurve& curve);
        static void splitCurve(const CubicBezierCurve& curve, float t, CubicBezierCurve& leftCurve, CubicBezierCurve& rightCurve);
        static Point evaluateCubicBezier(const CubicBezierCurve &curve, float t);
        static Point interpolate(const Point &p1, const Point &p2, float t);
        static Point pointAtBezierCurve(const CubicBezierCurve &curve, float t);

        int elementCount() const { return elements.size(); }

        Element elementAt(int index) const { return elements.at(index); }

        void clear() { elements.clear(); }

        void addElement(const Element& e) { elements.push_back(e); }

        void lineTo(int x, int y);
        void moveTo(int x, int y);
        void moveTo(const Point& p);
        void cubicTo(const Point &c1, const Point &c2, const Point &endPoint);

        void lineTo(const Point &point);

        PainterPath& operator<<(const PainterPath::Element& element);

    private:
        std::vector<Element> elements;



        static float distanceBetweenPoints(const Point &p1, const Point &p2);

    };
}
