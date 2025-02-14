#pragma once

#include <Widget.h>
#include <NumberAnimation.h>
#include <ColorAnimation.h>

class Switch : public Widget {
public:
    explicit Switch(Widget *parent = nullptr);

    void toggle();

    void mousePressEvent(MouseEvent *event) override;

    // Getters
    [[nodiscard]] bool isChecked() const {
        return m_checked;
    }

    // Setters
    void setChecked(bool checked) {
        m_checked = checked;
    }

protected:
    void paintEvent(const PaintEvent &event) override;

private:
    void startAnimation();

    bool m_checked{};
    float m_ellipseX{};
    Color m_bgColor;
    std::shared_ptr<NumberAnimation> m_transitionAnimation;
    std::shared_ptr<ColorAnimation> m_colorAnimation;
};
