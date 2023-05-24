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
             constant RectVertexUniforms &uniforms [[buffer(2)]])
{
    RasterizerData out;

    float2 pixelSpacePosition = vertices[vertexID].position.xy + uniforms.origin_position;

    float2 pixelSize = 1.0 / uniforms.viewport_size;
    float2 screenSize = float2(viewportSizePointer[0].x, viewportSizePointer[0].y);

    // Adjust pixelSpacePosition based on DPI
    pixelSpacePosition *= screenSize * pixelSize;

    // Convert pixelSpacePosition to normalized device coordinates
    out.position = float4(pixelSpacePosition * 2.0 - 1.0, 0.0, 1.0);

    return out;
}

fragment float4 rectFragmentShader(RasterizerData in [[stage_in]],
                               constant RectFragmentUniforms &uniforms [[buffer(0)]])
{
    vector_float4 background_color = uniforms.background_color;
    return background_color;
}
