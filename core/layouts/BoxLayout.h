#include <Layout.h>
#include <AbstractWidget.h>

class BoxLayout : public Layout {
public:
    enum class Orientation { HORIZONTAL, VERTICAL };
    BoxLayout(AbstractWidget* parent, Orientation orientation);
    void applyLayout() override;
private:
    Orientation orientation;
};