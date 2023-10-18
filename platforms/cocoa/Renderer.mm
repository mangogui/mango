#include "Renderer.h"
#include "PainterPath.h"
#include <stdio.h>
#include <cmath>
#include "../../metal_config.h"


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

class Vector {
public:
    Vector(float x, float y) : x_(x), y_(y) {}

    float getX() const { return x_; }
    float getY() const { return y_; }

    Vector operator-(const Vector& other) const {
        return Vector(x_ - other.x_, y_ - other.y_);
    }

    float dot(const Vector& other) const {
        return x_ * other.x_ + y_ * other.y_;
    }

private:
    float x_;
    float y_;
};

bool inTriangle(const GUI::Point& pointToCheck, const GUI::Point& earTip, const GUI::Point& earTipPlusOne, const GUI::Point& earTipMinusOne) {
    if( (pointToCheck.x() == earTip.x() && pointToCheck.y() == earTip.y()) ||
        (pointToCheck.x() == earTipPlusOne.x() && pointToCheck.y() == earTipPlusOne.y()) ||
        (pointToCheck.x() == earTipMinusOne.x() && pointToCheck.y() == earTipMinusOne.y()) )
        return false;

    Vector v0 = Vector((earTipMinusOne.x() - earTip.x()), (earTipMinusOne.y() - earTip.y()));
    Vector v1 = Vector((earTipPlusOne.x() - earTip.x()), (earTipPlusOne.y() - earTip.y()));
    Vector v2 = Vector((pointToCheck.x() - earTip.x()), (pointToCheck.y() - earTip.y()));

    float u = (v1.dot(v1) * v2.dot(v0) - v1.dot(v0) * v2.dot(v1)) / (v0.dot(v0) * v1.dot(v1) - v0.dot(v1) * v1.dot(v0));
    float v = (v0.dot(v0) * v2.dot(v1) - v0.dot(v1) * v2.dot(v0)) / (v0.dot(v0) * v1.dot(v1) - v0.dot(v1) * v1.dot(v0));

    if (u < 0 || v < 0 || u > 1 || v > 1 || (u + v) > 1)
        return false;

    return true;
}

// Function to check if a vertex is an ear.
bool IsEar(const GUI::Point& prev, const GUI::Point& curr, const GUI::Point& next, const std::vector<GUI::Point>& polygon) {
    // Check if no other vertex lies inside the triangle formed by 'prev', 'curr', and 'next'.
    for (const GUI::Point& p : polygon) {
        if (inTriangle(p, prev, curr, next)) {
            return false;
        }
    }

    return true;
}

std::vector<int> EarClipping(const std::vector<GUI::Point>& polygon) {
    int n = polygon.size();
    std::vector<int> indices(n);
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    std::vector<int> triangleIndices;

    while (n > 3) {
        bool earFound = false;
        for (int i = 0; i < n; i++) {
            int prev = indices[(i + n - 1) % n];
            int curr = indices[i];
            int next = indices[(i + 1) % n];

            bool isEar = IsEar(polygon[prev], polygon[curr], polygon[next], polygon);
            if (isEar) {
                earFound = true;
                triangleIndices.push_back(prev);
                triangleIndices.push_back(curr);
                triangleIndices.push_back(next);

                indices.erase(indices.begin() + i);
                n--;
                break;
            }
        }

        if (!earFound) {
            // No ear was found in this iteration, terminate the loop.
            break;
        }
    }

    // Add the last triangle (final ear).
    for (int i : indices) {
        triangleIndices.push_back(i);
    }

    return triangleIndices;
}


