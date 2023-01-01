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
