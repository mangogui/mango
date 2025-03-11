#include <Layout.h>
#include <AbstractWidget.h>

class GridLayout : public Layout {
public:
    GridLayout(AbstractWidget* parent, int rows, int cols);
    void applyLayout() override;
private:
    int m_rows, m_cols;
};
