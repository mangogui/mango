#include "Renderer.h"
#include "PainterPath.h"
#include <stdio.h>

// Main class performing the rendering
@implementation Renderer
{
    id<MTLDevice> _device;

    id<MTLBuffer> fragmentUniformsBuffer;
    id<MTLBuffer> vertexUniformsBuffer;
    id<MTLBuffer> indexBuffer;

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
        mtkView.sampleCount = 4;

        pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.sampleCount = mtkView.sampleCount;
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

- (void)drawPolygonInMTKView:(nonnull MTKView *)view withPoints:(std::vector<GUI::Point>)points {
    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"rectVertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"rectFragmentShader"];

    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;

    NSError *error;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

    _commandQueue = [_device newCommandQueue];

    const size_t numPoints = points.size();
    Vertex polygonVertices[numPoints];
    uint16_t polygonIndices[(numPoints - 2) * 3];

    for (size_t i = 0; i < numPoints; i++) {
        const GUI::Point& point = points[i];
        polygonVertices[i] = {{ point.x(), point.y() }};
    }

    size_t index = 0;
    for (size_t i = 1; i < numPoints - 1; i++) {
        polygonIndices[index++] = 0;
        polygonIndices[index++] = static_cast<uint16_t>(i);
        polygonIndices[index++] = static_cast<uint16_t>(i + 1);
    }

    // Create a new command buffer for each render pass to the current drawable.
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    // Obtain a renderPassDescriptor generated from the view's drawable textures.
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if (renderPassDescriptor != nil) {
        // Create a render command encoder.
        id<MTLRenderCommandEncoder> renderEncoder =
                [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        // Set the region of the drawable to draw into.
        [renderEncoder setViewport:(MTLViewport){0.0, 0.0, static_cast<double>(_viewportSize.x), static_cast<double>(_viewportSize.y), 0.0, 1.0}];

        [renderEncoder setRenderPipelineState:_pipelineState];

        // Create and set the index buffer.
        id<MTLBuffer> indexBuffer = [_device newBufferWithBytes:polygonIndices length:sizeof(polygonIndices) options:MTLResourceStorageModeShared];
        [renderEncoder setVertexBuffer:indexBuffer offset:0 atIndex:1];

        vertexUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
        fragmentUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
        [renderEncoder setFragmentBuffer:fragmentUniformsBuffer offset:0 atIndex:0];

        vertexBuffer = [_device newBufferWithBytes:polygonVertices length:sizeof(polygonVertices) options:MTLResourceStorageModeShared];

        RectFragmentUniforms fragmentUniforms = {
                .origin_position = simd_make_float2(0, 0),
                .border_width = 0,
                .background_color = simd_make_float4(0.4, 1.0, 0.4, 1.0)
        };

        RectVertexUniforms vertexUniforms = {
                .origin_position = simd_make_float2(0, 0),
                .viewport_size = _viewportSize,
        };

        // Pass in the parameter data.
        [renderEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:VertexInputIndexVertices];

        [renderEncoder setVertexBytes:&_viewportSize
                               length:sizeof(_viewportSize)
                              atIndex:VertexInputIndexViewportSize];

        [renderEncoder setVertexBytes:&vertexUniforms
                               length:sizeof(vertexUniforms)
                              atIndex:2];

        [renderEncoder setFragmentBytes:&fragmentUniforms
                                 length:sizeof(fragmentUniforms)
                                atIndex:0];

        // Draw the polygon using indexed drawing.
        [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                  indexCount:sizeof(polygonIndices) / sizeof(uint16_t)
                                   indexType:MTLIndexTypeUInt16
                                 indexBuffer:indexBuffer
                           indexBufferOffset:0];

        [renderEncoder endEncoding];

        // Schedule a present once the framebuffer is complete using the current drawable.
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    // Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    _window->paintEvent();
    GUI::PainterPath& path = _window->painterPath();
    std::vector<GUI::Point> points;
    for (int i = 0; i < path.elementCount(); i++) {
        GUI::PainterPath::Element element = path.elementAt(i);
        if (element.isLineTo())
            points.push_back(GUI::Point(element.x, element.y));
    }
    path.clear();

    [self drawPolygonInMTKView:view withPoints:points];
}


/// Called whenever view changes orientation or is resized
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

@end