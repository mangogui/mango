#pragma once

#include <Point.h>
#include <MNRect.h>
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

            [[nodiscard]] bool isMoveTo() const { return type == MoveTo; }

            [[nodiscard]] bool isLineTo() const { return type == LineTo; }

            [[nodiscard]] bool isCurveTo() const { return type == CurveTo; }

            [[nodiscard]] bool isCurveToData() const { return type == CurveToData; }

            [[nodiscard]] Point toPoint() const { return {x, y}; }
        };

        PainterPath() noexcept = default;

        void addEllipse(const MNRect &boundingRect);

        void addRect(const MNRect &boundingRect);

        void addRoundedRect(const MNRect &boundingRect, float x_radius, float y_radius);


        void clear() { elements.clear(); }

        void addElement(const Element &e) { elements.push_back(e); }

        void lineTo(int x, int y);

        void moveTo(int x, int y);

        void moveTo(const Point &p);

        void cubicTo(const Point &c1, const Point &c2, const Point &endPoint);

        void lineTo(const Point &point);

        [[nodiscard]] int elementCount() const { return elements.size(); }

        [[nodiscard]] Element elementAt(int index) const { return elements.at(index); }

        PainterPath &operator<<(const PainterPath::Element &element);

    private:
        std::vector<Element> elements;
    };
}
