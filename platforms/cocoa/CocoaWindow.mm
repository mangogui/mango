#import "CocoaWindow.h"
#import "Cocoa/Cocoa.h"
#import "MetalKit/MTKView.h"
#include <simd/simd.h>
#include "ShaderTypes.h"
#include "../../Rect.h"
#include <iostream>

const char* rect_source = R"(
#include <metal_stdlib>

using namespace metal;

#include <simd/simd.h>

typedef enum VertexInputIndex
{
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize = 1,
} VertexInputIndex;

typedef struct
{
    vector_float2 position;
} Vertex;

typedef struct
{
    vector_float2 origin_position;
    vector_float2 viewport_size;
    float width;
    float height;
} RectVertexUniforms;

typedef struct
{
    vector_float2 origin_position;
    float width;
    float height;
    float border_width;
    vector_float4 background_color;
} RectFragmentUniforms;

struct RasterizerData
{
    float4 position [[position]];
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant Vertex *vertices [[buffer(VertexInputIndexVertices)]],
             constant vector_uint2 *viewportSizePointer [[buffer(VertexInputIndexViewportSize)]],
             constant RectVertexUniforms &uniforms [[buffer(2)]])
{
    RasterizerData out;

    float2 pixelSpacePosition = vertices[vertexID].position.xy + uniforms.origin_position;

    out.position = vector_float4(0, 0, 0.0, 1.0);
    out.position.xy = pixelSpacePosition / uniforms.viewport_size * float2(2.0, -2.0) + float2(-1.0, 1);

    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant RectFragmentUniforms &uniforms [[buffer(0)]])
{
    vector_float4 background_color = uniforms.background_color;
    float border_width = uniforms.border_width;
    vector_float4 pos = in.position;
    float left = uniforms.origin_position.x + border_width;
    float right = uniforms.origin_position.x + uniforms.width - border_width;
    float top = uniforms.origin_position.y + border_width;
    float bottom = uniforms.origin_position.y + uniforms.height - border_width;
    if ((pos.x < left) or (pos.x > right))
        background_color = vector_float4(1.0, 0.4, 0.4, 1.0);
    if ((pos.y < top) or (pos.y > bottom))
        background_color = vector_float4(1.0, 0.4, 0.4, 1.0);
    return background_color;
}
)";

const char* ellipse_source = R"(
#include <metal_stdlib>

using namespace metal;

#include <simd/simd.h>

typedef enum VertexInputIndex
{
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize = 1,
} VertexInputIndex;

typedef struct
{
    vector_float2 position;
} Vertex;

typedef struct
{
    vector_float2 origin_position;
    vector_float2 viewport_size;
    float width;
    float height;
} EllipseVertexUniforms;

typedef struct
{
    vector_float2 origin_position;
    vector_float2 viewport_size;
    float width;
    float height;
    float border_width;
    vector_float4 background_color;
} EllipseFragmentUniforms;

struct RasterizerData
{
    float4 position [[position]];
    float2 center;
};

vertex RasterizerData
vertexShader(uint vertexID [[vertex_id]],
             constant Vertex *vertices [[buffer(VertexInputIndexVertices)]],
             constant EllipseVertexUniforms &uniforms [[buffer(2)]])
{
    RasterizerData out;

    float2 pixelSpacePosition = vertices[vertexID].position.xy + uniforms.origin_position;

    out.position = vector_float4(0, 0, 0.0, 1.0);
    out.position.xy = pixelSpacePosition / uniforms.viewport_size * float2(2.0, -2.0) + float2(-1.0, 1);
    out.center = uniforms.origin_position + float2(uniforms.width, uniforms.width) / 2.0;

    return out;
}

fragment float4 fragmentShader(RasterizerData in [[stage_in]],
                               constant EllipseFragmentUniforms &uniforms [[buffer(0)]])
{
    float fade = .01;
    float4 background_color = uniforms.background_color;
    float distance = 1 - length((in.position.xy - in.center) / float2(uniforms.width, uniforms.height) * 2.0);
    background_color.a *= smoothstep(0.0, fade, distance);
    return background_color;
}
)";

@interface WindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation WindowDelegate
- (void)windowDidResize:(NSNotification *)notification {

}
- (void)windowWillStartLiveResize:(NSNotification *)notification {

}
- (void)windowDidEndLiveResize:(NSNotification *)notification {

}
@end

@interface CocoaWindowObjC : NSWindow
@end

@implementation CocoaWindowObjC

- (BOOL)canBecomeKeyWindow { return YES; }

- (BOOL)canBecomeMainWindow { return YES; }

@end

@interface Renderer : NSObject<MTKViewDelegate>
@end

