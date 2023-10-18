#include <metal_stdlib>

using namespace metal;

#include <simd/simd.h>

typedef enum VertexInputIndex
{
    VertexInputIndexVertices     = 0,
    VertexInputIndexViewportSize = 1,
    VertexInputIndexScaleFactor = 2,
} VertexInputIndex;

typedef struct
{
    vector_float2 position;
} Vertex;

typedef struct
{
    vector_float2 origin_position;
    vector_float2 viewport_size;
} RectVertexUniforms;

typedef struct
{
    vector_float2 origin_position;
    float border_width;
    vector_float4 background_color;
} RectFragmentUniforms;

struct RasterizerData
{
    float4 position [[position]];
};

vertex RasterizerData
rectVertexShader(uint vertexID [[vertex_id]],
             constant Vertex *vertices [[buffer(VertexInputIndexVertices)]],
             constant vector_uint2 *viewportSizePointer [[buffer(VertexInputIndexViewportSize)]],
            constant float *scaleFactorPointer [[buffer(VertexInputIndexScaleFactor)]],
             constant RectVertexUniforms &uniforms [[buffer(3)]])
{
    RasterizerData out;

    float2 pixelSpacePosition = vertices[vertexID].position.xy + uniforms.origin_position;

    // Apply the scale factor to the pixelSpacePosition
    pixelSpacePosition *= *scaleFactorPointer;

    out.position = vector_float4(0, 0, 0.0, 1.0);
    out.position.xy = pixelSpacePosition / uniforms.viewport_size * float2(2.0, -2.0) + float2(-1.0, 1);

    return out;
}

fragment float4 rectFragmentShader(RasterizerData in [[stage_in]],
                               constant RectFragmentUniforms &uniforms [[buffer(0)]])
{
    vector_float4 background_color = uniforms.background_color;
    return background_color;
}
