#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include <simd/simd.h>
#include "ShaderTypes.h"
#include "../../Rect.h"
#import "CocoaWindow.h"


@interface Renderer : NSObject<MTKViewDelegate>
@property (readwrite) GUI::CocoaWindow* window;
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
- (void)drawPolygonInMTKView:(nonnull MTKView *)view withPoints:(std::vector<GUI::Point>)points;
@end
