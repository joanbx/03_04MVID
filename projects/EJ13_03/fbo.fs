#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D screenTexture;

uniform vec2 offset;

void main() {

    //http://dev.theomader.com/gaussian-kernel-calculator/
    float kernel[9] = float[] ( 
        0.104829,	0.109453,	0.11288,
        0.114988,	0.115699,	0.114988,	
        0.11288,	0.109453,	0.104829
    );


    vec3 color = vec3(0.0);
    for (int i = -4; i <= 4; ++i) {
        color += vec3(texture(screenTexture, uv + i*offset)) * kernel[i+4];
    }

    FragColor = vec4(color, 1.0);
}
