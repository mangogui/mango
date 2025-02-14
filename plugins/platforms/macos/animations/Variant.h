#pragma once

#include <variant>
#include <iostream>
#include "Color.h"

class Variant {
public:
    enum class Type {
        None,
        Int,
        Float,
        Color
    };

    using VariantType = std::variant<std::monostate, int, float, Color>;

    // Constructors
    Variant() : m_value(std::monostate{}), m_type(Type::None) {}

    Variant(int value) : m_value(value), m_type(Type::Int) {}

    Variant(float value) : m_value(value), m_type(Type::Float) {}

    Variant(Color value) : m_value(value), m_type(Type::Color) {}

    // Setter
    void setValue(int value) {
        m_value = value;
        m_type = Type::Int;
    }

    void setValue(float value) {
        m_value = value;
        m_type = Type::Float;
    }

    void setValue(Color value) {
        m_value = value;
        m_type = Type::Color;
    }

    // Getter for current type
    [[nodiscard]] Type getType() const {
        return m_type;
    }

    // Safe Getters
    [[nodiscard]] int getInt() const {
        return (m_type == Type::Int) ? std::get<int>(m_value) : 0;
    }

    [[nodiscard]] float getFloat() const {
        return (m_type == Type::Float) ? std::get<float>(m_value) : 0.0f;
    }

    [[nodiscard]] Color getColor() const {
        return (m_type == Type::Color) ? std::get<Color>(m_value) : Color();
    }

    operator int() const {
        if (m_type == Type::Int) {
            return std::get<int>(m_value);
        }
        // Handle the case where the variant isn't an int
        std::cerr << "Variant does not hold an int!" << std::endl;
        return 0; // Default return value
    }

    operator float() const {
        if (m_type == Type::Float) {
            return std::get<float>(m_value);
        }
        // Handle the case where the variant isn't an int
        std::cerr << "Variant does not hold an float!" << std::endl;
        return 0; // Default return value
    }

    operator Color() const {
        if (m_type == Type::Color) {
            return std::get<Color>(m_value);
        }
        std::cerr << "Variant does not hold a Color!" << std::endl;
        return {}; // Return a default Color
    }

private:
    VariantType m_value;
    Type m_type;
};