#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

uniform vec2 offset;

void main() {
   
    vec2 offsets[9] = vec2[](
        vec2(-offset.x, offset.y),
        vec2(0.0, offset.y),
        vec2(offset.x, offset.y),
        vec2(-offset.x, 0.0),
        vec2(0.0, 0.0),
        vec2(offset.x, 0.0),
        vec2(-offset.x, -offset.y),
        vec2(0.0, -offset),
        vec2(offset.x, -offset.y)
    );

    
    float Gx[9] = float[] (
        -1, 0, 1,
        -2,  0, 2,
        -1, 0, 1
    );
    float Gy[9] = float[] (
        -1, -2, -1,
        0,  0, 0,
        1, 2, 1
    );

    vec4 Sx = vec4(0.0);
    vec4 Sy = vec4(0.0);
    for(int i = 0; i<9; i++){
        Sx += texture(screenTexture, uv.st + offsets[i]) * Gx[i];
        Sy += texture(screenTexture, uv.st + offsets[i]) * Gy[i];
    }
    
    FragColor = sqrt(Sx * Sx + Sy * Sy);

}