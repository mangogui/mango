#pragma once

#include <string>


class Object {
public:
    const std::string objectName() const { return m_objectName; };

    void setObjectName(std::string objectName) {
        m_objectName = std::move(objectName);
    }
private:
    std::string m_objectName;
};