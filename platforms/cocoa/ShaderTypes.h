/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Header containing types and enum constants shared between Metal shaders and C/ObjC rect_source
*/

#ifndef ShaderTypes_h
#define ShaderTypes_h

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

#endif /* ShaderTypes_h */
