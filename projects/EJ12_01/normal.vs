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

out mat3 TBN;
out vec3 fragPos;
out vec2 uv;


struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

void main() {


    uv = aUv;
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));

    //Con Ortogonalización:
    vec3 T = normalize(normalMat * aTangent);
    vec3 N = normalize(normalMat * aNormal);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N, T);
    TBN = transpose(mat3(T, B, N));
    
    //Simple sin ortogonalización:
    // vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    // vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    // vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    // TBN = mat3(T, B, N);

    
    gl_Position = proj * view * model * vec4(aPos, 1.0);

 
}