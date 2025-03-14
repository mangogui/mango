#import "ViewObjC.h"

@implementation ViewObjC {
    AbstractWidget *widget;
}

- (NSView *)initWithWidget:(AbstractWidget *)_widget {
    self = [super init];

    widget = _widget;

    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect]; // Call the super implementation

    NSGraphicsContext *graphicsContext = [NSGraphicsContext currentContext];
    if (!graphicsContext) {
        NSLog(@"ViewObjC: NSGraphicsContext is null, window might be invalid.");
        return;
    }

    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    if (!context) {
        NSLog(@"ViewObjC: CGContext is null");
        return;
    }

    widget->setNativeContext(context);
    PaintEvent event;
    widget->paintEvent(&event); // Call the paintEvent method
}

- (BOOL)isFlipped { return YES; }

- (void)awakeFromNib {
    [super awakeFromNib];

    // Define the tracking area options
    NSTrackingAreaOptions trackingOptions = NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow;

    // Create _a tracking area
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                                options:trackingOptions
                                                                  owner:self
                                                               userInfo:nil];

    // Add the tracking area to the view
    [self addTrackingArea:trackingArea];
}

- (void)updateTrackingAreas {
    [super updateTrackingAreas];

    // Remove any existing tracking areas
    for (NSTrackingArea *trackingArea in [self trackingAreas]) {
        [self removeTrackingArea:trackingArea];
    }

    // Create _a new tracking area for the entire view
    NSTrackingAreaOptions trackingOptions = (NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow);
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:self.bounds
                                                                options:trackingOptions
                                                                  owner:self
                                                               userInfo:nil];

    // Add the tracking area to the view
    [self addTrackingArea:trackingArea];
}

- (void)mouseEntered:(NSEvent *)event {
    NSEventType eventType = [event type];
}

- (void)mouseUp:(NSEvent *)event {
    NSEventType eventType = [event type];
    MouseEvent mouseEvent(MouseEvent::LeftButtonUp);
    widget->mouseReleaseEvent(&mouseEvent);
}

- (void)mouseDown:(NSEvent *)event {
    NSEventType eventType = [event type];
    MouseEvent mouseEvent(MouseEvent::LeftButtonDown);
    widget->mousePressEvent(&mouseEvent);
}

- (void)mouseMoved:(NSEvent *)event {
//    NSEventType eventType = [event type];
//    MouseEvent mouseEvent(MouseEvent::MouseMove);
//    widget->mouseMoveEvent(&mouseEvent);
}
@end