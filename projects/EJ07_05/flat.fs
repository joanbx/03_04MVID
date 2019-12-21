#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
//uniform float ambientStrength;



void main() {

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //vec3 ambient = ambientStrength * lightColor;
    //vec3 phong = (ambient + diffuse) * objectColor;
    vec3 phong = (diffuse) * objectColor;
    FragColor = vec4(phong, 1.0f);
}