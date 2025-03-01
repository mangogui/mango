#import "ViewObjC.h"

@implementation ViewObjC {
    Widget *widget;
}

- (NSView *)initWithWidget:(Widget *)_widget {
    self = [super init];

    widget = _widget;

    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect]; // Call the super implementation

    CGContextRef context = [[NSGraphicsContext currentContext] CGContext];
    if (!context) {
        NSLog(@"ViewObjC: CGContext is null");
        return;
    }

    widget->setCGContextRef(context);

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

- (void)setBackgroundColor:(NSColor *)color {
    if ([self wantsLayer]) {
        [self.layer setBackgroundColor:[color CGColor]];
    } else {
        [self setWantsLayer:YES];
        [self.layer setBackgroundColor:[color CGColor]];
    }
}
@end