#version 450
// Fullbright textured: lightmaps/renderstyles come later (R2+); vertex color
// carries per-vertex tint, alpha blends like the 2D textured pipeline.
layout(binding = 0) uniform sampler2D tex;
layout(location = 0) in vec2 inUV;
layout(location = 1) in vec4 inColor;
layout(location = 0) out vec4 outColor;
void main() {
    outColor = texture(tex, inUV) * inColor;
}
