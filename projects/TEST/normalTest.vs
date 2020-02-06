#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aUv;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec3 aTangent;
layout (location=4) in vec3 aBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMat;
uniform mat4 lightSpaceMatrix;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

//out vec3 fragPos;
out vec2 uv;
out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;
out vec3 simpleNormal;
out vec4 fragPosLighSpace;

uniform vec3 viewPos;

void main() {
    uv = aUv;
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));

    simpleNormal = normalMat * aNormal;

    //Normal
    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N, T);

    mat3 TBN = transpose(mat3(T, B, N));  //inverse of TBN, World -> tangent space

    tangentLightPos = TBN * dirLight.direction;
    tangentViewPos = TBN * viewPos;
    tangentFragPos = TBN * fragPos;

    //SHADOW
    fragPosLighSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
