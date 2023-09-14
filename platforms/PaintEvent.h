#pragma once
#include "../Rect.h"


namespace GUI {
    class PaintEvent {
        public:
            PaintEvent() = default;
            void setRect(const Rect& r) {
                this->_rect = r;
            }

            const Rect& rect() const { return _rect; }

        private:
            Rect _rect;

    };
};


