#pragma once

#include <string>


class Object {
public:
    std::string objectName() { return m_objectName; };

    void setObjectName(std::string objectName) {
        m_objectName = std::move(objectName);
    }
private:
    std::string m_objectName;
};