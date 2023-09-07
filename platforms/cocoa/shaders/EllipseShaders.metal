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
    float width;
    float height;
    float border_width;
    vector_float4 background_color;
} EllipseFragmentUniforms;
struct RasterizerData
{
    float4 position [[position]];
    float2 pixel_position [[pixel_position]];
    float2 center;
    float scale_factor;
};
vertex RasterizerData
ellipseVertexShader(uint vertexID [[vertex_id]],
             constant Vertex *vertices [[buffer(VertexInputIndexVertices)]],
             constant EllipseVertexUniforms &uniforms [[buffer(2)]])
{
    float scale_factor = 2.0;

    float2 pixel_position = vertices[vertexID].position.xy + uniforms.origin_position;
    float2 device_position = pixel_position / uniforms.viewport_size * scale_factor * scale_factor * float2(1.0, -1.0) + float2(-1.0, 1.0);

    RasterizerData out;
    out.position = float4(device_position, 0.0, 1.0);
    out.pixel_position = pixel_position;
    out.center = uniforms.origin_position + float2(uniforms.width, uniforms.height) / 2;
    out.scale_factor = scale_factor;

    return out;
}
fragment float4 ellipseFragmentShader(RasterizerData in [[stage_in]],
                               constant EllipseFragmentUniforms &uniforms [[buffer(0)]])
{
    float4 background_color = uniforms.background_color;
    float distance = 1.0 - length((in.pixel_position.xy - in.center) / float2(uniforms.width, uniforms.height) * in.scale_factor);
    float fade = fwidth(distance) * 2.0;
    background_color.a *= smoothstep(0.0, fade, distance);
    return background_color;
}