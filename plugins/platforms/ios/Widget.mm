#import <UIKit/UIKit.h>
#include <objc/objc.h>
#include <objc/NSObjCRuntime.h>
#include <objc/message.h>
#include "Widget.h"
#include "ViewController.h"


@interface UIKitWindowObjC : UIWindow
@end

@implementation UIKitWindowObjC

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
    }
    return self;
}

@end

@interface UIKitViewObjC : UIView
- (instancetype)initWithWidget:(Widget *)widget;
@end

@implementation UIKitViewObjC {
    Widget *widget;
}

- (instancetype)initWithWidget:(Widget *)_widget {
    self = [super initWithFrame:CGRectMake(0, 0, 400, 400)];
    if (self) {
        widget = _widget;
        [self setupView];
    }
    return self;
}

- (void)setupView {
    self.backgroundColor = [UIColor whiteColor];
    self.opaque = NO;
    self.userInteractionEnabled = YES;
}

- (BOOL)isFlipped {
    return YES;
}

@end

struct UIKitWindowWrapper {
    UIKitWindowObjC *wrapped;
};

struct UIKitViewWrapper {
    UIKitViewObjC *wrapped;
};

Widget::Widget(Widget *_parent) {
    parent = _parent;
    window_wrapper = new UIKitWindowWrapper();
    view_wrapper = new UIKitViewWrapper();
    CGRect rect = CGRectMake(0, 0, 400, 400);
    window_wrapper->wrapped = [[UIKitWindowObjC alloc] initWithFrame:rect];
    [window_wrapper->wrapped setRootViewController:[[ViewController alloc] init]];
    [window_wrapper->wrapped makeKeyAndVisible];
    [window_wrapper->wrapped addSubview:view_wrapper->wrapped];

    if (parent) {
        [parent->view_wrapper->wrapped addSubview:view_wrapper->wrapped];
    } else {
        UIApplication *uiApplication = [UIApplication sharedApplication];
        id<UIApplicationDelegate> appDelegate = uiApplication.delegate;
        UIWindow *mainWindow = appDelegate.window;
        [mainWindow addSubview:view_wrapper->wrapped];
    }
}

Widget::~Widget() {
    if (window_wrapper) {
//        [window_wrapper->wrapped release];
        delete window_wrapper;
    }
}

const char *Widget::windowTitle() const {
    return "";
}

void Widget::setWindowTitle(const char *title) {
}

void Widget::resize(int width, int height) {
    CGRect frame = CGRectMake(0, 0, width, height);
    view_wrapper->wrapped.frame = frame;
}

void Widget::maximize() {
    CGRect mainScreenBounds = [UIScreen mainScreen].bounds;
    window_wrapper->wrapped.frame = mainScreenBounds;
}

void Widget::fullscreen() {
    window_wrapper->wrapped.frame = [UIScreen mainScreen].bounds;
}

void Widget::init_view() {
    UIKitViewObjC *view = [[UIKitViewObjC alloc] initWithWidget:this];
    view_wrapper->wrapped = view;
}

void Widget::move(int x, int y) {
    view_wrapper->wrapped.frame = CGRectMake(x, y, view_wrapper->wrapped.frame.size.width, view_wrapper->wrapped.frame.size.height);
}

void Widget::mousePressEvent() {

}

void Widget::resizeEvent() {

}

void Widget::update() {
    [view_wrapper->wrapped setNeedsDisplay];
}

float Widget::scaleFactor() {
    return [UIScreen mainScreen].scale;
}

