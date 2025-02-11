#pragma once

#include "Mango/Widget.h"
#include "Mango/NumberAnimation.h"
#include "Mango/ColorAnimation.h"

class Switch : public Widget {
public:
    explicit Switch(Widget *parent = nullptr);

    void toggle() {
        m_checked = !m_checked;
        startAnimation();
    }

    void mousePressEvent(MouseEvent *event) override {
        toggle();
        startAnimation();
    }

    // Getters
    [[nodiscard]] bool isChecked() const {
        return m_checked;
    }

    // Setters
    void setChecked(bool checked) {
        m_checked = checked;
    }

protected:
    void paintEvent(const PaintEvent& event) override;

private:
    void startAnimation();

    bool m_checked{};
    float m_ellipseX{};
    Color m_bgColor;
    std::shared_ptr<NumberAnimation> m_transitionAnimation;
    std::shared_ptr<ColorAnimation> m_colorAnimation;
};
