#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D tex_1;
uniform sampler2D tex_2;
uniform float mixTex;

void main() {
    FragColor = mix(texture(tex_1, UV), texture(tex_2, UV), mixTex);
}