#include <Layout.h>

class GridLayout : public Layout {
public:
    GridLayout(int rows, int cols);
    void applyLayout() override;
private:
    int m_rows, m_cols;
};