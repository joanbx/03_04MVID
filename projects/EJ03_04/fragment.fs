#version 330 core

//in vec3 myColor;
in vec3 myPos;
out vec4 FragColor;


void main() {
    FragColor = vec4(myPos, 1.0);
}

//Al tener la posición [-1,1] como color [0,1], le estamos pasando los siguientes valores:
//Vértice derecha:   (0.5 -0.5 0)  -> o en RGB (0.5,0,0) Es decir parcialmente rojo
//Vértice izquierda: (-0.5 -0.5 0) -> o en RGB (0,0,0)   Es decir negro
//Vértice arriba:    (0.0 0.5 0)   -> o en RGB (0,0.5,0) Es decir parcialmente verde