// Main class performing the rendering
@implementation Renderer
{
    id<MTLDevice> _device;

    id<MTLBuffer> fragmentUniformsBuffer;
    id<MTLBuffer> vertexUniformsBuffer;

    id<MTLBuffer> vertexBuffer;

    id<MTLRenderPipelineState> _pipelineState;

    id<MTLCommandQueue> _commandQueue;

    vector_float2 _viewportSize;

    matrix_float4x4 _projectionMatrix;

    @public GUI::CocoaWindow* window;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
{
    self = [super init];
    if(self)
    {
        _device = mtkView.device;
    }

    return self;
}

- (void)drawRectangle:(nonnull MTKView *)view {
    NSError *error;
    id<MTLLibrary> library = [_device newLibraryWithSource:@(rect_source) options:nullptr error:&error];
    if (library == nil) {
        NSLog(@"Error: failed to create Metal library: %@", error);
        return;
    }
    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    pipelineStateDescriptor.colorAttachments[0].blendingEnabled             = YES;
    pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation           = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation         = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor        = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor      = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

    _commandQueue = [_device newCommandQueue];
    GUI::Rect rect = GUI::Rect(0, 0, 400, 400);
    static const Vertex quadVertices[] =
    {
        { {  0, 0 } },
        { {rect.width(), 0 } },
        { {rect.width(),   rect.height() } },

        { {  0,            0 } },
        { {rect.width(),   rect.height() } },
        { {  0,            rect.height() } },
    };

    // Create a new command buffer for each render pass to the current drawable.
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    // Obtain a renderPassDescriptor generated from the view's drawable textures.
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil)
    {
        // Create a render command encoder.
        id<MTLRenderCommandEncoder> renderEncoder =
                [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        // Set the region of the drawable to draw into.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, static_cast<double>(_viewportSize.x), static_cast<double>(_viewportSize.y), 0.0, 1.0}];

        [renderEncoder setRenderPipelineState:_pipelineState];

        vertexUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
        fragmentUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];

        [renderEncoder setFragmentBuffer:fragmentUniformsBuffer offset:0 atIndex:0];

        vertexBuffer = [_device newBufferWithLength:sizeof(Vertex) options:MTLResourceStorageModeShared];

        RectFragmentUniforms fragmentUniforms = {
                .origin_position = simd_make_float2(rect.x(), rect.y()),
                .width = rect.width(),
                .height = rect.height(),
                .border_width = 20,
                .background_color = simd_make_float4(0.4, 1.0, 0.4, 1.0)
        };

        RectVertexUniforms vertexUniforms = {
                .origin_position = simd_make_float2(rect.x(), rect.y()),
                .viewport_size = _viewportSize,
                .width = rect.width(),
                .height = rect.height()
        };

        // Pass in the parameter data.
        [renderEncoder setVertexBytes:quadVertices
                               length:sizeof(quadVertices)
                              atIndex:VertexInputIndexVertices];

        [renderEncoder setVertexBytes:&_viewportSize
                               length:sizeof(_viewportSize)
                              atIndex:VertexInputIndexViewportSize];

        [renderEncoder setVertexBytes:&vertexUniforms
                               length:sizeof(vertexUniforms)
                              atIndex:2];

        [renderEncoder setFragmentBytes:&fragmentUniforms
                                 length:sizeof(fragmentUniforms)
                                atIndex:0];

        // Draw the triangle.
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:6];

        [renderEncoder endEncoding];

        // Schedule a present once the framebuffer is complete using the current drawable.
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    // Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
}

- (void)drawEllipse:(nonnull MTKView *)view {
    NSError *error;
    id<MTLLibrary> library = [_device newLibraryWithSource:@(ellipse_source) options:nullptr error:&error];
    if (library == nil) {
        NSLog(@"Error: failed to create Metal library: %@", error);
        return;
    }
    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"vertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"fragmentShader"];

    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat;
    pipelineStateDescriptor.colorAttachments[0].blendingEnabled             = YES;
    pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation           = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation         = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor        = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor      = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

    _commandQueue = [_device newCommandQueue];
    GUI::Rect rect = GUI::Rect(100, 100, 400, 400);
    static const Vertex quadVertices[] =
    {
            { {  0, 0 } },
            { {rect.width(), 0 } },
            { {rect.width(),   rect.height() } },

            { {  0,            0 } },
            { {rect.width(),   rect.height() } },
            { {  0,            rect.height() } },
    };

    // Create a new command buffer for each render pass to the current drawable.
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    // Obtain a renderPassDescriptor generated from the view's drawable textures.
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil)
    {
        // Create a render command encoder.
        id<MTLRenderCommandEncoder> renderEncoder =
                [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        // Set the region of the drawable to draw into.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, static_cast<double>(_viewportSize.x), static_cast<double>(_viewportSize.y), 0.0, 1.0}];

        [renderEncoder setRenderPipelineState:_pipelineState];

        vertexUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
        fragmentUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];

        [renderEncoder setFragmentBuffer:fragmentUniformsBuffer offset:0 atIndex:0];

        vertexBuffer = [_device newBufferWithLength:sizeof(Vertex) options:MTLResourceStorageModeShared];

        EllipseFragmentUniforms fragmentUniforms = {
                .origin_position = simd_make_float2(rect.x(), rect.y()),
                .viewport_size = _viewportSize,
                .width = rect.width(),
                .height = rect.height(),
                .border_width = 20,
                .background_color = simd_make_float4(0.4, 1.0, 0.4, 1.0)
        };

        EllipseVertexUniforms vertexUniforms = {
                .origin_position = simd_make_float2(rect.x(), rect.y()),
                .viewport_size = _viewportSize,
                .width = rect.width(),
                .height = rect.height()
        };

        // Pass in the parameter data.
        [renderEncoder setVertexBytes:quadVertices
                               length:sizeof(quadVertices)
                              atIndex:VertexInputIndexVertices];

        [renderEncoder setVertexBytes:&_viewportSize
                               length:sizeof(_viewportSize)
                              atIndex:VertexInputIndexViewportSize];

        [renderEncoder setVertexBytes:&vertexUniforms
                               length:sizeof(vertexUniforms)
                              atIndex:2];

        [renderEncoder setFragmentBytes:&fragmentUniforms
                                 length:sizeof(fragmentUniforms)
                                atIndex:0];

        // Draw the triangle.
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:6];

        [renderEncoder endEncoding];

        // Schedule a present once the framebuffer is complete using the current drawable.
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    // Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    [self drawEllipse:view];
    window->paintEvent();
}


