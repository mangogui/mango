#pragma once
#include <Event.h>
#include <MNSize.h>

class ResizeEvent: public Event {
public:
    ResizeEvent(int newWidth, int newHeight)
            : Event(Event::Type::ResizeEvent), m_width(newWidth), m_height(newHeight) {}

    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }
    [[nodiscard]] MNSize size() const { return {m_width, m_height}; }

private:
    int m_width;
    int m_height;
};

