#ifndef MANGO_PAINTERPATH_H
#define MANGO_PAINTERPATH_H

#include "../../Rect.h"
#include <vector>

namespace GUI {
    class PainterPath {
    public:
        enum ElementType {
            MoveTo,
            LineTo,
            CurveTo,
            QuadTo
        };

        struct CubicBezierCurve {
            Point controlPoints[4];
        };

        class Element {
        public:
            float x;
            float y;
            ElementType type;
            bool isMoveTo() const { return type == MoveTo; }
            bool isLineTo() const { return type == LineTo; }
            bool isCurveTo() const { return type == CurveTo; }
        };

        PainterPath() noexcept = default;

        void addEllipse(const Rect &boundingRect);
        void addRect(const Rect &boundingRect);
        void addRoundedRect(const Rect &boundingRect, float radius);

        void approximateCubicBezierCurve(const CubicBezierCurve& curve);

        int elementCount() const { return elements.size(); }

        Element elementAt(int index) { return elements.at(index); }

        void clear() { elements.clear(); }

    private:
        std::vector<Element> elements;
    };
}

#endif //MANGO_PAINTERPATH_H
