#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

//out vec3 myColor;
out vec3 myPos;

void main() {
    //myColor = aColor;
    myPos = aPos;
    gl_Position = vec4(aPos, 1.0);
}

//Pasamos la posicion (aPos) a un output para usarlo en el shader de fragmentos