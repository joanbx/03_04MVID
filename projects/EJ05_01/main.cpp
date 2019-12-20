/*
EJ05.01 - Generar un cubo de manera procedural, a partir de un centro y un radio.
Autor: Joan Baixauli
Ejercicio basado en: AG05
*/
#include <glad/glad.h>

#include "engine/window.hpp"
#include "engine/shader.hpp"

#include "engine/texture.hpp"
#include "engine/geometry/cube.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../../src/engine/geometry/geometry.cpp"
#include <vector>
#include <iostream>

# define PI           3.14159265358979323846  /* pi */

void handleInput() {/*Intentionally Left BLank*/ }

void render(uint32_t VAO, const Shader& shader, Texture& tex) {
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, static_cast<float>(glfwGetTime())* glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	shader.use();

	tex.use(shader, "tex", 0);

	shader.set("model", model);
	shader.set("view", view);
	shader.set("proj", proj);  //TODO const mat4

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//geom.render();
}

int getSign(float f) {
	if (f > 0) return 1;
	if (f < 0) return -1;
	return 0;
}

uint32_t proceduralCube(glm::vec3& center, float& radius, bool& cubeInsideSphere, uint32_t* VBO) {

	//Vertices - Posicions
	std::vector<float> positions;		//Vector/Array posición
	std::vector<float> uvs;				//Vector/Array uvs
	std::vector<float> normals;			//Vector/Array normal
	std::vector<uint32_t> indices;		//Vector/Array Indice
	
	int faces = 6;						//Cubo tiene 6 caras
	uint32_t _nVerts = faces * 2 * 3;	//Número de vértices

	float x=1,y=1,z=1;					//Buffer signos
	float pos[3] = { 0,0,0 };			//Buffer posiciones
	float uv[2] = { 0,0 };				//Buffer uv
	float normal[3] = { 0,0,0 };		//Buffer normal
	int i = 0;							//Buffer índice

	//Si el cubo está dentro de la esfera (radio= centro a vértice), ésta es la fórmula para calcular distancia de centro del cubo a una cara
	if (cubeInsideSphere)
		radius = radius / sqrt(3);

	float angles[6] = {225.0f,135.0f,45.0f,225.0f,45.0f,315.0f}; //ángulos correspondientes a las posiciones de los 6 vértices de los 2 triángulos de un cuadrado.
	int order[6] = {3,1,-3,-1,-2,2};	//que eje fijo tenemos en cada sequencia (1=x,2=y,3=z), el signo determina si el eje fijo es positivo o negativo
										// FRONTAL - DERECHA - TRASERO - IZQUIERDA - ABAJO - ARRIBA (caras de los cubos desenrollados)

	for (int f = 0; f < faces; ++f) { //Por cada cara
		
		for (int p = 0; p < 6; p++) { // 2 * 3 vertex
			if (abs(order[f]) == 3) { //EJE Z fijo
				//pos
				x = getSign(order[f]) * getSign(sin(angles[p] * PI / 180));//sin -1/+1
				y = getSign(cos(angles[p] * PI / 180));//cos -1/+1
				z = getSign(order[f]);
				//uv
				uv[0] = (getSign(order[f]) *x < 0) ? 0 : getSign(order[f]) *x;
				uv[1] = (y < 0) ? 0 : y;
				//normal
				normal[0] = 0; normal[1] = 0; normal[2] = z;

			}
			else if (abs(order[f]) == 2) { //EJE Y fijo
				//pos
				x = getSign(sin(angles[p] * PI / 180));//sin
				z = -getSign(order[f]) * getSign(cos(angles[p] * PI / 180));//cos 
				y = getSign(order[f]);
				//uv
				uv[0] = (x < 0) ? 0 : x;
				uv[1] = (-getSign(order[f]) * z < 0) ? 0 : -getSign(order[f]) * z;
				//normal
				normal[0] = 0; normal[1] = y; normal[2] = 0;
			}
			else if (abs(order[f]) == 1) { //EJE X fijo
				//pos
				y = getSign(cos(angles[p] * PI / 180));//-cos
				z = -getSign(order[f]) * getSign(sin(angles[p] * PI / 180));//-sin
				x = getSign(order[f]);
				//uv
				uv[1] = (y < 0) ? 0 : y;
				uv[0] = (z < 0) ? 0 : z;
				//normal
				normal[0] = x; normal[1] = 0; normal[2] = 0;
			}
			//Calculamos posiciones según radio y centro
			pos[0] = x * radius + center.x;
			pos[1] = y * radius + center.y;
			pos[2] = z * radius + center.z;

			positions.insert(positions.end(), pos, pos + 3);
			uvs.insert(uvs.end(), uv, uv + 2);
			normals.insert(normals.end(), normal, normal + 3);
			indices.push_back(i);
			i++;

			std::cout << pos[0] << "  " << pos[1] << " " << pos[2] << " | " << uv[0] << " " << uv[1] << " | " << normal[0] << "  " << normal[1] << " " << normal[2] << std::endl;
			



		}
		std::cout << "-----------------------" << std::endl;
	}

	uint32_t VAO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(4, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[0]);         //elements
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _nVerts, indices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);                 //positions
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVerts * 3, positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);                 //uvs
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVerts * 2, uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);                 //normals
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _nVerts * 3, normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	positions.clear();
	uvs.clear();
	normals.clear();
	indices.clear();

	return VAO;

}

int main(int, char* []) {
	Window* window = Window::instance();

	glClearColor(0.0f, 0.3f, 0.6f, 1.0f);

	const Shader shader("../projects/AG05/vertex.vs", "../projects/AG05/fragment.fs");


	//float center[3] = { 0,0,0 };
	glm::vec3 center = { 0.25f,0.25f,0 };

	float radio = 0.5f;
	bool CubeInsideSphere = false;


	uint32_t VBO[4]{ 0,0,0,0 };


	const uint32_t VAO = proceduralCube(center, radio, CubeInsideSphere, VBO);
	//const Cube cube(1.0f);

	Texture tex("../assets/textures/blue_blocks.jpg", Texture::Format::RGB);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Window::instance()->setCaptureMode(false);

	while (window->alive()) {
		handleInput();
		render(VAO, shader, tex);
		window->frame();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(4, VBO);
	return 0;
}