#import "SceneDelegate.h"
#import "ViewController.h"

@implementation SceneDelegate


- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions {
    if (![scene isKindOfClass:[UIWindowScene class]]) {
        return;
    }

    UIWindowScene *windowScene = (UIWindowScene *)scene;
    self.window = [[UIWindow alloc] initWithFrame:windowScene.coordinateSpace.bounds];
    self.window.windowScene = windowScene;

//    ViewController *viewController = [[ViewController alloc] init];
//    self.window.rootViewController = viewController;

    [self.window makeKeyAndVisible];
}

@end