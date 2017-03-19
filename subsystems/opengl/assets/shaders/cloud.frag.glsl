#version 330
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColour;

void main() {
    color = vec4(spriteColour, 1.0) * texture(image, TexCoords);
//    color = vec4(TexCoords, 0.0f, 1.0f);
}