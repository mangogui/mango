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

    PaintEvent event;
    widget->paintEvent(event); // Call the paintEvent method
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
    MouseEvent mouseEvent{};
    MouseEvent::Type type = MouseEvent::LeftButtonUp;
    if (eventType == NSEventTypeLeftMouseUp) {
        type = MouseEvent::Type::LeftButtonUp;
    }
    mouseEvent.setType(type);
    widget->mouseReleaseEvent(&mouseEvent);
}

- (void)mouseDown:(NSEvent *)event {
    NSEventType eventType = [event type];
    MouseEvent mouseEvent{};
    MouseEvent::Type type = MouseEvent::LeftButtonDown;
    if (eventType == NSEventTypeLeftMouseDown) {
        type = MouseEvent::Type::LeftButtonDown;
    }
    mouseEvent.setType(type);
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