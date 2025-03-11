#include "BoxLayout.h"

BoxLayout::BoxLayout(AbstractWidget* parent, Orientation orientation) : Layout(parent), orientation(orientation) {}

void BoxLayout::applyLayout() {
    int x = 0;
    int y = 0;
    int w = m_parent->width();
    int h = m_parent->height();

    int _s = widgets.size();

    if (_s == 0) return; // Handle the case where there are no widgets

    if (orientation == Orientation::HORIZONTAL) {
        int baseWidth = w / _s;
        int remainingWidth = w % _s; // Remainder after integer division

        for (int i = 0; i < _s; ++i) {
            int currentWidth = baseWidth;
            if (i < remainingWidth) {
                currentWidth++; // Distribute remaining pixels
            }
            widgets[i]->move(x, y);
            widgets[i]->resize(currentWidth, h);
            x += currentWidth;
        }
    } else { // Vertical orientation
        int baseHeight = h / _s;
        int remainingHeight = h % _s;

        for (int i = 0; i < _s; ++i) {
            int currentHeight = baseHeight;
            if (i < remainingHeight) {
                currentHeight++;
            }
            widgets[i]->move(x, y);
            widgets[i]->resize(w, currentHeight);
            y += currentHeight;
        }
    }
}