#import "WindowDelegate.h"
#include "CocoaWindowObjC.h"
#include <ResizeEvent.h>

@implementation WindowDelegate
- (void)windowDidResize:(NSNotification *)notification {
    CocoaWindowObjC *window = notification.object;
    NSSize newSize = window.frame.size;
    auto widget = window.widget;

    if (widget) {
        ResizeEvent event(newSize.width, newSize.height);
        widget->resizeEvent(&event);
    }
}

- (NSSize) windowWillResize:(NSWindow *) sender
                     toSize:(NSSize) frameSize {
    return frameSize;
}

- (void)windowWillStartLiveResize:(NSNotification *)notification {

}

- (void)windowDidEndLiveResize:(NSNotification *)notification {

}
@end