- (void)drawPolygonInMTKView:(nonnull MTKView *)view withPolygons:(std::vector<std::vector<GUI::Point>>)polygons andColors:(std::map<int, GUI::Color>)colors {
    id<MTLFunction> vertexFunction = [library newFunctionWithName:@"rectVertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName:@"rectFragmentShader"];

    pipelineStateDescriptor.vertexFunction = vertexFunction;
    pipelineStateDescriptor.fragmentFunction = fragmentFunction;

    NSError *error;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&error];

    NSAssert(_pipelineState, @"Failed to create pipeline state: %@", error);

    _commandQueue = [_device newCommandQueue];

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

        for (std::vector<GUI::Point>& points: polygons) {
            GUI::Color current_color;
            int polygon_index = 0;

            if (colors.find(polygon_index) != colors.end()) {
                current_color = colors[polygon_index];
            }
            polygon_index += 1;

            // Triangulate the polygon using ear clipping
            std::vector<int> triangleIndices = EarClipping(points);

            const size_t numPoints = points.size();
            const size_t numTriangles = triangleIndices.size() / 3;

            Vertex polygonVertices[numPoints];
            uint16_t polygonIndices[numTriangles * 3];

            for (size_t i = 0; i < numPoints; i++) {
                const GUI::Point& point = points[i];
                polygonVertices[i] = {{ point.x(), point.y() }};
            }

            // Create and set the index buffer
            for (size_t i = 0; i < numTriangles * 3; i++) {
                polygonIndices[i] = static_cast<uint16_t>(triangleIndices[i]);
            }

            // Create and set the index buffer.
            id<MTLBuffer> indexBuffer = [_device newBufferWithBytes:polygonIndices length:sizeof(polygonIndices) options:MTLResourceStorageModeShared];
            [renderEncoder setVertexBuffer:indexBuffer offset:0 atIndex:1];

            float scaleFactor = _widget->scaleFactor();
            id<MTLBuffer> scaleFactorBuffer = [_device newBufferWithBytes:&scaleFactor length:sizeof(float) options:MTLResourceStorageModeShared];

            // Set the scale factor buffer in your Metal render pipeline
            [renderEncoder setVertexBuffer:scaleFactorBuffer offset:0 atIndex:2];
            [renderEncoder setVertexBytes:&scaleFactor length:sizeof(float) atIndex:2];

            vertexUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
            fragmentUniformsBuffer = [_device newBufferWithLength:sizeof(RectFragmentUniforms) options:MTLResourceStorageModeShared];
            [renderEncoder setFragmentBuffer:fragmentUniformsBuffer offset:0 atIndex:0];

            vertexBuffer = [_device newBufferWithBytes:polygonVertices length:sizeof(polygonVertices) options:MTLResourceStorageModeShared];

            RectFragmentUniforms fragmentUniforms = {
                    .origin_position = simd_make_float2(0, 0),
                    .border_width = 0,
                    .background_color = current_color.toSimdFloat4()
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
                                  atIndex:3];

            [renderEncoder setFragmentBytes:&fragmentUniforms
                                     length:sizeof(fragmentUniforms)
                                    atIndex:0];

            // Draw the polygon using indexed drawing.
            [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                      indexCount:sizeof(polygonIndices) / sizeof(uint16_t)
                                       indexType:MTLIndexTypeUInt16
                                     indexBuffer:indexBuffer
                               indexBufferOffset:0];

        }

        [renderEncoder endEncoding];


        // Schedule a present once the framebuffer is complete using the current drawable.
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    //     Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
}

void addLineElement(const GUI::PainterPath& path, std::vector<GUI::Point>& points, int current_element_index, float l) {
    GUI::PainterPath::Element element = path.elementAt(current_element_index);
    GUI::PainterPath::Element previous_element = path.elementAt(current_element_index-1);
    float a = std::atan2(element.y - previous_element.y, element.x - previous_element.x);
    float theta = a + M_PI * 3 / 2;
    GUI::Point p0(previous_element.x + l * std::cos(theta), previous_element.y + l * std::sin(theta));

    theta = a + M_PI / 2;
    GUI::Point p1(previous_element.x + l * std::cos(theta), previous_element.y + l * std::sin(theta));

    theta = a + M_PI * 1 / 2;
    GUI::Point p2{element.x + l * std::cos(theta), element.y + l * std::sin(theta)};

    theta = a + M_PI * 3 / 2;
    GUI::Point p3(element.x + l * std::cos(theta), element.y + l * std::sin(theta));

    points.push_back(p0);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
}


GUI::CubicBezierCurve create_cubic_bezier_curve(const GUI::PainterPath& path, int element_index) {
    GUI::CubicBezierCurve curve;
    for (int j = -1; j < 3; j++) {
        GUI::PainterPath::Element e = path.elementAt(element_index+j);
        curve.controlPoints[j+1] = {e.x, e.y};
    }
    return curve;
}

// Calculate the angle at a specific point on the Bezier curve
double calculateAngleAtPoint(double t, const GUI::CubicBezierCurve &curve) {
    GUI::Point P0 = curve.controlPoints[0];
    GUI::Point P1 = curve.controlPoints[1];
    GUI::Point P2 = curve.controlPoints[2];
    GUI::Point P3 = curve.controlPoints[3];

    // Calculate the derivative of the Bezier curve at t
    double dx = 3 * (1 - t) * (1 - t) * (P1.x() - P0.x()) + 6 * (1 - t) * t * (P2.x() - P1.x()) + 3 * t * t * (P3.x() - P2.x());
    double dy = 3 * (1 - t) * (1 - t) * (P1.y() - P0.y()) + 6 * (1 - t) * t * (P2.y() - P1.y()) + 3 * t * t * (P3.y() - P2.y());

    // Calculate the angle in radians
    double angle_radians = std::atan2(dy, dx);

    return angle_radians;
}

