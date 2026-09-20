#version 450
// 3D textured pipeline: world-space positions, combined MVP via push constants.
// NDC convention matches the 2D pipelines (screen-top -> y=-1); the MVP built
// on CPU (SetViewProj) already encodes the Y flip, so no negation here.
layout(push_constant) uniform Push { mat4 mvp; } push;
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;
layout(location = 0) out vec2 outUV;
layout(location = 1) out vec4 outColor;
void main() {
    gl_Position = push.mvp * vec4(inPos, 1.0);
    outUV = inUV;
    outColor = inColor;
}
