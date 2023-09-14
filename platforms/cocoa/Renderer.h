#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include <simd/simd.h>
#include "ShaderTypes.h"
#include "../../Rect.h"
#import "Widget.h"


@interface Renderer : NSObject<MTKViewDelegate>
@property (readwrite) GUI::Widget* window;
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView;
- (void)drawPolygonInMTKView:(nonnull MTKView *)view withPolygons:(std::vector<std::vector<GUI::Point>>)polygons andColors:(std::map<int, GUI::Color>)colors;
@end
