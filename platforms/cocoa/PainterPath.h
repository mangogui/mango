#ifndef MANGO_PAINTERPATH_H
#define MANGO_PAINTERPATH_H

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
        };

        PainterPath() noexcept = default;

        void addEllipse(const Rect &boundingRect);
        void addRect(const Rect &boundingRect);
        void addRoundedRect(const Rect &boundingRect, float x_radius, float y_radius);

        static void adaptiveApproximateCubicBezier(PainterPath& path, const CubicBezierCurve& curve, float threshold = 1.0);
        static float calculateCurveLength(const CubicBezierCurve& curve);
        static void splitCurve(const CubicBezierCurve& curve, float t, CubicBezierCurve& leftCurve, CubicBezierCurve& rightCurve);

        int elementCount() const { return elements.size(); }

        Element elementAt(int index) { return elements.at(index); }

        void clear() { elements.clear(); }

        void addElement(const Element& e) { elements.push_back(e); }

    private:
        std::vector<Element> elements;

        static Point evaluateCubicBezier(const CubicBezierCurve &curve, float t);

        static Point interpolate(const Point &p1, const Point &p2, float t);

        static float distanceBetweenPoints(const Point &p1, const Point &p2);
    };
}

#endif //MANGO_PAINTERPATH_H
