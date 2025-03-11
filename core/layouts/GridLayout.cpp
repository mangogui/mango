#include "GridLayout.h"

GridLayout::GridLayout(AbstractWidget* parent, int rows, int cols) : Layout(parent), m_rows(rows), m_cols(cols) {
}

void GridLayout::applyLayout() {
    if (widgets.empty() || m_rows <= 0 || m_cols <= 0) return;

    int totalWidgets = widgets.size();
    int w = m_parent->width();
    int h = m_parent->height();

    int cellWidth = w / m_cols;
    int remainingWidth = w % m_cols; // Remaining pixels to distribute
    int cellHeight = h / m_rows;
    int remainingHeight = h % m_rows; // Remaining pixels to distribute

    int y = 0; // Start at top
    for (int row = 0; row < m_rows; ++row) {
        int currentHeight = cellHeight + (row < remainingHeight ? 1 : 0); // Distribute extra height
        int x = 0; // Start at left

        for (int col = 0; col < m_cols; ++col) {
            int currentWidth = cellWidth + (col < remainingWidth ? 1 : 0); // Distribute extra width
            int index = row * m_cols + col;

            if (index >= totalWidgets) break; // Stop if we run out of widgets

            widgets[index]->move(x, y);
            widgets[index]->resize(currentWidth, currentHeight);

            x += currentWidth; // Move to the next column
        }

        y += currentHeight; // Move to the next row
    }
}