void addStrokedCurveElement(const GUI::PainterPath& path, std::vector<GUI::Point>& points, int current_element_index, float l) {
    // Get the control points for the cubic Bezier curve
    GUI::Point P0 = path.elementAt(current_element_index - 1).toPoint();
    GUI::Point P1 = path.elementAt(current_element_index).toPoint();
    GUI::Point P2 = path.elementAt(current_element_index + 1).toPoint();
    GUI::Point P3 = path.elementAt(current_element_index + 2).toPoint();

    GUI::CubicBezierCurve curve = create_cubic_bezier_curve(path, current_element_index);

    int stroke_width = 100;

    // Calculate the number of segments to approximate the curve
    int num_segments = 100; // You can adjust this based on your desired smoothness

    for (int i = 0; i <= num_segments; i++) {
        double t = static_cast<double>(i) / num_segments;

        // Calculate the point on the Bezier curve at t
        GUI::Point curve_point = GUI::PainterPath::pointAtBezierCurve(curve, t);

        // Calculate the angle at this point on the curve
        double angle_radians = calculateAngleAtPoint(t, curve);

        // Calculate the offset for the stroked curve
        double dx = stroke_width * std::cos(angle_radians - M_PI / 2);
        double dy = stroke_width * std::sin(angle_radians - M_PI / 2);

        points.push_back(GUI::Point(curve_point.x(), curve_point.y()));

        // Add the point on the stroked curve to the result path
        points.push_back(GUI::Point(curve_point.x() + dx, curve_point.y() + dy));
    }
}


std::vector<std::vector<GUI::Point>> buildPath(const GUI::PainterPath& path) {
    std::vector<std::vector<GUI::Point>> polygons;
    std::vector<GUI::Point> currentPolygon;
    int i = 0;

    while (i < path.elementCount()) {
        GUI::PainterPath::Element element = path.elementAt(i);

        switch (element.type) {
            case (GUI::PainterPath::LineTo): {
                currentPolygon.push_back(element.toPoint());
                i += 1;
                break;
            }
            case (GUI::PainterPath::CurveTo): {
                GUI::CubicBezierCurve curve = create_cubic_bezier_curve(path, i); // Initializing the curve's control points
                GUI::PainterPath::adaptiveApproximateCubicBezier(currentPolygon, curve);
                i += 3;
                break;
            }
            case (GUI::PainterPath::MoveTo): {
                if (!currentPolygon.empty()) {
                    // Remove consecutive equal points
                    std::vector<GUI::Point> filteredPoints;
                    if (!currentPolygon.empty()) {
                        filteredPoints.push_back(currentPolygon[0]); // Always add the first point

                        for (size_t j = 1; j < currentPolygon.size(); ++j) {
                            if (currentPolygon[j] != currentPolygon[j - 1]) {
                                filteredPoints.push_back(currentPolygon[j]);
                            }
                        }
                    }
                    polygons.push_back(filteredPoints);
                }
                currentPolygon.clear();
                currentPolygon.push_back(element.toPoint());
                i += 1;
                break;
            }
            case (GUI::PainterPath::CurveToData): {
                i += 1;
                break;
            }
        }
    }

    // Add the last polygon if not empty
    if (!currentPolygon.empty()) {
        std::vector<GUI::Point> filteredPoints;
        if (!currentPolygon.empty()) {
            filteredPoints.push_back(currentPolygon[0]); // Always add the first point

            for (size_t j = 1; j < currentPolygon.size(); ++j) {
                if (currentPolygon[j] != currentPolygon[j - 1]) {
                    filteredPoints.push_back(currentPolygon[j]);
                }
            }
        }
        polygons.push_back(filteredPoints);
    }

    return polygons;
}

- (void)drawInMTKView:(nonnull MTKView *)view {
    GUI::PaintEvent event;
    event.setRect(GUI::Rect(0, 0, _viewportSize.x, _viewportSize.y));
    _widget->paintEvent(event);

    GUI::PainterPath &path = _widget->painterPath();

    std::map<int, GUI::Color> colors;
    int polygon_index = 0;

    int i = 0;
    while (i < path.elementCount()) {
        GUI::PainterPath::Element element = path.elementAt(i);
        if (element.isLineTo()) {
            i += 1;
        }
        else if (element.isCurveTo()) {
            i += 3;
        }
        else if (element.isMoveTo()) {
            if (_widget->states().find(i) != _widget->states().end()) {
                colors[polygon_index] = _widget->states()[i].color;
                polygon_index += 1;
            }
            i += 1;
        }
    }

    std::vector<std::vector<GUI::Point>> polygons = buildPath(path);

    [self drawPolygonInMTKView:view withPolygons:polygons andColors:colors];

    path.clear();
    _widget->states().clear();
}

/// Called whenever view changes orientation or is resized
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
}

@end