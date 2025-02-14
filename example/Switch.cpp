#include "Switch.h"
#include "Mango/Canvas.h"


Switch::Switch(Widget *parent) : Widget(parent), m_checked(false),
                                 m_transitionAnimation(nullptr), m_bgColor(Color("#E9E9EA")) {
    resize(200, 100);
    m_ellipseX = height() * .1;
}

void Switch::toggle() {
    m_checked = !m_checked;
    startAnimation();
}

void Switch::mousePressEvent(MouseEvent *event) {
    toggle();
    startAnimation();
}

void Switch::startAnimation() {
    float startX = isChecked() ? height() * .1 : width() - height() * .9;
    float endX = isChecked() ? width() - height() * .9 : height() * .1;

    m_transitionAnimation = std::make_shared<NumberAnimation>(
            startX,
            endX,
            400,
            [this](int value) {
                m_ellipseX = value;
                update(); // Request repaint
            }
    );
    m_transitionAnimation->start();

    m_colorAnimation = std::make_shared<ColorAnimation>(
            isChecked() ? Color("#E9E9EA") : Color("#65C466"),
            isChecked() ? Color("#65C466") : Color("#E9E9EA"),
            400,
            [this](Color value) {
                m_bgColor = value;
                update(); // Request repaint
            }
    );
    m_colorAnimation->start();
}

void Switch::paintEvent(const PaintEvent &event) {
    Canvas canvas(this);
    canvas.begin();

    const auto &_size = size();

    // Draw background
    canvas.setFillStyle(FillStyle(m_bgColor));
    canvas.drawRoundedRect(MNRect(0, 0, width(), height()), _size.getHeight() / 2, _size.getHeight() / 2);

    // Draw the sliding circle
    canvas.setFillStyle(FillStyle(Color("#FFFFFF")));
    canvas.drawEllipse(
            MNRect(m_ellipseX, height() * .1, height() * .8, height() * .8)); // Make circle size match m_height

    canvas.end();
}

