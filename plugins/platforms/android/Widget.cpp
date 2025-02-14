#include "Widget.h"
#include "AndroidActivity.h"
#include "JniEnvironment.h"
#include <jni.h>
#include <android/log.h>


JniObject getActivityObject() {
    return AndroidActivity::getInstance().getActivity();
}

JniObject getDecorViewObject(const JniObject &activityObject) {
    JniObject windowObject = activityObject.callMethod<jobject>("getWindow", "()Landroid/view/Window;");
    return windowObject.callMethod<jobject>("getDecorView", "()Landroid/view/View;");
}

Widget::Widget(Widget *parent)
        : m_parent(parent), m_viewObject(nullptr) {
    createView(parent);
}

Widget::~Widget() {
    for (Widget *child: m_children) {
        delete child;
    }
    delete m_viewObject;
}

void Widget::handleMouseEvent(const MouseEvent &event) {
    LOGI("handleMouseEvent: Type: %u, Coordinates: %.2f, %.2f", event.type, event.x, event.y);
}

void Widget::createView() {
    JniObject activityObject = getActivityObject();
    JniObject decorViewObject = getDecorViewObject(activityObject);

    if (!decorViewObject.isInstanceOf("android/view/ViewGroup")) {
        LOGI("decorViewObject is not a ViewGroup");
        return;
    }

    this->m_viewObject = new JniObject(
            "android/widget/RelativeLayout",
            "(Landroid/content/Context;)V",
            activityObject.getObject()
    );

    this->setBackgroundColor("#D9D9D9");

    decorViewObject.callMethod<void>("addView", "(Landroid/view/View;)V",
                                     m_viewObject->getObject());
}

void Widget::createView(Widget *parent) {
    if (parent == nullptr) {
        return createView();
    }

    JniObject *parentViewObject = parent->m_viewObject;

    if (!parentViewObject->isInstanceOf("android/view/ViewGroup")) {
        LOGI("parentViewObject is not a ViewGroup");
        return;
    }

    JniObject activityObject = AndroidActivity::getInstance().getActivity();

    this->m_viewObject = new JniObject(
            "android/view/View",
            "(Landroid/content/Context;)V",
            activityObject.getObject()
    );

    parentViewObject->callMethod<void>("addView", "(Landroid/view/View;)V",
                                       m_viewObject->getObject());

    parent->m_children.push_back(this);

    this->setBackgroundColor("#568CF6");
}


void Widget::resize(int width, int height) {
    if (!m_parent) {
        return;
    }
    JniObject layoutParams("android/widget/RelativeLayout$LayoutParams", "(II)V", width, height);
    m_viewObject->callMethod<void>("setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V",
                                   layoutParams.getObject());

    LOGI("Size set to (width: %d, height: %d)", width, height);
}

void Widget::move(int x, int y) {
    if (!m_parent) {
        return;
    }

    JniObject layoutParamsObject = m_viewObject->callMethod<jobject>("getLayoutParams",
                                                                     "()Landroid/view/ViewGroup$LayoutParams;");

    layoutParamsObject.callMethod<void>("setMargins", "(IIII)V", x, y, 0, 0);

    m_viewObject->callMethod<void>("setLayoutParams", "(Landroid/view/ViewGroup$LayoutParams;)V",
                                   layoutParamsObject.getObject());

    LOGI("Position set to (x: %d, y: %d)", x, y);
}

void Widget::setBackgroundColor(const std::string &colorString) {
    jstring jColorString = JniEnvironment::getInstance().getEnv()->NewStringUTF(
            colorString.c_str());
    jint color = JniObject::callStaticMethod<jint>("android/graphics/Color", "parseColor",
                                                   "(Ljava/lang/String;)I", jColorString);
    m_viewObject->callMethod<void>("setBackgroundColor", "(I)V", color);
}
