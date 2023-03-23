#include "Renderer.h"
#include "PainterPath.h"
#include <stdio.h>

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

    MTLRenderPipelineDescriptor *pipelineStateDescriptor;

    matrix_float4x4 _projectionMatrix;
    id<MTLLibrary> library;
}

- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
{
    self = [super init];
    if(self)
    {
        _device = mtkView.device;

        pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;
        pipelineStateDescriptor.colorAttachments[0].blendingEnabled             = YES;
        pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation           = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation         = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor        = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor      = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;

        NSError *error;
        library = [_device newLibraryWithFile:@"Shaders.metallib" error:&error];
        if (library == nil) {
            NSLog(@"Error: failed to create Metal library: %@", error);
            return self;
        }
    }

    return self;
}

- (void)drawRectangle:(nonnull MTKView *)view rect:(GUI::Rect&)rect;{
    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"rectVertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"rectFragmentShader"];

    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;

    NSError *error;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

    _commandQueue = [_device newCommandQueue];
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

- (void)render:(nonnull MTKView *)view {
    _commandQueue = [_device newCommandQueue];

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

        vertexUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
        fragmentUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];

        [renderEncoder setFragmentBuffer:fragmentUniformsBuffer offset:0 atIndex:0];

        vertexBuffer = [_device newBufferWithLength:sizeof(Vertex) options:MTLResourceStorageModeShared];

        for (int i=0; i<_window->painterPath().elementCount(); i++) {
            if (i+4 < _window->painterPath().elementCount()) {
                int curve_count = 0;
                for (int j = 0; j < 4; j++) {
                    if (_window->painterPath().elementAt(i + j + 1).isCurveTo()) curve_count++;
                }
                if (curve_count == 4 and
                    _window->painterPath().elementAt(i).x == _window->painterPath().elementAt(i + 4).x
                    and _window->painterPath().elementAt(i).y == _window->painterPath().elementAt(i + 4).y) {

                    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"ellipseVertexShader"];
                    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"ellipseFragmentShader"];

                    pipelineStateDescriptor.label = @"Simple Pipeline";
                    pipelineStateDescriptor.vertexFunction = vertexFunction;
                    pipelineStateDescriptor.fragmentFunction = fragmentFunction;

                    NSError *error;
                    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

                    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

                    [renderEncoder setRenderPipelineState:_pipelineState];

                    float x = _window->painterPath().elementAt(i).x;
                    float y = _window->painterPath().elementAt(i).y;
                    float width = _window->painterPath().elementAt(i + 2).x - _window->painterPath().elementAt(i).x;
                    float height = _window->painterPath().elementAt(i + 2).y - _window->painterPath().elementAt(i).y;
                    GUI::Rect rect = GUI::Rect(x, y, width, height);

                    EllipseFragmentUniforms fragmentUniforms = {
                            .origin_position = simd_make_float2(rect.x(), rect.y()),
                            .width = rect.width(),
                            .height = rect.height(),
                            .border_width = 20,
                            .background_color = _window->fillColorAtIndex(i).toSimdFloat4()
                    };

                    EllipseVertexUniforms vertexUniforms = {
                            .origin_position = simd_make_float2(rect.x(), rect.y()),
                            .viewport_size = _viewportSize,
                            .width = rect.width(),
                            .height = rect.height()
                    };

                    static const Vertex quadVertices[] =
                            {
                                    {{0,            0}},
                                    {{rect.width(), 0}},
                                    {{rect.width(), rect.height()}},

                                    {{0,            0}},
                                    {{rect.width(), rect.height()}},
                                    {{0,            rect.height()}},
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

                    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                                      vertexStart:0
                                      vertexCount:6];
                }
            }
            if (i+4 < _window->painterPath().elementCount()) {
                int line_count = 0;
                for (int j = 0; j < 4; j++) {
                    if (_window->painterPath().elementAt(i + j + 1).isLineTo()) line_count++;
                }
                if (line_count == 4 and
                    _window->painterPath().elementAt(i).x == _window->painterPath().elementAt(i + 4).x
                    and _window->painterPath().elementAt(i).y == _window->painterPath().elementAt(i + 4).y) {

                    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"rectVertexShader"];
                    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"rectFragmentShader"];

                    pipelineStateDescriptor.label = @"Simple Pipeline";
                    pipelineStateDescriptor.vertexFunction = vertexFunction;
                    pipelineStateDescriptor.fragmentFunction = fragmentFunction;

                    NSError *error;
                    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

                    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

                    [renderEncoder setRenderPipelineState:_pipelineState];

                    float x = _window->painterPath().elementAt(i).x;
                    float y = _window->painterPath().elementAt(i).y;
                    float width = _window->painterPath().elementAt(i + 2).x - _window->painterPath().elementAt(i).x;
                    float height = _window->painterPath().elementAt(i + 2).y - _window->painterPath().elementAt(i).y;
                    GUI::Rect rect = GUI::Rect(x, y, width, height);

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

                    static const Vertex quadVertices[] =
                            {
                                    {{0,            0}},
                                    {{rect.width(), 0}},
                                    {{rect.width(), rect.height()}},

                                    {{0,            0}},
                                    {{rect.width(), rect.height()}},
                                    {{0,            rect.height()}},
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
                }
            }
        }

        [renderEncoder endEncoding];

        [commandBuffer presentDrawable:view.currentDrawable];
    }

    [commandBuffer commit];
}

- (void)drawInMTKView:(nonnull MTKView *)view
{
    _window->paintEvent();
    [self render:view];
    _window->painterPath().clear();
    _window->states().clear();
}

/// Called whenever view changes orientation or is resized
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

@end