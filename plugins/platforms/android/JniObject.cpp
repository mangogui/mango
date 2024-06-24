#include "JniObject.h"
#include "JniEnvironment.h"


JniObject::JniObject(const std::string& className, const std::string& constructorSig, ...) : m_env(JniEnvironment::getInstance().getEnv()), m_obj(nullptr), m_class(nullptr) {
    m_class = m_env->FindClass(className.c_str());
    if (!m_class) {
        throw std::runtime_error("Class not found: " + className);
    }

    jmethodID constructor = m_env->GetMethodID(m_class, "<init>", constructorSig.c_str());
    if (!constructor) {
        throw std::runtime_error("Constructor not found: " + className + constructorSig);
    }

    va_list args;
    va_start(args, constructorSig);
    m_obj = m_env->NewObjectV(m_class, constructor, args);
    va_end(args);

    if (!m_obj) {
        throw std::runtime_error("Failed to create object: " + className);
    }

    m_obj = m_env->NewGlobalRef(m_obj);
}

JniObject::JniObject(jobject obj) : m_env(JniEnvironment::getInstance().getEnv()), m_obj(obj), m_class(nullptr) {
    if (!m_obj) {
        throw std::runtime_error("Invalid jobject");
    }
    m_class = m_env->GetObjectClass(m_obj);
    m_obj = m_env->NewGlobalRef(m_obj);
}

JniObject::~JniObject() {
    if (m_obj) {
        m_env->DeleteGlobalRef(m_obj);
    }
    if (m_class) {
        m_env->DeleteLocalRef(m_class);
    }
}


jobject JniObject::getObject() const {
    return m_obj;
}

bool JniObject::isInstanceOf(const std::string& className) const {
    jclass clazz = m_env->FindClass(className.c_str());
    if (!clazz) {
        throw std::runtime_error("Class not found: " + className);
    }
    bool result = m_env->IsInstanceOf(m_obj, clazz);
    m_env->DeleteLocalRef(clazz);
    return result;
}

jstring JniObject::createStringUTF(const std::string& utf8String) {
    return JniEnvironment::getInstance().getEnv()->NewStringUTF(utf8String.c_str());
}