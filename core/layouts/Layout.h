#pragma once

#include <vector>
class AbstractWidget;

class Layout {
public:
    explicit Layout(AbstractWidget* parent = nullptr): m_parent(parent) {}
    virtual void applyLayout() = 0;
    virtual void addWidget(AbstractWidget* widget);
    virtual ~Layout() = default;
protected:
    std::vector<AbstractWidget*> widgets;
    AbstractWidget* m_parent;
};