/// Called whenever view changes orientation or is resized
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = 800;
    _viewportSize.y = 800;
}

@end


namespace GUI {

    struct CocoaWindowWrapper {
        CocoaWindowObjC *wrapped;
    };

    CocoaWindow::CocoaWindow() : wrapper(new CocoaWindowWrapper()) {
        @autoreleasepool {
            NSRect rect = NSMakeRect(0, 0, 400, 400);
            NSWindowStyleMask styleMask = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable
                    | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
            wrapper->wrapped = [[CocoaWindowObjC alloc] initWithContentRect:rect
                                                                  styleMask:styleMask
                                                                    backing:NSBackingStoreBuffered
                                                                      defer:NO];
            WindowDelegate *windowDelegate = [[WindowDelegate alloc] init];
            [wrapper->wrapped setDelegate:windowDelegate];
            [wrapper->wrapped setShowsResizeIndicator:YES];
            [wrapper->wrapped setAcceptsMouseMovedEvents:YES];
            [wrapper->wrapped setLevel:NSNormalWindowLevel];
            [wrapper->wrapped setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary |
                                                    NSWindowCollectionBehaviorManaged];
            [wrapper->wrapped setTitle:@"MainWindow"];

            initMTKView();
        }
    }

    CocoaWindow::~CocoaWindow() {
        @autoreleasepool {
            if (wrapper)
                [wrapper->wrapped release];
            delete wrapper;
        }
    }

    void CocoaWindow::center() {
        @autoreleasepool {
            [wrapper->wrapped center];
        }
    }

    void CocoaWindow::display() {
        @autoreleasepool {
            [wrapper->wrapped display];
            [wrapper->wrapped orderFrontRegardless];
        }
    }

    const char *CocoaWindow::windowTitle() const {
        @autoreleasepool {
            const char *title = [[wrapper->wrapped title] UTF8String];
            return title;
        }
    }

    void CocoaWindow::setWindowTitle(const char *title) {
        @autoreleasepool {
            [wrapper->wrapped setTitle:@(title)];
        }
    }

    void CocoaWindow::resize(int width, int height) {
        @autoreleasepool {
            NSRect frame = [wrapper->wrapped frame];
            frame.size.width = width;
            frame.size.height = height;
            [wrapper->wrapped setFrame:frame display:YES];
        }
    }

    Size CocoaWindow::size() const {
        @autoreleasepool {
            NSRect frame = [wrapper->wrapped frame];
            Size _size = Size(frame.size.width, frame.size.height);
            return _size;
        }
    }

    void CocoaWindow::maximize() {
        @autoreleasepool {
            [wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
        }
    }

    void CocoaWindow::fullscreen() {
        @autoreleasepool {
            [wrapper->wrapped setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
            [wrapper->wrapped toggleFullScreen:wrapper->wrapped];
        }
    }

    void CocoaWindow::initMTKView() {
        @autoreleasepool {
            MTKView *mtkView = [[MTKView alloc] init];
            mtkView.enableSetNeedsDisplay = YES;
            mtkView.device = MTLCreateSystemDefaultDevice();
            mtkView.clearColor = MTLClearColorMake(0.2, 0.2, 0.2, 1.0);
            Renderer *renderer = [[Renderer alloc] initWithMetalKitView:mtkView];
            renderer->window = this;
            [mtkView setDelegate:renderer];
            [wrapper->wrapped setContentView:mtkView];
            [wrapper->wrapped makeFirstResponder:mtkView];
        }
    }

    void CocoaWindow::paintEvent() {
        // std::cout << "paintEvent" << std::endl;
    }
}
