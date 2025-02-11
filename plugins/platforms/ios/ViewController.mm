#import "ViewController.h"

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];

    // Set the view's background color to white
    self.view.backgroundColor = [UIColor whiteColor];

    // Create a blue view
    UIView *blueView = [[UIView alloc] initWithFrame:CGRectMake(50, 100, 200, 200)];
    blueView.backgroundColor = [UIColor blueColor];

    // Add the blue view to the main view
    [self.view addSubview:blueView];
}


@end