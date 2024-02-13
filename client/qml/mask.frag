#version 440
layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};
layout(binding = 1) uniform sampler2D colorSource;
layout(binding = 2) uniform sampler2D maskSource;
void main() {
    vec2 texSize = textureSize(maskSource, 0); // Get the size of the texture

    // Convert floating-point texture coordinates to integer texture coordinates
    ivec2 texCoordsInt = ivec2(qt_TexCoord0 * texSize);
    fragColor = texelFetch(colorSource, texCoordsInt, 0)
                    * texelFetch(maskSource, texCoordsInt, 0).a
                    * qt_Opacity;
}
