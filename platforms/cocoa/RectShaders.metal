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
