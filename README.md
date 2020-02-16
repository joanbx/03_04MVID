### 04MVID
OpenGL project regarding the subject "Programación I" (Programming I) from "Máster en Diseño y Desarrollo de Videojuegos" (Design and game development Master) at VIU University.

##Folders
- **Activities**: Description of different activities (spanish)
- **Assets**: 3D Models, textures and fonts used in the scope of this project.
- **CMake**: Compiler
- **include**: All the needed include libraries.
	- Imported in the project: GLFW, KHR, assimp, freetype, glad, glm
	- Custom: engine, gameplay
- **libs**: Assimp and freetype compiled libraries for windows.
- **Projects**: Different activities made during the course to learn OpenGL. Including the Final project.
- **Sources**: Divided by deps, engine and gameplay (shootme up or final project).

##Engine
Our engine consists in the following classes:
- **Assets**: Container of Models and Geometries
- **Camera**: Creates a (perspective) camera
- **GameBehaviour**: Start and Update
- **GameObject**:  Link between node attached to a scenegraph and the gameplay objects
- **Input**:  Instance of input keyboard events
- **Light**: Dirlight, spotlight and pointlight properties
- **Material**: Set Properties of shader with a given shadow class, dirLght, spotlights and pointLights
It is customized for our normal blinn phong shader with directional shadow and multiple lights
- **Mesh**: Render in a given shader a mesh (vertices, indices and textures)
- **Model**: Used to import 3d models with assimp
- **Node**: Nodes that hangs from the scnegraph. It has also a drawable function
- **ParticleSystem**: ParticleGenerator acts as a container for rendering a large number of  particles
- **SceneGraph**: Used to manage all the nodes present on this scene.
- **Shader**: Sets properties into the shader
- **Shadow**: Paints directional shadow. Needs depth and debug shaders, plus a quad geometry and a vec3 direction (dirLight)
- **TextRenderer**: A renderer class for rendering text displayed by a font loaded using the FreeType library.
- **Texture**: It gets a texture from a path to be used. It contains different options (wrap, filter)
- **Transform**: Container for position, rotation and scale
- **Window**: Creates a window instance with a given width and height

##Architecture
![alt text](https://github.com/joanbx/03_04MVID/blob/develop/Architecture.png)

##GamePlay
In the FINAL project we propose a shootem up game with 2d dynamics and 3d rendering.
- **Asteroid**: Asteroid gameobject behaviour
- **Bullet**: Bullet (ammo) object
- **Enemy**: Enemy ships
- **EnemyManager**: Handles all the enemies wtih its bullets plus asteroids and the incoming waves
- **Floor**: Move quads with texture to give a movement effect in the scene
- **Game**: Handles the loading of all objects and classes plus the main loop
- **PowerUp**: Power up behaviour (Extra shoot)
- **Ship**: Player