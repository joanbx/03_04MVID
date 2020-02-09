// #version 330 core
// in vec2 TexCoords;
// in vec4 ParticleColor;
// out vec4 color;

// uniform sampler2D sprite;

// void main()
// {
//     color = (texture(sprite, TexCoords) * ParticleColor);
// }  
#version 330 core
in vec2 uv;
in vec4 ParticleColor;
out vec4 FragColor;

uniform sampler2D tex;

void main() {
    vec4 finalColor = vec4(texture(tex, uv) * ParticleColor);
    FragColor = vec4(finalColor);
}