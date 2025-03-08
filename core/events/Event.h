#pragma once

class Event {
public:
    enum class Type {
        MouseEvent,
        PaintEvent,
        ResizeEvent,
    };
    explicit Event(const Type& type): m_type(type) {}

    void setType(const Type& type) {
        m_type = type;
    }

    [[nodiscard]] const Type& type() const {
        return m_type;
    }

    virtual ~Event() = default;
private:
    Type m_type;
};

