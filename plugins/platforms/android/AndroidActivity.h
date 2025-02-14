#ifndef MY_APPLICATION_ANDROIDACTIVITY_H
#define MY_APPLICATION_ANDROIDACTIVITY_H

#include "jni.h"

class AndroidActivity {
public:
    static AndroidActivity &getInstance() {
        static AndroidActivity instance;
        return instance;
    }

    void initialize(jobject activity) {
        m_activity = activity;
    }

    jobject getActivity() const {
        return m_activity;
    }

private:
    AndroidActivity() = default;

    AndroidActivity(const AndroidActivity &) = delete;

    AndroidActivity &operator=(const AndroidActivity &) = delete;

    jobject m_activity;
};


#endif //MY_APPLICATION_ANDROIDACTIVITY_H
