#pragma once

#include "../../Size.h"

namespace GUI {
    class Screen {
    public:
        void setSize(Size s) {
            _size = s;
        }
        Size size() { return _size; }
    private:
        Size _size;
    };
}


