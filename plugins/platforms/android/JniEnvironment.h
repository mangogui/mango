#ifndef MY_APPLICATION_JNIENVIRONMENT_H
#define MY_APPLICATION_JNIENVIRONMENT_H

#include "jni.h"

class JniEnvironment {
public:
    static JniEnvironment &getInstance() {
        static JniEnvironment instance;
        return instance;
    }

    void initialize(JNIEnv *env) {
        m_env = env;
    }

    [[nodiscard]] JNIEnv *getEnv() const {
        return m_env;
    }

    JniEnvironment() = default;

    JniEnvironment(const JniEnvironment &) = delete;

    JniEnvironment &operator=(const JniEnvironment &) = delete;

private:
    JNIEnv *m_env;
};

#endif //MY_APPLICATION_JNIENVIRONMENT_H
