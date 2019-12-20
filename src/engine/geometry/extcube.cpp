#include "engine/geometry/extcube.hpp"

ExtendedCube::ExtendedCube(float size, float cX, float cY, float cZ) : _size(size), _cX(cX), _cY(cY), _cZ(cZ) {
	_nVertices = 6 * 2 * 3;   //6 faces * 2 triangles * 3 vertices;
	_nElements = _nVertices;

    const float half = size / 2.0f;

    float positions[] = { cX - half, cY - half,cZ + half,    //front
						  cX + half, cY - half,cZ + half,
						  cX + half, cY + half,cZ + half,

						  cX - half, cY - half,cZ + half,
						  cX + half, cY + half,cZ + half,
						  cX - half, cY + half,cZ + half,

						  cX + half, cY - half,cZ + half,    //right
						  cX + half, cY - half,cZ - half,
						  cX + half, cY + half,cZ - half,

						  cX + half, cY - half,cZ + half,
						  cX + half, cY + half,cZ - half,
						  cX + half, cY + half,cZ + half,

						  cX + half, cY - half,cZ - half,    //back
						  cX - half, cY - half,cZ - half,
						  cX - half, cY + half,cZ - half,

						  cX + half, cY - half,cZ - half,
						  cX - half, cY + half,cZ - half,
						  cX + half, cY + half,cZ - half,

						  cX - half, cY - half,cZ - half,    //left
						  cX - half, cY - half,cZ + half,
						  cX - half, cY + half,cZ + half,

						  cX - half, cY - half,cZ - half,
						  cX - half, cY + half,cZ + half,
						  cX - half, cY + half,cZ - half,

						  cX - half, cY - half,cZ - half,    //bottom
						  cX + half, cY - half,cZ - half,
						  cX + half, cY - half,cZ + half,

						  cX - half, cY - half,cZ - half,
						  cX + half, cY - half,cZ + half,
						  cX - half, cY - half,cZ + half,

						  cX - half, cY + half,cZ + half,    //top
						  cX + half, cY + half,cZ + half,
						  cX + half, cY + half,cZ - half,

						  cX - half, cY + half,cZ + half,
						  cX + half, cY + half,cZ - half,
						  cX - half, cY + half,cZ - half};

	float uvs[] = { 0.25f, 0.5f,   //front
					0.5f, 0.5f,
					0.5f, 0.75f,

					0.25f, 0.5f,
					0.5f, 0.75f,
					0.25f, 0.75f,

					0.5f, 0.5f,   //right
					0.75f, 0.5f,
					0.75f, 0.75f,

					0.5f, 0.5f,
					0.75f, 0.75f,
					0.5f, 0.75f,

					0.75f, 0.5f,   //back
					1.0f, 0.5f,
					1.0f, 0.75f,

					0.75f, 0.5f,
					1.0f, 0.75f,
					0.75f, 0.75f,

					0.0f, 0.5f,   //left
					0.25f, 0.5f,
					0.25f, 0.75f,

					0.0f, 0.5f,
					0.25f,0.75f,
					0.0f, 0.75f,

					0.25f, 0.25f,   //bottom
					0.5f, 0.25f,
					0.5f, 0.5f,

					0.25f, 0.25f,
					0.5f, 0.5f,
					0.25f, 0.5f,

					0.25f, 0.75f,   //top
					0.5f, 0.75f,
					0.5f, 1.0f,

					0.25f, 0.75f,
					0.5f, 1.0f,
					0.25f, 1.0f };

    float normals[] = { 0.0f, 0.0f, 1.0f,  //front
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,

                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f,

                        1.0f, 0.0f, 0.0f,  //right
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,

                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, 0.0f,

                        0.0f, 0.0f, -1.0f,  //back
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,

                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,
                        0.0f, 0.0f, -1.0f,

                        -1.0f, 0.0f, 0.0f,  //left
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,

                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,
                        -1.0f, 0.0f, 0.0f,

                        0.0f, -1.0f, 0.0f,  //bottom
                        0.0f, -1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f,

                        0.0f, -1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f,
                        0.0f, -1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,  //top
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,

                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f,
                        0.0f, 1.0f, 0.0f};

    uint32_t indices[] = { 0, 1, 2,       3 , 4, 5,  //front
                            6,7,8, 9,10, 11, //right
                            12, 13, 14, 15, 16, 17, //back
                            18, 19, 20, 21, 22, 23, //left
                            24, 25, 26, 27, 28, 29, //bottom
                            30, 31, 32, 33, 34, 35 }; //top

    uploadData(positions, uvs, normals, indices);
}