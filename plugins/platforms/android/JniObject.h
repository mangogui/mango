#ifndef MY_APPLICATION_JNIOBJECT_H
#define MY_APPLICATION_JNIOBJECT_H

#include "jni.h"
#include <cstdarg>
#include <stdexcept>
#include <type_traits>
#include <string>
#include "JniEnvironment.h"

template<typename T>
struct is_jni_primitive : std::false_type {
};

template<>
struct is_jni_primitive<void> : std::true_type {
};
template<>
struct is_jni_primitive<jint> : std::true_type {
};
template<>
struct is_jni_primitive<jboolean> : std::true_type {
};
template<>
struct is_jni_primitive<jbyte> : std::true_type {
};
template<>
struct is_jni_primitive<jchar> : std::true_type {
};
template<>
struct is_jni_primitive<jshort> : std::true_type {
};
template<>
struct is_jni_primitive<jlong> : std::true_type {
};
template<>
struct is_jni_primitive<jfloat> : std::true_type {
};
template<>
struct is_jni_primitive<jdouble> : std::true_type {
};
template<>
struct is_jni_primitive<jobject> : std::true_type {
};
template<>
struct is_jni_primitive<jstring> : std::true_type {
};
template<>
struct is_jni_primitive<jclass> : std::true_type {
};
template<>
struct is_jni_primitive<jthrowable> : std::true_type {
};
template<>
struct is_jni_primitive<jarray> : std::true_type {
};

class JniObject {
public:
    // Constructors and Destructor
    JniObject(const std::string &className, const std::string &constructorSig, ...);

    JniObject(jobject obj);

    ~JniObject();

    // Method to call instance methods
    template<typename ReturnType, typename... Args>
    ReturnType callMethod(const std::string &methodName, const std::string &methodSig, Args... args) const;

    // Static method to call static methods
    template<typename ReturnType, typename... Args>
    static ReturnType
    callStaticMethod(const std::string &className, const std::string &methodName, const std::string &methodSig,
                     Args... args);

    // Getter for the underlying jobject
    jobject getObject() const;

    // Method to check instance of a class
    bool isInstanceOf(const std::string &className) const;

    static jstring createStringUTF(const std::string &utf8String);

private:
    JNIEnv *m_env;
    jobject m_obj;
    jclass m_class;
};


template<typename ReturnType, typename... Args>
ReturnType JniObject::callMethod(const std::string &methodName, const std::string &methodSig, Args... args) const {
    static_assert(is_jni_primitive<ReturnType>::value, "Unsupported return type");

    jmethodID methodID = m_env->GetMethodID(m_class, methodName.c_str(), methodSig.c_str());
    if (!methodID) {
        throw std::runtime_error("Method not found: " + methodName + methodSig);
    }

    if constexpr (std::is_same_v<ReturnType, void>) {
        m_env->CallVoidMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jint>) {
        return m_env->CallIntMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jboolean>) {
        return m_env->CallBooleanMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jbyte>) {
        return m_env->CallByteMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jchar>) {
        return m_env->CallCharMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jshort>) {
        return m_env->CallShortMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jlong>) {
        return m_env->CallLongMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jfloat>) {
        return m_env->CallFloatMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jdouble>) {
        return m_env->CallDoubleMethod(m_obj, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jobject>) {
        return static_cast<ReturnType>(m_env->CallObjectMethod(m_obj, methodID, args...));
    } else {
        static_assert(is_jni_primitive<ReturnType>::value, "Unsupported return type");
    }
}

template<typename ReturnType, typename... Args>
ReturnType
JniObject::callStaticMethod(const std::string &className, const std::string &methodName, const std::string &methodSig,
                            Args... args) {
    static_assert(is_jni_primitive<ReturnType>::value, "Unsupported return type");
    JNIEnv *env = JniEnvironment::getInstance().getEnv();

    jclass clazz = env->FindClass(className.c_str());
    if (!clazz) {
        throw std::runtime_error("Class not found: " + className);
    }

    jmethodID methodID = env->GetStaticMethodID(clazz, methodName.c_str(), methodSig.c_str());
    if (!methodID) {
        throw std::runtime_error("Static method not found: " + className + methodName + methodSig);
    }

    if constexpr (std::is_same_v<ReturnType, void>) {
        env->CallStaticVoidMethod(clazz, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jint>) {
        return env->CallStaticIntMethod(clazz, methodID, args...);
    } else if constexpr (std::is_same_v<ReturnType, jstring>) {
        return static_cast<ReturnType>(env->CallStaticObjectMethod(clazz, methodID, args...));
    } else {
        static_assert(is_jni_primitive<ReturnType>::value, "Unsupported return type");
    }

    env->DeleteLocalRef(clazz);
}


#endif //MY_APPLICATION_JNIOBJECT_H
