#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include <simd/simd.h>
#include "ShaderTypes.h"
#include "../../Rect.h"
#import "Widget.h"


@interface Renderer : NSObject<MTKViewDelegate>
@property (readwrite) GUI::Widget* widget;
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
- (void)drawPolygonInMTKView:(nonnull MTKView *)view withPoints:(std::vector<GUI::Point>)points andColors:(std::map<int, GUI::Color>)colors;
@end
