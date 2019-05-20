//glew include
#include <GL/glew.h>

//std includes
#include <sstream>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

//glfw include
#include <GLFW/glfw3.h>

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
//Texture includes
#include "Headers/Texture.h"
//Model includes
#include "Headers/Model.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Box box, box2, box3, box4, boxb1, boxb2, boxm, boxm2, boxm3;
Box boxWater;
Cylinder cylinder2(20, 20, 0.5, 0.5);

Sphere sphereAnimacion(20, 20);
Cylinder cylinderAnimacion(20, 20, 0.5, 0.5);
Cylinder cylinderAnimacion2(20, 20, 0.5, 0.5);

Shader shaderColor;
Shader shaderTexture;
Shader shaderCubeTexture;
Shader shaderMateriales;
Shader shaderDirectionLight;
Shader shaderPointLight;
Shader shaderSpotLight;
Shader shaderLighting;

Model modelRock;
Model modelArbol;
Model modelAirCraft;
Model arturito;
Model modelTrain;
Model modelPajaro;
Model modelPersonas;
Model modelHumano;
Model modelHombreAdmirando;
Model modelCpu;
//Model modelComputer;
//Model modelLCD;
//Model modelComputer;
//Model modelLCD;
Model modelSilla;
Model modelMesa;
Model modelLaptop;


GLuint textureID1, textureHojasID, textureID3, textureCespedID, textureWaterID, textureCubeTexture, textureMetalID,
textureConcretoID, textureRocaID, textureParedID, textureParedSalonID, textureTirolID, texturecrID, texturePuertaCristalID,
textureCpuID, textureNegroID, textureMaderaID, textureMonitorID, textureTecladoID, textureMouseID, texturePizarronID,
textureProyectorID, textureBlancoID, texturePisoID, textureQID, textureGlassID, textureTierraID, textureAuxiliarID, texturePatasID, 
textureGarrafonID, textureDispensadorID, textureRojoID , textureBrazoID;
GLuint cubeTextureID;

std::vector<std::vector<glm::mat4>> getKeyFrames(std::string fileName) {
	std::vector<std::vector<glm::mat4>> keyFrames;
	std::string line;
	std::ifstream infile(fileName);
	std::string s = line;
	while (std::getline(infile, line))
	{
		std::vector<glm::mat4> transforms;
		s = line;
		size_t pos1 = 0;
		std::string token1;
		std::string delimiter1 = "|";
		while ((pos1 = s.find(delimiter1)) != std::string::npos) {
			token1 = s.substr(0, pos1);
			//std::cout << token1 << std::endl;

			size_t pos2 = 0;
			std::string token2;
			std::string delimiter2 = ",";

			int i = 0;
			int j = 0;
			glm::mat4 transform;
			float aaa[16];
			while ((pos2 = token1.find(delimiter2)) != std::string::npos) {
				token2 = token1.substr(0, pos2);
				aaa[i++] = atof(token2.c_str());
				/*if (j == 0)
					transform[i].x = atof(token2.c_str());
				if (j == 1)
					transform[i].y = atof(token2.c_str());
				if (j == 2)
					transform[i].z = atof(token2.c_str());
				if (j == 3)
					transform[i].w = atof(token2.c_str());*/
					/*j++;
					if (j > 3) {
						i++;
						j = 0;
					}*/
				token1.erase(0, pos2 + delimiter2.length());
			}
			aaa[i++] = atof(token1.c_str());
			transform = glm::make_mat4(aaa);
			transforms.push_back(transform);
			s.erase(0, pos1 + delimiter1.length());
		}
		keyFrames.push_back(transforms);
	}
	return keyFrames;
}

GLenum types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

std::string fileNames[6] = {
	"../../Textures/mp_sincity/sincity_ft.tga",
	"../../Textures/mp_sincity/sincity_bk.tga",
	"../../Textures/mp_sincity/sincity_up.tga",
	"../../Textures/mp_sincity/sincity_dn.tga",
	"../../Textures/mp_sincity/sincity_rt.tga",
	"../../Textures/mp_sincity/sincity_lf.tga"
};

int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX, offsetX;
int lastMousePosY, offsetY;

double deltaTime;

float rot1 = 0.01f;
float compy = glm::cos(glm::radians(-40.0f));
float yaw2 = -90.0f;
float yaw1 = 90.0f;
float pitch2 = 0.0f;
float pitch1 = -87.0f;

int dirTotal = 0;
bool dir = true;

float rot0 = 0, rot2 = 0, rot3 = 0;
float rot4 = 0, rot5 = 0, rot6 = 0;
float rot7 = 0, rot8 = 0, rot9 = 0;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	shaderColor.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");
	shaderTexture.initialize("../../Shaders/texturizado_res.vs", "../../Shaders/texturizado_res.fs");
	shaderCubeTexture.initialize("../../Shaders/cubeTexture.vs", "../../Shaders/cubeTexture.fs");
	shaderMateriales.initialize("../../Shaders/iluminacion_materiales_res.vs", "../../Shaders/iluminacion_materiales_res.fs");
	shaderDirectionLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/directionalLight.fs");
	shaderPointLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/pointLight.fs");
	shaderSpotLight.initialize("../../Shaders/typeLight.vs", "../../Shaders/spotLight.fs");
	shaderLighting.initialize("../../Shaders/typeLight.vs", "../../Shaders/multipleLights.fs");

	sphereAnimacion.init();
	sphereAnimacion.setShader(&shaderLighting);
	sphereAnimacion.setColor(glm::vec3(0.3, 0.3, 1.0));

	cylinderAnimacion.init();
	cylinderAnimacion.setShader(&shaderLighting);
	cylinderAnimacion.setColor(glm::vec3(0.8, 0.3, 1.0));

	cylinderAnimacion2.init();
	cylinderAnimacion2.setShader(&shaderLighting);
	cylinderAnimacion2.setColor(glm::vec3(0.2, 0.7, 0.3));

	sphere.init();
	cylinder.init();
	box.init();
	boxb1.init();
	boxb2.init();
	boxm.init();
	boxm2.init();
	boxm3.init();
	box4.init();
	box.scaleUVS(glm::vec2(10.0, 10.0));
	box2.init();
	box2.scaleUVS(glm::vec2(1.0, 1.0));
	boxWater.init();
	boxWater.scaleUVS(glm::vec2(1.0, 1.0));
	box3.init();
	modelRock.loadModel("../../models/rock/rock.obj");
	modelArbol.loadModel("../../models/Conifer_1_Obj/Tree_Conifer_1.obj");
	modelAirCraft.loadModel("../../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelPajaro.loadModel("../../models/Drone-Design/Drone_obj.obj");
	modelPersonas.loadModel("../../models/together__people/people.obj");
	modelHumano.loadModel("../../models/Patrick/Patrick.obj");
	modelHombreAdmirando.loadModel("../../models/30817character007/7.obj");
	modelSilla.loadModel("../../models/Sillas/source/215219/215219.obj");
	modelMesa.loadModel("../../models/Table/source/SCP table .obj");
	modelLaptop.loadModel("../../models/laptop/Laptop_High-Polay_HP_BI_2_obj.obj");
	//modelLCD.loadModel("../../models/lcd/LCD.obj");
	//modelComputer.loadModel("../../models/computadora/Comp_and_Floppy.obj");

	camera->setPosition(glm::vec3(0.0f, 15.0f, 50.0f));

	// Textura Ladrillos
	int imageWidth, imageHeight;
	Texture texture("../../Textures/texturaLadrillos.jpg");
	FIBITMAP* bitmap = texture.loadImage(false);
	unsigned char * data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID1);
	glBindTexture(GL_TEXTURE_2D, textureID1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	// Texture Goku
	texture = Texture("../../Textures/hojas.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureHojasID);
	glBindTexture(GL_TEXTURE_2D, textureHojasID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	// Texture glass
	texture = Texture("../../Textures/glass.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureGlassID);
	glBindTexture(GL_TEXTURE_2D, textureGlassID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap)
		;
	// Texture Tierra
	texture = Texture("../../Textures/tierra.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTierraID);
	glBindTexture(GL_TEXTURE_2D, textureTierraID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	// Textura cuadritos
	texture = Texture("../../Textures/test.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureID3);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/cesped.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureCespedID);
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/concreto.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureConcretoID);
	glBindTexture(GL_TEXTURE_2D, textureConcretoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	Texture texture3("../../Textures/rocaV.jpg");
	FIBITMAP* bitmap3 = texture3.loadImage();
	unsigned char * data3 = texture3.convertToData(bitmap3, imageWidth, imageHeight);
	glGenTextures(1, &textureRocaID);
	//se enlaza el tipo de textura al ID textureID (Texture 2D)
	glBindTexture(GL_TEXTURE_2D, textureRocaID);
	// set the texture wrapping parameters
	//repetir en eje S y eje T (en este caso el X y Y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data3) {
		//TipoTextura,0(MIPMAPS),Formato interno del buffer, ancho, alto, borde, formato de la libreria, tipo de dato, datos de la imagen
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data3);
		//le indica a openGL que se encargue de general los mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	texture3.freeImage(bitmap3);

	texture = Texture("../../Textures/water2.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureWaterID);
	glBindTexture(GL_TEXTURE_2D, textureWaterID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/metal.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMetalID);
	glBindTexture(GL_TEXTURE_2D, textureMetalID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/ventana2.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureQID);
	glBindTexture(GL_TEXTURE_2D, textureQID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/pared.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParedID);
	glBindTexture(GL_TEXTURE_2D, textureParedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	texture = Texture("../../Textures/cr.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturecrID);
	glBindTexture(GL_TEXTURE_2D, texturecrID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/ParedSalon.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParedSalonID);
	glBindTexture(GL_TEXTURE_2D, textureParedSalonID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/Tirol.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTirolID);
	glBindTexture(GL_TEXTURE_2D, textureTirolID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/puerta_cristal.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturePuertaCristalID);
	glBindTexture(GL_TEXTURE_2D, texturePuertaCristalID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/cpu_frente.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureCpuID);
	glBindTexture(GL_TEXTURE_2D, textureCpuID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/negro.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureNegroID);
	glBindTexture(GL_TEXTURE_2D, textureNegroID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/madera.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMaderaID);
	glBindTexture(GL_TEXTURE_2D, textureMaderaID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/monitor.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMonitorID);
	glBindTexture(GL_TEXTURE_2D, textureMonitorID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/Teclado.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureTecladoID);
	glBindTexture(GL_TEXTURE_2D, textureTecladoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/mouse.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureMouseID);
	glBindTexture(GL_TEXTURE_2D, textureMouseID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/pizarron.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturePizarronID);
	glBindTexture(GL_TEXTURE_2D, texturePizarronID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/proyector.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureProyectorID);
	glBindTexture(GL_TEXTURE_2D, textureProyectorID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/blanco.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureBlancoID);
	glBindTexture(GL_TEXTURE_2D, textureBlancoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/piso.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturePisoID);
	glBindTexture(GL_TEXTURE_2D, texturePisoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/patas.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &texturePatasID);
	glBindTexture(GL_TEXTURE_2D, texturePatasID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/garrafon.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureGarrafonID);
	glBindTexture(GL_TEXTURE_2D, textureGarrafonID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/dispensador.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureDispensadorID);
	glBindTexture(GL_TEXTURE_2D, textureDispensadorID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	texture = Texture("../../Textures/rojo.jpg");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureRojoID);
	glBindTexture(GL_TEXTURE_2D, textureRojoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);

	texture = Texture("../../Textures/brazo.png");
	bitmap = texture.loadImage(false);
	data = texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureBrazoID);
	glBindTexture(GL_TEXTURE_2D, textureBrazoID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	texture.freeImage(bitmap);


	glGenTextures(1, &cubeTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		texture = Texture(fileNames[i]);
		FIBITMAP* bitmap = texture.loadImage(true);
		data = texture.convertToData(bitmap, imageWidth, imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		texture.freeImage(bitmap);
	}

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shaderColor.destroy();
	shaderTexture.destroy();
	shaderCubeTexture.destroy();
	shaderMateriales.destroy();
	shaderDirectionLight.destroy();
	shaderPointLight.destroy();
	shaderSpotLight.destroy();
	sphere.destroy();
	cylinder.destroy();

	sphereAnimacion.destroy();
	cylinderAnimacion.destroy();
	cylinderAnimacion2.destroy();

	box.destroy();
	box2.destroy();
	box.destroy();
	box2.destroy();
	box3.destroy();
	boxWater.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
	if ((glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) && rot1 < 0.0f) {

		rot1 += 0.1;
	}
	if ((glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) && rot1 > -1.49f) {
		rot1 -= 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		camera->setPitch(pitch1);
		camera->setYaw(yaw2);
		camera->setPosition(glm::vec3(0.0f, 100.0f, 30.0f));
		camera->setUp(glm::vec3(0.0, 1.0, 0.0));
		camera->setFront(glm::vec3(0.0, -compy, -1.0));
		camera->update();
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		camera->setPitch(pitch2);
		camera->setYaw(yaw2);
		camera->setPosition(glm::vec3(0.0f, 15.0f, 50.0f));
		camera->setUp(glm::vec3(0.0, 1.0, 0.0));
		camera->setFront(glm::vec3(0.0, -compy, -1.0));
		camera->update();
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		camera->setPitch(pitch2);
		camera->setYaw(yaw1);
		camera->setPosition(glm::vec3(-36.0, 6.5, 25.9));
		camera->setUp(glm::vec3(0.0, 1.0, 0.0));
		camera->setFront(glm::vec3(0.0, -compy, -1.0));
		camera->update();
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		camera->setPitch(pitch2);
		camera->setYaw(yaw1);
		camera->setPosition(glm::vec3(27.2, 2.7, 24.8));
		camera->setUp(glm::vec3(0.0, 1.0, 0.0));
		camera->setFront(glm::vec3(0.0, -compy, -1.0));
		camera->update();
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			textureAuxiliarID = textureNegroID;
		}
		else {
			textureAuxiliarID = textureMonitorID;
		}
	}

	//saludo
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (dir == true && rot7 > -1.3) {
			rot7 -= 0.1;
		}
		else {
			dir = false;
		}
		if (dir == false && rot7 < 1.3f) {
			rot7 += 0.1;
		}
		else {
			dir = true;
		}
	}
	//Presumir brazo.
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {

		if (dirTotal == 0 && rot7 > -1.3) {
			rot7 -= 0.1;
		}
		else {
			dirTotal = 1;
		}
		if (dirTotal == 1 && rot4 > -1.3) {
			rot4 -= 0.1;
		}
		else {
			dirTotal = 2;
		}
		if (dirTotal == 2 && rot0 > -1.3) {
			rot0 -= 0.1;
		}
		else {
			dirTotal = 3;
		}
		if (dirTotal == 3 && rot0 < 1.3f) {
			rot0 += 0.1;
		}
		else {
			dirTotal = 4;
		}
		if (dirTotal == 4 && rot4 < 1.3f) {
			rot4 += 0.1;
		}
		else {
			dirTotal = 5;
		}
		if (dirTotal == 5 && rot7 < 1.3f) {
			rot7 += 0.1;
		}
		else {
			dirTotal = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			rot0 -= 0.1;
		}
		else {
			rot0 += 0.1;

		}
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			rot4 -= 0.1;
		}
		else {
			rot4 += 0.1;

		}
	}

	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			rot7 -= 0.1;
		}
		else {
			rot7 += 0.1;

		}
	}
}


void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	float angle = 0.0;
	float ratio = 50.0;

	float aircraftZ = 0.0, saltoY = 0.0;
	bool direcionAirCraft = true, Salto = true;
	float rotationAirCraft = 0.0;
	bool finishRotation = true, Brincar = true;

	// Game loop
	float NanoZ = 0.0, NanoX = 0.0;
	float rotationNano = 0.0;
	int  cuartoVuelta = 0, direcionNano = 0;


	//se obtienen los frames del brazo.
	std::vector<std::vector<glm::mat4>> keyFramesBrazo = getKeyFrames("../../animaciones/animationMano.txt");
	//numero de pasos para moverse de un frame a otro (bascimente la velocidad)
	int numPasosAnimBrazo = 500;
	int numPasosAnimBrazoCurr = 0;

	//indices del arreglo keyFramesBrazo el actual y el siguiente
	int indexKeyFrameBrazoCurr = 0;
	int indexKeyFrameBrazoNext = 1;
	float interpolation = 0.0;

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		// Matrix de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		// matrix de vista
		glm::mat4 view = camera->getViewMatrix();

		cylinderAnimacion.setProjectionMatrix(projection);
		cylinderAnimacion.setViewMatrix(view);
		sphereAnimacion.setProjectionMatrix(projection);
		sphereAnimacion.setViewMatrix(view);

		shaderTexture.turnOn();
		// Importante si se quiere renderizar modelos y texturas
		/*
		glActiveTexture(GL_TEXTURE0);
		cylinder.setShader(&shaderTexture);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setPosition(glm::vec3(-3.0f, 2.0f, -3.0f));
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		cylinder.render(0, cylinder.getSlices() * cylinder.getStacks() * 2 * 3);
		glBindTexture(GL_TEXTURE_2D, textureID1);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3, cylinder.getSlices() * 3);
		glBindTexture(GL_TEXTURE_2D, textureID3);
		cylinder.render(cylinder.getSlices() * cylinder.getStacks() * 2 * 3 + cylinder.getSlices() * 3, cylinder.getSlices() * 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		shaderTexture.turnOff();
		*/

		// Iluminaci�n
		glm::mat4 lightModelmatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
		lightModelmatrix = glm::translate(lightModelmatrix, glm::vec3(0.0f, 0.0f, -ratio));

		shaderMateriales.turnOn();
		glUniform3fv(shaderMateriales.getUniformLocation("light.position"), 1, glm::value_ptr(glm::vec3(lightModelmatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
		glUniform3fv(shaderMateriales.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
		glUniform3f(shaderMateriales.getUniformLocation("light.ambient"), 0.2, 0.2, 0.2);
		glUniform3f(shaderMateriales.getUniformLocation("light.diffuse"), 0.2, 0.3, 0.6);
		glUniform3f(shaderMateriales.getUniformLocation("light.specular"), 0.5, 0.3, 0.2);
		glUniform3f(shaderMateriales.getUniformLocation("material.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(shaderMateriales.getUniformLocation("material.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(shaderMateriales.getUniformLocation("material.specular"), 0.7f, 0.7f, 0.7f);
		glUniform1f(shaderMateriales.getUniformLocation("material.shininess"), 10.0f);
		//cylinder.render();
		shaderMateriales.turnOff();

		shaderLighting.turnOn();
		glUniform3fv(shaderLighting.getUniformLocation("viewPos"), 1, glm::value_ptr(camera->getPosition()));
		//Directional light
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.ambient"), 0.25, 0.25, 0.25);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.diffuse"), 0.8, 0.8, 0.8);
		glUniform3f(shaderLighting.getUniformLocation("directionalLight.light.specular"), 1.15, 1.15, 1.15);
		glUniform3fv(shaderLighting.getUniformLocation("directionalLight.direction"), 1, glm::value_ptr(glm::vec3(0, -1.0, 0.0)));
		//Numero de luces spot y point
		int locCount = shaderLighting.getUniformLocation("pointLightCount");
		glUniform1i(shaderLighting.getUniformLocation("pointLightCount"), 1);
		glUniform1i(shaderLighting.getUniformLocation("spotLightCount"), 1);
		// Point light
		glUniform3fv(shaderLighting.getUniformLocation("pointLights[0].position"), 1, glm::value_ptr(glm::vec3(lightModelmatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))));
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].constant"), 1.0f);
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].linear"), 0.14f);
		glUniform1f(shaderLighting.getUniformLocation("pointLights[0].quadratics"), 0.07f);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.ambient"), 0.025, 0.025, 0.025);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.diffuse"), 0.2, 0.3, 0.15);
		glUniform3f(shaderLighting.getUniformLocation("pointLights[0].light.specular"), 0.5, 0.1, 0.2);
		// Spot light
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].position"), 1, glm::value_ptr(camera->getPosition()));
		glUniform3fv(shaderLighting.getUniformLocation("spotLights[0].direction"), 1, glm::value_ptr(camera->getFront()));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].outerCutOff"), glm::cos(glm::radians(15.0f)));
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].constant"), 1.0f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].linear"), 0.14f);
		glUniform1f(shaderLighting.getUniformLocation("spotLights[0].quadratics"), 0.07f);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.ambient"), 0.25, 0.25, 0.25);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.diffuse"), 0.7, 0.2, 0.6);
		glUniform3f(shaderLighting.getUniformLocation("spotLights[0].light.specular"), 0.1, 0.7, 0.8);
		shaderLighting.turnOff();

		modelArbol.setShader(&shaderLighting);
		modelArbol.setProjectionMatrix(projection);
		modelArbol.setViewMatrix(view);
		modelArbol.setPosition(glm::vec3(-6.0, 3.5, 35.0));
		modelArbol.setScale(glm::vec3(0.05, 0.05, 0.05));
		modelArbol.render();

		modelPajaro.setShader(&shaderLighting);
		modelPajaro.setProjectionMatrix(projection);
		modelPajaro.setViewMatrix(view);
		//modelPajaro.setPosition(glm::vec3(0.0f, 25.0f, 45.0f));
		modelPajaro.setScale(glm::vec3(10.0, 10.0, 10.0));
		glm::mat4 matrixAirCraft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, aircraftZ, 0.0f));
		matrixAirCraft = glm::translate(matrixAirCraft, glm::vec3(0.0f, 25.0f, 55.0f));
		matrixAirCraft = glm::rotate(matrixAirCraft, rotationAirCraft, glm::vec3(0, 1, 0));
		modelPajaro.render(matrixAirCraft);

		//no funciona al moverle en el eje Y
		/*
		modelPersonas.setShader(&shaderLighting);
		modelPersonas.setProjectionMatrix(projection);
		modelPersonas.setViewMatrix(view);
		//modelPersonas.setPosition(glm::vec3(0.0f, 25.0f, 45.0f));
		modelPersonas.setScale(glm::vec3(0.03, 0.03, 0.03));
		glm::mat4 matrixNano = glm::translate(glm::mat4(1.0f), glm::vec3(NanoX, 0.0f, NanoZ));
		matrixNano = glm::translate(matrixNano, glm::vec3(-15.0f, 0.0f, 30.0f));
		matrixNano = glm::rotate(matrixNano, rotationNano, glm::vec3(0, 1, 0));
		modelPersonas.render(matrixNano);
		*/

		modelHumano.setShader(&shaderLighting);
		modelHumano.setProjectionMatrix(projection);
		modelHumano.setViewMatrix(view);
		//modelHumano.setPosition(glm::vec3(0.0f, 25.0f, 45.0f));
		modelHumano.setScale(glm::vec3(4.03, 4.03, 4.03));
		glm::mat4 matrixNano = glm::translate(glm::mat4(1.0f), glm::vec3(NanoX, saltoY, NanoZ));
		matrixNano = glm::translate(matrixNano, glm::vec3(-13.0f, 1.15f, 25.0f));
		matrixNano = glm::rotate(matrixNano, rotationNano, glm::vec3(0, 1, 0));
		modelHumano.render(matrixNano);

		//no hace nada
		modelHombreAdmirando.setShader(&shaderLighting);
		modelHombreAdmirando.setProjectionMatrix(projection);
		modelHombreAdmirando.setViewMatrix(view);
		modelHombreAdmirando.setPosition(glm::vec3(0.0f, 2.0f, 80.0f));
		modelHombreAdmirando.setScale(glm::vec3(100.03, 100.03, 100.03));
		modelHombreAdmirando.render();



		/*
		glm::quat firstQuat;
		glm::quat secondQuat;
		glm::quat finalQuat;
		glm::mat4 interpoltaedMatrix;
		glm::vec4 transformComp1;
		glm::vec4 transformComp2;
		glm::vec4 finalTrans;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID3);
		if (keyFramesBrazo[indexKeyFrameBrazoCurr].size() == 7 && keyFramesBrazo[indexKeyFrameBrazoNext].size() == 7) {

			//matriz de rotacion actual
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][0]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][0]);
			//slerp hace la interpolacion de quaternion (matriz de rotacion)
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			//se convierte el quaternion a una matriz de 4x4
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			//se obteiene la translacion del frame i -1
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][0] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			//se obteiene la translacion del frame i
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][0] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			// se realiza la interpolacion entre el frame i-1 y el frame i
			//transformcomp1 es frame i-1
			//transformcomp2 es frame i
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			//unimos la matriz de interpolacion del quaternion y la interpolacion de la translacion
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			// Animacion KeyFrames
			glm::mat4 matrixGlobalAnimation = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.0f, 0.0f));
			// Se modela siempre con los ejes de giro en el eje z
			// Articulacion 1
			glm::mat4 keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ0 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ0 = glm::scale(cylinderMatrixJ0, glm::vec3(0.08f, 0.08f, 0.08f));
			sphereAnimacion.render(cylinderMatrixJ0);

			// Articulacion 2
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][1]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][1]);
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][1] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][1] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ1 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ1 = glm::scale(cylinderMatrixJ1, glm::vec3(0.1f, 0.1f, 0.1f));
			sphereAnimacion.render(cylinderMatrixJ1);

			// Articulacion 3
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][2]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][2]);
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][2] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][2] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ2 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ2 = glm::scale(cylinderMatrixJ2, glm::vec3(0.11f, 0.11f, 0.11f));
			sphereAnimacion.render(cylinderMatrixJ2);

			// Hueso 1
			glm::mat4 cylinderMatrixL1 = glm::translate(keyFrameJoint, glm::vec3(0.0f, 0.0f, 0.125f));
			cylinderMatrixL1 = glm::rotate(cylinderMatrixL1, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixL1 = glm::scale(cylinderMatrixL1, glm::vec3(0.1f, 0.25f, 0.1f));
			cylinderAnimacion.render(cylinderMatrixL1);

			// Articulacion 4
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][3]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][3]);
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][3] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][3] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ3 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ3 = glm::scale(cylinderMatrixJ3, glm::vec3(0.11f, 0.11f, 0.11f));
			sphereAnimacion.render(cylinderMatrixJ3);

			// Articulacion 5
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][4]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][4]);
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][4] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][4] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ4 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ4 = glm::scale(cylinderMatrixJ4, glm::vec3(0.11f, 0.11f, 0.11f));
			sphereAnimacion.render(cylinderMatrixJ4);

			// Hueso 2
			glm::mat4 cylinderMatrixL2 = glm::translate(keyFrameJoint, glm::vec3(0.0f, 0.0f, 0.125f));
			cylinderMatrixL2 = glm::rotate(cylinderMatrixL2, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixL2 = glm::scale(cylinderMatrixL2, glm::vec3(0.1f, 0.25f, 0.1f));
			cylinderAnimacion.render(cylinderMatrixL2);

			// Articulacion 6
			firstQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoCurr][5]);
			secondQuat = glm::quat_cast(keyFramesBrazo[indexKeyFrameBrazoNext][5]);
			finalQuat = glm::slerp(firstQuat, secondQuat, interpolation);
			interpoltaedMatrix = glm::mat4_cast(finalQuat);
			transformComp1 = keyFramesBrazo[indexKeyFrameBrazoCurr][5] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			transformComp2 = keyFramesBrazo[indexKeyFrameBrazoNext][5] * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			finalTrans = (float)(1.0 - interpolation) * transformComp1 + transformComp2 * interpolation;
			interpoltaedMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(finalTrans)) * interpoltaedMatrix;

			keyFrameJoint = matrixGlobalAnimation * interpoltaedMatrix;
			glm::mat4 cylinderMatrixJ5 = glm::rotate(keyFrameJoint, 1.5708f, glm::vec3(1.0, 0.0f, 0.0));
			cylinderMatrixJ5 = glm::scale(cylinderMatrixJ5, glm::vec3(0.11f, 0.11f, 0.11f));
			sphereAnimacion.render(cylinderMatrixJ5);

		}

		numPasosAnimBrazoCurr++;
		interpolation = numPasosAnimBrazoCurr / (float)numPasosAnimBrazo;

		if (interpolation >= 1.0) {
			interpolation = 0.0;
			numPasosAnimBrazoCurr = 0;
			indexKeyFrameBrazoCurr = indexKeyFrameBrazoNext;
			indexKeyFrameBrazoNext++;
		}

		if (indexKeyFrameBrazoNext > keyFramesBrazo.size() - 1) {
			indexKeyFrameBrazoCurr = 0;
			indexKeyFrameBrazoNext = 1;
		}
		*/

		/*arturito.setShader(&shaderLighting);
		arturito.setProjectionMatrix(projection);
		arturito.setViewMatrix(view);
		arturito.setScale(glm::vec3(1.0, 1.0, 1.0));
		glm::mat4 matrixArturito = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, aircraftZ));
		matrixArturito = glm::translate(matrixArturito, glm::vec3(-10.0, 2.0, 15.0));
		matrixArturito = glm::rotate(matrixArturito, rotationAirCraft, glm::vec3(0, 1, 0));
		arturito.render(matrixArturito);*/

		//Suelo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureConcretoID);
		box.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setPosition(glm::vec3(0.0, 0.0, 0.0));
		box.setScale(glm::vec3(120.0, 2.2, 160.0));
		box.render();

		//Jardinera
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.0, 2.25, 35.0));
		box2.setScale(glm::vec3(11.0, 2.5, 11.0));
		box2.render();

		//hojas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.0, 3.55, 35.0));
		box2.setScale(glm::vec3(8.0, 0.001, 8.0));
		box2.render();

		//bardas

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-2.75, 1.25, 35.5));
		box2.setScale(glm::vec3(1.5, 2.5, 6.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(3.0, 1.25, 37.75));
		box2.setScale(glm::vec3(11.0, 2.5, 1.5));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(10.5, 1.25, 36.25));
		box2.setScale(glm::vec3(3.0, 2.5, 1.5));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(12.75, 1.25, 28.5));
		box2.setScale(glm::vec3(1.5, 2.5, 6.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(17.5, 1.25, 30.75));
		box2.setScale(glm::vec3(8.0, 2.5, 1.5));
		box2.render();*/
		/////////////////////////aqui empezar� con lo mio

		//derecha1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(27.0, 23.1, 41.9));
		box2.setScale(glm::vec3(26.0f, 44.0, 0.0f));
		box2.render();

		//derecha-derecha2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(65.0, 44.0, 0.0));
		glm::mat4 matrixBox = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, -16.0));
		matrixBox = glm::rotate(matrixBox, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox);

		//derecha6
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(27.0, 23.1, -48.5));
		box2.setScale(glm::vec3(26.0, 44.0, 0.0));
		box2.render();

		//izquierda3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 23.1, 65.0));
		box2.setScale(glm::vec3(26.0, 44.0, 0.0));
		box2.render();

		//izquierda5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 23.1, -36.0));
		box2.setScale(glm::vec3(26.0, 44.0, 0.0));
		box2.render();

		//izquierda izquierda4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(76.0, 44.0, 0.0));
		glm::mat4 matrixBox2 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, 2.0));
		matrixBox2 = glm::rotate(matrixBox2, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox2);

		//izquierda-izquierda7
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(63.15, 44.0, 0.0));
		glm::mat4 matrixBox3 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, 33.45));
		matrixBox3 = glm::rotate(matrixBox3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox3);

		//izquierda-izquierda7-2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(63.0, 44.0, 0.0));
		glm::mat4 matrixBox4 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, 33.5));
		matrixBox4 = glm::rotate(matrixBox4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox4);

		//izquierda8
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 23.1, 1.9));
		box2.setScale(glm::vec3(37.0, 44.0, 0.0));
		box2.render();

		//izquierda-izquierda9
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(40.0, 44.0f, 0.0));
		glm::mat4 matrixBox5 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, 21.9));
		matrixBox5 = glm::rotate(matrixBox5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox5);

		//izquierda-izquierda2-2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(40.0, 44.0, 0.0));
		glm::mat4 matrixBox6 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, 21.9));
		matrixBox6 = glm::rotate(matrixBox6, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox6);

		//izquierda-izquierda10
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(14.5, 44.0, 0.0));
		glm::mat4 matrixBox7 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1F, -28.75));
		matrixBox7 = glm::rotate(matrixBox7, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox7);

		//izquierda-izquierda11
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(27.0, 44.0, 0.0));
		glm::mat4 matrixBox8 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, -35.0));
		matrixBox8 = glm::rotate(matrixBox8, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox8);

		//izquierda12
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 23.1, -21.5));
		box2.setScale(glm::vec3(37.0, 44.0, 0.0));
		box2.render();

		//techo derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(26.0f, 90.4f, 0.0f));
		glm::mat4 matrixBox9 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 45.1, -3.3));
		matrixBox9 = glm::rotate(matrixBox9, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		box.render(matrixBox9);

		//techo izquierda
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(26.0, 101.0, 0.0));
		glm::mat4 matrixBox10 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 45.1, 14.5));
		matrixBox10 = glm::rotate(matrixBox10, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		box.render(matrixBox10);

		//techo enmedio
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(39.0, 23.5, 0.0));
		glm::mat4 matrixBox11 = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0, 45.1, -9.8));
		matrixBox11 = glm::rotate(matrixBox11, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		box.render(matrixBox11);

		//PILAR-FRONTAL
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.0, 23.1, 2.0));
		box2.setScale(glm::vec3(2.0, 44.0, 1.0));
		box2.render();

		//PILAR-FRONTAL2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(2.0, 23.1, 2.0));
		box2.setScale(glm::vec3(2.0, 44.0, 1.0));
		box2.render();

		//PILAR-H
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 12.0, 2.2));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//PILAR-H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 22.0, 2.2));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//PILAR-H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 32.0, 2.2));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//PILAR-H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 42.0, 2.2));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//PILAR-H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 44.0, 2.2));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		////pilar izquierda
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox12 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, 64.4));
		matrixBox12 = glm::rotate(matrixBox12, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox12);

		////PILAR IZQUIERDA2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox13 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, 2.8));
		matrixBox13 = glm::rotate(matrixBox13, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox13);

		//PILAR IZQUIERDA3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox14 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, 26.4));
		matrixBox14 = glm::rotate(matrixBox14, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox14);

		//PILAR IZQUIERDA4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox15 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, 46.4));
		matrixBox15 = glm::rotate(matrixBox15, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox15);




		//PILAR IZQUIERDA5 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox30 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, 64.4));
		matrixBox30 = glm::rotate(matrixBox30, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox30);


		//PILAR DERECHO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox16 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, 2.8));
		matrixBox16 = glm::rotate(matrixBox16, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox16);

		//PILAR derecho2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox17 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, 41.4));
		matrixBox17 = glm::rotate(matrixBox17, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox17);

		//PILAR derecho3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox18 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, 22.2));
		matrixBox18 = glm::rotate(matrixBox18, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox18);

		//PILAR derecho4 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox29 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, 41.4));
		matrixBox29 = glm::rotate(matrixBox29, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox29);

		//PILAR-DERECHA-H5 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox31 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 44.05, 41.89));
		matrixBox31 = glm::rotate(matrixBox31, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox31);

		//PILAR-DERECHA-H4 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox32 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 42.05, 41.89));
		matrixBox32 = glm::rotate(matrixBox32, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox32);


		//PILAR-DERECHA-H3 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox33 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 32.0, 41.89));
		matrixBox33 = glm::rotate(matrixBox33, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox33);

		//PILAR-DERECHA-H2 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox34 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 22.0, 41.89));
		matrixBox34 = glm::rotate(matrixBox34, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox34);

		//PILAR-DERECHA-H1 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox35 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 12.0, 41.89));
		matrixBox35 = glm::rotate(matrixBox35, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox35);

		//PILAR-IZQUIERDA-H5 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox36 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 44.1, 64.9));
		matrixBox36 = glm::rotate(matrixBox36, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox36);

		//PILAR-IZQUIERDA-H4 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox37 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 42.1, 64.9));
		matrixBox37 = glm::rotate(matrixBox37, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox37);

		//PILAR-IZQUIERDA-H3 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox38 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 32.0, 64.9));
		matrixBox38 = glm::rotate(matrixBox38, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox38);

		//PILAR-IZQUIERDA-H2 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox39 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 22.00, 64.9));
		matrixBox39 = glm::rotate(matrixBox39, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox39);

		//PILAR-IZQUIERDA-H1 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(0.4, 2.0, 25.2));
		glm::mat4 matrixBox40 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 12.00, 65.2));
		matrixBox40 = glm::rotate(matrixBox40, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox40);


		//VENTANA izquierda1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(22.0, 8.0, 0.1));
		glm::mat4 matrixBox60 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 37.0, 14.5));
		matrixBox60 = glm::rotate(matrixBox60, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox60);

		//VENTANA izquierda4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(22.0, 8.0, 0.1));
		glm::mat4 matrixBox63 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 27.0, 14.5));
		matrixBox63 = glm::rotate(matrixBox63, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox63);

		//VENTANA izquierda5- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(22.0, 8.0, 0.1));
		glm::mat4 matrixBox64 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 17.0, 14.5));
		matrixBox64 = glm::rotate(matrixBox64, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox64);

		//VENTANA izquierda6- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(22.0, 10.0, 0.1));
		glm::mat4 matrixBox65 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 06.0, 14.5));
		matrixBox65 = glm::rotate(matrixBox65, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox65);

		//VENTANA izquierda2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox61 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 37.0, 34.5));
		matrixBox61 = glm::rotate(matrixBox61, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox61);

		//VENTANA izquierda7- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox66 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 27.0, 34.5));
		matrixBox66 = glm::rotate(matrixBox66, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox66);

		//VENTANA izquierda8- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox67 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 17.0, 34.5));
		matrixBox67 = glm::rotate(matrixBox67, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox67);

		//VENTANA izquierda9- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 10.0, 0.1));
		glm::mat4 matrixBox68 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 6.0, 34.5));
		matrixBox68 = glm::rotate(matrixBox68, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox68);

		//VENTANA izquierda3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox62 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 37.0, 54.5));
		matrixBox62 = glm::rotate(matrixBox62, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox62);

		//VENTANA izquierda10- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox69 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 27.0, 54.5));
		matrixBox69 = glm::rotate(matrixBox69, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox69);

		//VENTANA izquierda11- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 8.0, 0.1));
		glm::mat4 matrixBox70 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 17.0, 54.5));
		matrixBox70 = glm::rotate(matrixBox70, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox70);

		//VENTANA izquierda12- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(21.0, 10.0, 0.1));
		glm::mat4 matrixBox71 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 6.0, 54.5));
		matrixBox71 = glm::rotate(matrixBox71, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox71);

		//VENTANA ENMEDIO13- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox72 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 37.0, 2.0));
		matrixBox72 = glm::rotate(matrixBox72, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox72);

		////VENTANA ENMEDIO16- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox75 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 27.0, 2.0));
		matrixBox75 = glm::rotate(matrixBox75, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox75);

		//VENTANA ENMEDIO17- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox76 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 17.0, 2.0));
		matrixBox76 = glm::rotate(matrixBox76, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox76);


		//VENTANA ENMEDIO14- nuevAi
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox73 = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0, 37.0, 2.0));
		matrixBox73 = glm::rotate(matrixBox73, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox73);

		//VENTANA ENMEDIO18- nuevAi
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox77 = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0, 27.0, 2.0));
		matrixBox77 = glm::rotate(matrixBox77, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox77);

		//VENTANA ENMEDIO19- nuevAi
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox78 = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0, 17.0, 2.0));
		matrixBox78 = glm::rotate(matrixBox78, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox78);

		//VENTANA ENMEDIO20- nuevAi
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.0, 12.0));
		glm::mat4 matrixBox79 = glm::translate(glm::mat4(1.0f), glm::vec3(-17.0, 6.0, 2.0));
		matrixBox79 = glm::rotate(matrixBox79, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox79);


		//VENTANA ENMEDIO15- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox74 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 37.0, 2.0));
		matrixBox74 = glm::rotate(matrixBox74, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox74);

		//VENTANA ENMEDIO21- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox80 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 27.0, 2.0));
		matrixBox80 = glm::rotate(matrixBox80, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox80);

		//VENTANA ENMEDIO22- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox81 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 17.0, 2.0));
		matrixBox81 = glm::rotate(matrixBox81, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox81);

		//VENTANA ENMEDIO23- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.0, 12.0));
		glm::mat4 matrixBox82 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 06.0, 2.0));
		matrixBox82 = glm::rotate(matrixBox82, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox82);


		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox59 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 38.0, 12.5));
		matrixBox59 = glm::rotate(matrixBox59, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox59);


		//VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox51 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 38.0, 32.0));
		matrixBox51 = glm::rotate(matrixBox51, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox51);

		//VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox52 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 28.0, 32.0));
		matrixBox52 = glm::rotate(matrixBox52, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox52);


		//VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox53 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 28.0, 12.5));
		matrixBox53 = glm::rotate(matrixBox53, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox53);

		//VENTANA DERECHA5- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox56 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 18.0, 12.5));
		matrixBox56 = glm::rotate(matrixBox56, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox56);

		//VENTANA DERECHA7- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox57 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 6.0, 12.5));
		matrixBox57 = glm::rotate(matrixBox57, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox57);


		//VENTANA DERECHA8- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox58 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 6.0, 32.0));
		matrixBox58 = glm::rotate(matrixBox58, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox58);


		//VENTANA DERECHA6- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox55 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 18.0, 32.0));
		matrixBox55 = glm::rotate(matrixBox55, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox55);


		//pilar  DH5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(36.9, 2.0, 1.0));
		glm::mat4 matrixBox19 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 44.0, 22.0));
		matrixBox19 = glm::rotate(matrixBox19, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox19);

		//PILAR DH4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(36.9, 2.0, 1.0));
		glm::mat4 matrixBox20 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 42.0, 22.0));
		matrixBox20 = glm::rotate(matrixBox20, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox20);


		//PILAR DH3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(36.9, 2.0, 1.0));
		glm::mat4 matrixBox21 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 32.0, 22.0));
		matrixBox21 = glm::rotate(matrixBox21, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox21);


		//PILAR DH2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(36.9, 2.0, 1.0));
		glm::mat4 matrixBox22 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 22.0, 22.0));
		matrixBox22 = glm::rotate(matrixBox22, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox22);

		//PILAR DH1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(36.9, 2.0, 1.0));
		glm::mat4 matrixBox23 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 12.0, 22.0));
		matrixBox23 = glm::rotate(matrixBox23, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox23);

		//PILAR IH5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(61.0, 2.0, 1.0));
		glm::mat4 matrixBox24 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 44.0, 34.5));
		matrixBox24 = glm::rotate(matrixBox24, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox24);

		//PILAR IH4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(61.0, 2.0, 1.0));
		glm::mat4 matrixBox25 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 42.0, 34.5));
		matrixBox25 = glm::rotate(matrixBox25, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox25);

		//PILAR IH3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(61.0, 2.0, 1.0));
		glm::mat4 matrixBox26 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 32.0, 34.5));
		matrixBox26 = glm::rotate(matrixBox26, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox26);

		//PILAR IH2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(61.0, 2.0, 1.0));
		glm::mat4 matrixBox27 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 22.0, 34.5));
		matrixBox27 = glm::rotate(matrixBox27, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox27);

		//PILAR IH1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(61.0, 2.0, 1.0));
		glm::mat4 matrixBox28 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 12.0, 34.5));
		matrixBox28 = glm::rotate(matrixBox28, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox28);

		////RAYADO DERECHA NUEVO4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox41 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 37.0, 42.2));
		matrixBox41 = glm::rotate(matrixBox41, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox41);

		//RAYADO DERECHA NUEVO3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox42 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 27.0, 42.2));
		matrixBox42 = glm::rotate(matrixBox42, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox42);

		//RAYADO DERECHA NUEVO2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox43 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 17.0, 42.2));
		matrixBox43 = glm::rotate(matrixBox43, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox43);

		//RAYADO DERECHA NUEVO1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 27.0));
		glm::mat4 matrixBox44 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 06.3, 42.2));
		matrixBox44 = glm::rotate(matrixBox44, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox44);

		//RAYADO IZQUIERDA NUEVO4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox45 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 37.0, 65.2));
		matrixBox45 = glm::rotate(matrixBox45, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox45);

		//RAYADO IZQUIERDA NUEVO3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox46 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 27.0, 65.2));
		matrixBox46 = glm::rotate(matrixBox46, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox46);

		//RAYADO IZQUIERDA NUEVO2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.5, 26.0));
		glm::mat4 matrixBox47 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 6.3, 65.2));
		matrixBox47 = glm::rotate(matrixBox47, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox47);

		//RAYADO IZQUIERDA NUEVO1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox48 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 17.0, 65.2));
		matrixBox48 = glm::rotate(matrixBox48, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox48);

		////Q
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureQID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 5.5, 5.5));
		glm::mat4 matrixBox49 = glm::translate(glm::mat4(1.0f), glm::vec3(19.0, 13.7, 42.5));
		matrixBox49 = glm::rotate(matrixBox49, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox49);


		//JARDINERA NUEVO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(12.75, 2.0, 48.0));
		box2.setScale(glm::vec3(2.5, 2.0, 6.0));
		box2.render();

		//JARDINERA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(16.75, 2.0, 50.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(22.0, 2.0, 50.0));
		box2.setScale(glm::vec3(5.0, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(27.0, 2.0, 50.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(32.0, 2.0, 50.0));
		box2.setScale(glm::vec3(5.0, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(37.2, 2.0, 50.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();


		//cambiar posición
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(27, 1.11, 45.5));
		box2.setScale(glm::vec3(26, 0.0, 7.0));
		box2.render();

		//JARDINERA DEL ARBOL

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(14.0, 2.0, 80.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(8.5, 2.0, 80.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(3.5, 2.0, 80.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-2.0, 2.0, 80.0));
		box2.setScale(glm::vec3(5.5, 2.0, 2.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-3.8, 2.0, 75.0));
		box2.setScale(glm::vec3(2.0, 2.0, 8.0));
		box2.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRocaID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-3.8, 2.0, 68.0));
		box2.setScale(glm::vec3(2.0, 2.0, 8.0));
		box2.render();


		//hojas nuevas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(13, 1.11, 10.0));
		box2.setScale(glm::vec3(3, 0.0, 10.0));
		box2.render();

		//hojas nuevas2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(13, 1.11, 20.0));
		box2.setScale(glm::vec3(3, 0.0, 10.0));
		box2.render();


		//hojas nuevas3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(13, 1.11, 30.0));
		box2.setScale(glm::vec3(3, 0.0, 10.0));
		box2.render();

		//hojas nuevas4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHojasID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(13, 1.11, 40.0));
		box2.setScale(glm::vec3(3, 0.0, 10.0));
		box2.render();



		//////////////////////////////laboratorio
		//pared donde ira la puerta
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedSalonID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 6.5, 25.9));
		box2.setScale(glm::vec3(26.0, 13.0, 0.0));
		box2.render();
		//pared izquierda vista desde puerta
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedSalonID);
		box3.setShader(&shaderLighting);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.setScale(glm::vec3(39.0, 13.0, 0.01));
		glm::mat4 matrixBoxJ21 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.5, 6.5, 45.4));
		matrixBoxJ21 = glm::rotate(matrixBoxJ21, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box3.render(matrixBoxJ21);
		//Pared derecha vista desde la puerta
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedSalonID);
		box3.setShader(&shaderLighting);
		box3.setProjectionMatrix(projection);
		box3.setViewMatrix(view);
		box3.setScale(glm::vec3(39.0, 13.0, 0.01));
		glm::mat4 matrixBoxJ22 = glm::translate(glm::mat4(1.0f), glm::vec3(-48.5, 6.5, 45.4));
		matrixBoxJ22 = glm::rotate(matrixBoxJ22, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box3.render(matrixBoxJ22);
		//pared enfrent de la puerta
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedSalonID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 6.5, 64.9));
		box2.setScale(glm::vec3(26.0, 13.0, 0.0));
		box2.render();
		//techo
		glBindTexture(GL_TEXTURE_2D, textureTirolID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 13.0, 45.4));
		box2.setScale(glm::vec3(25.0, 0.05, 39.0));
		box2.render();

		//piso
		glBindTexture(GL_TEXTURE_2D, texturePisoID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 1.3, 45.4));
		box2.setScale(glm::vec3(25.0, 0.1, 39.0));
		box2.render();

		//Puerta crital Derecha
		glm::mat4 matrixs12 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 6.0f, 2.0f));
		matrixs12 = glm::rotate(matrixs12, rot1, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 matrixc1 = glm::translate(matrixs12, glm::vec3(0.0, 0.0, 3.0));
		//matrixs12 = glm::rotate(matrixs12, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		matrixc1 = glm::scale(matrixc1, glm::vec3(0.0001, 10.0, 6.0));
		matrixs12 = glm::scale(matrixs12, glm::vec3(0.4f, 0.4f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, texturePuertaCristalID);
		boxWater.setShader(&shaderLighting);
		boxWater.setProjectionMatrix(projection);
		boxWater.setViewMatrix(view);
		boxWater.render(matrixc1);

		//Puerta crital Izquierda
		glm::mat4 matrixs2 = glm::translate(glm::mat4(1.0f), glm::vec3(-11.0f, 6.0f, 2.0f));
		matrixs2 = glm::rotate(matrixs2, -rot1, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 matrixc11 = glm::translate(matrixs2, glm::vec3(0.0, 0.0, 3.0));
		//matrixs12 = glm::rotate(matrixs12, 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		matrixc11 = glm::scale(matrixc11, glm::vec3(0.0001, 10.0, 6.0));
		matrixs2 = glm::scale(matrixs2, glm::vec3(0.4f, 0.4f, 0.4f));
		glBindTexture(GL_TEXTURE_2D, texturePuertaCristalID);
		boxWater.setShader(&shaderLighting);
		boxWater.setProjectionMatrix(projection);
		boxWater.setViewMatrix(view);
		boxWater.render(matrixc11);


		//PILAR DERECHA A
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox84 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, -48.5));
		matrixBox84 = glm::rotate(matrixBox84, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox84);


		//PILAR DERECHAA 2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox83 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, -48.5));
		matrixBox83 = glm::rotate(matrixBox83, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox83);


		////PILAR-DERECHA-H5 NuevoA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox85 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 44.05, -49.0));
		matrixBox85 = glm::rotate(matrixBox85, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox85);

		////PILAR-DERECHA-H4 NuevoA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox86 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 42.05, -49.0));
		matrixBox86 = glm::rotate(matrixBox86, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox86);


		////PILAR-DERECHA-H3 NuevoA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox87 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 32.0, -49.0));
		matrixBox87 = glm::rotate(matrixBox87, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox87);

		////PILAR-DERECHA-H2 NuevoA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox88 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 22.0, -49.0));
		matrixBox88 = glm::rotate(matrixBox88, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox88);

		//
		//PILAR-DERECHA-H1 NuevoA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox89 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 12.0, -49.0));
		matrixBox89 = glm::rotate(matrixBox89, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox89);

		//RAYADO DERECHA NUEVO4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox90 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 37.0, -49.0));
		matrixBox90 = glm::rotate(matrixBox90, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox90);

		//RAYADO DERECHA NUEVO3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox91 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 27.0, -49.0));
		matrixBox91 = glm::rotate(matrixBox91, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox91);

		//RAYADO DERECHA NUEVO2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox92 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 17.0, -49.0));
		matrixBox92 = glm::rotate(matrixBox92, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox92);

		//RAYADO DERECHA NUEVO1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 27.0));
		glm::mat4 matrixBox93 = glm::translate(glm::mat4(1.0f), glm::vec3(27.0, 06.3, -49.0));
		matrixBox93 = glm::rotate(matrixBox93, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox93);


		////pilar izquierdaA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox94 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, -35.5));
		matrixBox94 = glm::rotate(matrixBox94, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox94);

		//PILAR IZQUIERDAA Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox95 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, -35.5));
		matrixBox95 = glm::rotate(matrixBox95, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox95);

		//PILAR-IZQUIERDA-H5 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox97 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 44.1, -36.0));
		matrixBox97 = glm::rotate(matrixBox97, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox97);

		//////PILAR-IZQUIERDA-H4 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox96 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 42.1, -36.0));
		matrixBox96 = glm::rotate(matrixBox96, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox96);

		////PILAR-IZQUIERDA-H3 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox98 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 32.0, -36.0));
		matrixBox98 = glm::rotate(matrixBox98, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox98);

		////PILAR-IZQUIERDA-H2 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox99 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 22.00, -36.0));
		matrixBox99 = glm::rotate(matrixBox99, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox99);

		////PILAR-IZQUIERDA-H1 Nuevo
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(1.0, 2.0, 25.2));
		glm::mat4 matrixBox100 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 12.00, -36.0));
		matrixBox100 = glm::rotate(matrixBox100, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox100);

		//RAYADO IZQUIERDAA NUEVO4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox101 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 37.0, -36.0));
		matrixBox101 = glm::rotate(matrixBox101, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox101);

		//RAYADO IZQUIERDAA NUEVO3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox102 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 27.0, -36.0));
		matrixBox102 = glm::rotate(matrixBox102, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox102);

		//RAYADO IZQUIERDAA NUEVO2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox103 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 17.0, -36.0));
		matrixBox103 = glm::rotate(matrixBox103, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox103);

		//RAYADO IZQUIERDAA NUEVO1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturecrID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 26.0));
		glm::mat4 matrixBox104 = glm::translate(glm::mat4(1.0f), glm::vec3(-36.0, 7.0, -36.0));
		matrixBox104 = glm::rotate(matrixBox104, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox104);

		//////////////////////////AQUÍ VA OTRA VEZ LO NUEVO///////////////////////////////


		//pilar  II5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(100.0, 2.0, 1.0));
		glm::mat4 matrixBox131 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 44.05, 15.0));
		matrixBox131 = glm::rotate(matrixBox131, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox131);

		//////PILAR II4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(100.0, 2.0, 1.0));
		glm::mat4 matrixBox132 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 42.05, 15.0));
		matrixBox132 = glm::rotate(matrixBox132, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox132);


		//////PILAR II3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(100.0, 2.0, 1.0));
		glm::mat4 matrixBox133 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 32.05, 15.0));
		matrixBox133 = glm::rotate(matrixBox133, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox133);


		//////PILAR II2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(100.0, 2.0, 1.0));
		glm::mat4 matrixBox134 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 22.05, 15.0));
		matrixBox134 = glm::rotate(matrixBox134, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox134);


		//////PILAR II1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(100.0, 2.0, 1.0));
		glm::mat4 matrixBox135 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 12.05, 15.0));
		matrixBox135 = glm::rotate(matrixBox135, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox135);

		////pilar izquierda
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox136 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, 39.4));
		matrixBox136 = glm::rotate(matrixBox136, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox136);

		//////PILAR IZQUIERDA2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox137 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, 14.45));
		matrixBox137 = glm::rotate(matrixBox137, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox137);

		////PILAR IZQUIERDA3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox139 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 23.1, -10.525));
		matrixBox139 = glm::rotate(matrixBox139, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox139);


		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox140 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 38.0, 27));
		matrixBox140 = glm::rotate(matrixBox140, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox140);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox141 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 28.0, 27));
		matrixBox141 = glm::rotate(matrixBox141, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox141);

		//////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox142 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 18.0, 27));
		matrixBox142 = glm::rotate(matrixBox142, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox142);

		//////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox143 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 06.0, 27));
		matrixBox143 = glm::rotate(matrixBox143, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox143);


		//  //VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.5, 10.0, 0.1));
		glm::mat4 matrixBox144 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 38.0, 52.0));
		matrixBox144 = glm::rotate(matrixBox144, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox144);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.5, 10.0, 0.1));
		glm::mat4 matrixBox145 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 28.0, 52.0));
		matrixBox145 = glm::rotate(matrixBox145, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox145);

		//////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.5, 10.0, 0.1));
		glm::mat4 matrixBox146 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 18.0, 52.0));
		matrixBox146 = glm::rotate(matrixBox146, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox146);

		//////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.5, 10.0, 0.1));
		glm::mat4 matrixBox147 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 06.0, 52.0));
		matrixBox147 = glm::rotate(matrixBox147, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox147);


		//  //VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox148 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 38.0, 2.0));
		matrixBox148 = glm::rotate(matrixBox148, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox148);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox149 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 28.0, 2.0));
		matrixBox149 = glm::rotate(matrixBox149, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox149);

		//////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox150 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 18.0, 2.0));
		matrixBox150 = glm::rotate(matrixBox150, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox150);

		//////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(23.0, 10.0, 0.1));
		glm::mat4 matrixBox151 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 06.0, 2.0));
		matrixBox151 = glm::rotate(matrixBox151, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox151);


		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(26.0, 10.0, 0.1));
		glm::mat4 matrixBox152 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 38.0, -22.0));
		matrixBox152 = glm::rotate(matrixBox152, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox152);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(26.0, 10.0, 0.1));
		glm::mat4 matrixBox153 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 28.0, -22.0));
		matrixBox153 = glm::rotate(matrixBox153, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox153);

		//////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(26.0, 10.0, 0.1));
		glm::mat4 matrixBox154 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 18.0, -22.0));
		matrixBox154 = glm::rotate(matrixBox154, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox154);

		//////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(26.0, 10.0, 0.1));
		glm::mat4 matrixBox155 = glm::translate(glm::mat4(1.0f), glm::vec3(-49.0, 06.0, -22.0));
		matrixBox155 = glm::rotate(matrixBox155, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox155);



		//pilar  DDH5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(88.0, 2.0, 1.0));
		glm::mat4 matrixBox105 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 44.05, -3.5));
		matrixBox105 = glm::rotate(matrixBox105, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox105);

		////PILAR DH4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(88.0, 2.0, 1.0));
		glm::mat4 matrixBox106 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 42.05, -3.5));
		matrixBox106 = glm::rotate(matrixBox106, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox106);


		////PILAR DH3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(88.0, 2.0, 1.0));
		glm::mat4 matrixBox107 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 32.05, -3.5));
		matrixBox107 = glm::rotate(matrixBox107, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox107);


		////PILAR DH2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(88.0, 2.0, 1.0));
		glm::mat4 matrixBox108 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 22.05, -3.5));
		matrixBox108 = glm::rotate(matrixBox108, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox108);


		////PILAR DH1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(88.0, 2.0, 1.0));
		glm::mat4 matrixBox109 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 12.05, -3.5));
		matrixBox109 = glm::rotate(matrixBox109, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox109);

		//PILAR DERECHO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox110 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, 0.8));
		matrixBox110 = glm::rotate(matrixBox110, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox110);

		////PILAR derecho2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox111 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, 20.8));
		matrixBox111 = glm::rotate(matrixBox111, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox111);

		////PILAR derecho4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox112 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 23.1, -19.2));
		matrixBox112 = glm::rotate(matrixBox112, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox112);

		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(19.0, 10.0, 0.1));
		glm::mat4 matrixBox126 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 38.0, 31));
		matrixBox126 = glm::rotate(matrixBox126, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox126);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(19.0, 10.0, 0.1));
		glm::mat4 matrixBox127 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 28.0, 31));
		matrixBox127 = glm::rotate(matrixBox127, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox127);

		////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(19.0, 10.0, 0.1));
		glm::mat4 matrixBox128 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 18.0, 31));
		matrixBox128 = glm::rotate(matrixBox128, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox128);

		////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(19.0, 10.0, 0.1));
		glm::mat4 matrixBox129 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 06.0, 31));
		matrixBox129 = glm::rotate(matrixBox129, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox129);


		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox130 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 38.0, 10.5));
		matrixBox130 = glm::rotate(matrixBox130, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox130);


		//VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox114 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 28.0, 10.5));
		matrixBox114 = glm::rotate(matrixBox114, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox114);

		//VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox115 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 18.0, 10.5));
		matrixBox115 = glm::rotate(matrixBox115, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox115);

		//VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox117 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 06.0, 10.5));
		matrixBox117 = glm::rotate(matrixBox117, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox117);

		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox118 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 38.0, -9.3));
		matrixBox118 = glm::rotate(matrixBox118, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox118);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox119 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 28.0, -9.3));
		matrixBox119 = glm::rotate(matrixBox119, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox119);

		////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox120 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 18.0, -9.3));
		matrixBox120 = glm::rotate(matrixBox120, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox120);


		////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(18.0, 10.0, 0.1));
		glm::mat4 matrixBox121 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 06.0, -9.3));
		matrixBox121 = glm::rotate(matrixBox121, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox121);


		//VENTANA DERECHA1- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(27.5, 10.0, 0.1));
		glm::mat4 matrixBox122 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 38.0, -33.5));
		matrixBox122 = glm::rotate(matrixBox122, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox122);


		////VENTANA DERECHA2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(27.5, 10.0, 0.1));
		glm::mat4 matrixBox123 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 28.0, -33.5));
		matrixBox123 = glm::rotate(matrixBox123, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox123);

		//////VENTANA DERECHA3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(27.5, 10.0, 0.1));
		glm::mat4 matrixBox124 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 18.0, -33.5));
		matrixBox124 = glm::rotate(matrixBox124, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox124);


		//////VENTANA DERECHA4- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(27.5, 10.0, 0.1));
		glm::mat4 matrixBox125 = glm::translate(glm::mat4(1.0f), glm::vec3(40.0, 6.0, -33.5));
		matrixBox125 = glm::rotate(matrixBox125, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox125);

		//////////////////////////////////LO NUEVO///////////////////////////////
		//PILAR-FRONTAL
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-12.0, 23.1, -21.6));
		box2.setScale(glm::vec3(2.0, 44.0, 1.0));
		box2.render();

		////PILAR-FRONTAL2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(2.0, 23.1, -21.6));
		box2.setScale(glm::vec3(2.0, 44.0, 1.0));
		box2.render();

		////PILAR-H
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 12.0, -21.6));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		////PILAR-H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 22.0, -21.6));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		////PILAR-H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 32.0, -21.6));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		////PILAR-H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 42.0, -21.6));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//VENTANA ENMEDIO13- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox169 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 37.0, -22.0));
		matrixBox169 = glm::rotate(matrixBox169, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox169);

		////VENTANA ENMEDIO16- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox170 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 27.0, -22.0));
		matrixBox170 = glm::rotate(matrixBox170, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox170);

		//VENTANA ENMEDIO17- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox171 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 17.0, -22.0));
		matrixBox171 = glm::rotate(matrixBox171, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox171);

		//VENTANA ENMEDIO17- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.0, 12.0));
		glm::mat4 matrixBox172 = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0, 06.0, -22.0));
		matrixBox172 = glm::rotate(matrixBox172, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox172);

		//VENTANA ENMEDIO15- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox173 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 37.0, -22.0));
		matrixBox173 = glm::rotate(matrixBox173, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox173);

		//VENTANA ENMEDIO21- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox174 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 27.0, -22.0));
		matrixBox174 = glm::rotate(matrixBox174, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox174);

		//VENTANA ENMEDIO22- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 12.0));
		glm::mat4 matrixBox175 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 17.0, -22.0));
		matrixBox175 = glm::rotate(matrixBox175, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox175);

		//VENTANA ENMEDIO23- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.0, 12.0));
		glm::mat4 matrixBox176 = glm::translate(glm::mat4(1.0f), glm::vec3(8.0, 06.0, -22.0));
		matrixBox176 = glm::rotate(matrixBox176, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox176);

		//VENTANA ENMEDIO15- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 10.0));
		glm::mat4 matrixBox178 = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0, 37.0, -22.0));
		matrixBox178 = glm::rotate(matrixBox178, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox178);

		//VENTANA ENMEDIO21- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 10.0));
		glm::mat4 matrixBox179 = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0, 27.0, -22.0));
		matrixBox179 = glm::rotate(matrixBox179, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox179);

		//VENTANA ENMEDIO22- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 9.0, 10.0));
		glm::mat4 matrixBox180 = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0, 17.0, -22.0));
		matrixBox180 = glm::rotate(matrixBox180, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox180);

		//VENTANA ENMEDIO23- nuevAd
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(0.1, 10.0, 10.0));
		glm::mat4 matrixBox181 = glm::translate(glm::mat4(1.0f), glm::vec3(-18.0, 06.0, -22.0));
		matrixBox181 = glm::rotate(matrixBox181, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox181);

		//VENTANA izquierda3- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.0, 8.0, 0.1));
		glm::mat4 matrixBox182 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 37.0, -29.0));
		matrixBox182 = glm::rotate(matrixBox182, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox182);

		//VENTANA izquierda10- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.0, 8.0, 0.1));
		glm::mat4 matrixBox183 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 27.0, -29.0));
		matrixBox183 = glm::rotate(matrixBox183, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox183);

		//VENTANA izquierda11- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.0, 8.0, 0.1));
		glm::mat4 matrixBox184 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 17.0, -29.0));
		matrixBox184 = glm::rotate(matrixBox184, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox184);

		//VENTANA izquierda12- nuevA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.0, 10.0, 0.1));
		glm::mat4 matrixBox185 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 06.0, -29.0));
		matrixBox185 = glm::rotate(matrixBox185, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox185);

		////VENTANA izquierda2- nueva
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(11.0, 10.0, 0.1));
		glm::mat4 matrixBox186 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 06.0, -29.0));
		matrixBox186 = glm::rotate(matrixBox186, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox186);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(11.0, 10.0, 0.1));
		glm::mat4 matrixBox187 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 17.0, -29.0));
		matrixBox187 = glm::rotate(matrixBox187, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox187);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(11.0, 10.0, 0.1));
		glm::mat4 matrixBox188 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 27.0, -29.0));
		matrixBox188 = glm::rotate(matrixBox188, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox188);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(11.0, 10.0, 0.1));
		glm::mat4 matrixBox189 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 37.0, -29.0));
		matrixBox189 = glm::rotate(matrixBox189, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox189);


		//////VENTANA izquierda2- nueva

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.5, 10.0, 0.1));
		glm::mat4 matrixBox190 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 06.0, -41.0));
		matrixBox190 = glm::rotate(matrixBox190, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox190);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.5, 10.0, 0.1));
		glm::mat4 matrixBox191 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 17.0, -41.0));
		matrixBox191 = glm::rotate(matrixBox191, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox191);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.5, 10.0, 0.1));
		glm::mat4 matrixBox192 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 27.0, -41.0));
		matrixBox192 = glm::rotate(matrixBox192, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox192);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGlassID);
		box4.setShader(&shaderLighting);
		box4.setProjectionMatrix(projection);
		box4.setViewMatrix(view);
		box4.setScale(glm::vec3(13.5, 10.0, 0.1));
		glm::mat4 matrixBox193 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 37.0, -41.0));
		matrixBox193 = glm::rotate(matrixBox193, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box4.render(matrixBox193);







		////PILAR-H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-4.5, 44.0, -21.6));
		box2.setScale(glm::vec3(36.9, 2.0, 1.0));
		box2.render();

		//PILAR IH5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(12.0, 2.0, 1.0));
		glm::mat4 matrixBox156 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 44.0, -29.0));
		matrixBox156 = glm::rotate(matrixBox156, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox156);

		//PILAR IH4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(12.0, 2.0, 1.0));
		glm::mat4 matrixBox157 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 42.0, -29.0));
		matrixBox157 = glm::rotate(matrixBox157, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox157);

		////PILAR IH3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(12.0, 2.0, 1.0));
		glm::mat4 matrixBox158 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 32.0, -29.0));
		matrixBox158 = glm::rotate(matrixBox158, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox158);

		////PILAR IH2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(12.0, 2.0, 1.0));
		glm::mat4 matrixBox159 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 22.0, -29.0));
		matrixBox159 = glm::rotate(matrixBox159, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox159);

		////PILAR IH1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(12.0, 2.0, 1.0));
		glm::mat4 matrixBox160 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 12.0, -29.0));
		matrixBox160 = glm::rotate(matrixBox160, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox160);


		//PILAR DH5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(25.0, 2.0, 1.0));
		glm::mat4 matrixBox161 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 44.0, -35.0));
		matrixBox161 = glm::rotate(matrixBox161, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox161);

		//PILAR DH4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(25.0, 2.0, 1.0));
		glm::mat4 matrixBox162 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 42.0, -35.0));
		matrixBox162 = glm::rotate(matrixBox162, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox162);

		////PILAR DH3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(25.0, 2.0, 1.0));
		glm::mat4 matrixBox163 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 32.0, -35.0));
		matrixBox163 = glm::rotate(matrixBox163, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox163);

		////PILAR DH2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(25.0, 2.0, 1.0));
		glm::mat4 matrixBox164 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 22.0, -35.0));
		matrixBox164 = glm::rotate(matrixBox164, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox164);

		////PILAR DH1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(25.0, 2.0, 1.0));
		glm::mat4 matrixBox165 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 12.0, -35.0));
		matrixBox165 = glm::rotate(matrixBox165, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox165);


		//pilarf
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox166 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, -35.0));
		matrixBox166 = glm::rotate(matrixBox166, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox166);


		//PILARF2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox167 = glm::translate(glm::mat4(1.0f), glm::vec3(14.0, 23.1, -22.5));
		matrixBox167 = glm::rotate(matrixBox167, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox167);


		//PILARF3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureParedID);
		box2.setShader(&shaderLighting);
		box.setProjectionMatrix(projection);
		box.setViewMatrix(view);
		box.setScale(glm::vec3(2.0, 44.0, 1.0));
		glm::mat4 matrixBox168 = glm::translate(glm::mat4(1.0f), glm::vec3(-23.0, 23.1, -22.5));
		matrixBox168 = glm::rotate(matrixBox168, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		box.render(matrixBox168);


		//////////////////////////Laboratorio interior//////////////////////////////////

				//lado izquierdo
		float posx = -24.0, posz = 55, aumento = 0;
		for (int i = 0; i < 3; i++) {
			glBindTexture(GL_TEXTURE_2D, textureMaderaID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-28.0, 3.65, posz - aumento));
			box2.setScale(glm::vec3(9.0, 0.2, 2));
			box2.render();
			//columna 1
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-24.0, 4.5, posz - aumento));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-25.4, 4.5, posz - aumento));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-25.4, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-26.3, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();

			//columna2
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-26.8, 4.5, posz - aumento));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-28.2, 4.5, posz - aumento));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-28.2, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-29.1, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();

			//columna3
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-29.6, 4.5, posz - aumento));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-31.0, 4.5, posz - aumento));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-31.0, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-31.9, 3.8, (posz - 0.5) - aumento));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//patas de la mesa
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePatasID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-31.0, 2.5, posz - aumento));
			box2.setScale(glm::vec3(0.3, 2.2, 1.0));
			box2.render();

			aumento = aumento + 6;

		}
		//lado derecho
		float posx2 = -44.0, posz2 = 55, aumento2 = 0;
		for (int i = 0; i < 3; i++) {
			glBindTexture(GL_TEXTURE_2D, textureMaderaID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-44.0, 3.65, posz - aumento2));
			box2.setScale(glm::vec3(9.0, 0.2, 2));
			box2.render();

			//columna 1
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-40.0, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-41.4, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-41.4, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-42.3, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();

			//columna2
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-42.8, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-44.2, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-44.2, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-45.1, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();

			//columna3
			//cpu
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureCpuID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-45.6, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(0.5, 1.5, 1.5));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//monitor
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureAuxiliarID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-47.0, 4.5, posz - aumento2));
			box2.setScale(glm::vec3(2, 1.5, 0.1));
			box2.render(12, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//teclado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureTecladoID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-47.0, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(1.5, 0.05, 0.5));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();
			//mouse
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureMouseID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-47.9, 3.8, (posz - 0.5) - aumento2));
			box2.setScale(glm::vec3(0.1, 0.02, 0.15));
			box2.render(24, 6);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureNegroID);
			box2.render();

			//patas de la mesa
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePatasID);
			box2.setShader(&shaderLighting);
			box2.setProjectionMatrix(projection);
			box2.setViewMatrix(view);
			box2.setPosition(glm::vec3(-41.4, 2.5, posz - aumento2));
			box2.setScale(glm::vec3(0.3, 2.2, 1.0));
			box2.render();

			aumento2 = aumento2 + 6;
		}
		//sillas lado izquierdo
		float posz3 = 52.8, aumento3 = 0;
		for (int i = 0; i < 3; i++) {
			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla = glm::translate(glm::mat4(1.0f), glm::vec3(-26.8, 1.75, posz3 - aumento3));
			matrixSilla = glm::rotate(matrixSilla, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla);

			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla2 = glm::translate(glm::mat4(1.0f), glm::vec3(-29.6, 1.75, posz3 - aumento3));
			matrixSilla2 = glm::rotate(matrixSilla2, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla2);

			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla3 = glm::translate(glm::mat4(1.0f), glm::vec3(-32.4, 1.75, posz3 - aumento3));
			matrixSilla3 = glm::rotate(matrixSilla3, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla3);

			aumento3 = aumento3 + 6;
		}
		//sillas lado derecho
		float posz4 = 52.8, aumento4 = 0;
		for (int i = 0; i < 3; i++) {
			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla4 = glm::translate(glm::mat4(1.0f), glm::vec3(-42.6, 1.75, posz4 - aumento4));
			matrixSilla4 = glm::rotate(matrixSilla4, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla4);

			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla5 = glm::translate(glm::mat4(1.0f), glm::vec3(-45.4, 1.75, posz4 - aumento4));
			matrixSilla5 = glm::rotate(matrixSilla5, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla5);

			modelSilla.setShader(&shaderLighting);
			modelSilla.setProjectionMatrix(projection);
			modelSilla.setViewMatrix(view);
			modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
			glm::mat4 matrixSilla6 = glm::translate(glm::mat4(1.0f), glm::vec3(-48.2, 1.75, posz4 - aumento4));
			matrixSilla6 = glm::rotate(matrixSilla6, glm::radians(225.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			modelSilla.render(matrixSilla6);

			aumento4 = aumento4 + 6;
		}

		//Pizarron
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePizarronID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 6.5, 64.8));
		box2.setScale(glm::vec3(10, 5, 0.01));
		box2.render();

		//Proyector
		glBindTexture(GL_TEXTURE_2D, textureProyectorID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-36.0, 12.75, 45.4));
		box2.setScale(glm::vec3(1.55, 0.5, 1.5));
		box2.render(0, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBlancoID);
		box2.render();

		//escritorio profesor
		modelMesa.setShader(&shaderLighting);
		modelMesa.setProjectionMatrix(projection);
		modelMesa.setViewMatrix(view);
		modelMesa.setPosition(glm::vec3(-44.2, 2.3, 60.0));
		modelMesa.setScale(glm::vec3(3.0, 2.7, 1.0));
		modelMesa.render();

		//laptop profesor
		modelLaptop.setShader(&shaderLighting);
		modelLaptop.setProjectionMatrix(projection);
		modelLaptop.setViewMatrix(view);
		modelLaptop.setPosition(glm::vec3(-43.0, 3.5, 59.2));
		modelLaptop.setScale(glm::vec3(0.2, 0.2, 0.2));
		modelLaptop.render();
		//silla profesor
		modelSilla.setShader(&shaderLighting);
		modelSilla.setProjectionMatrix(projection);
		modelSilla.setViewMatrix(view);
		modelSilla.setScale(glm::vec3(2.0, 3.5, 2.0));
		glm::mat4 matrixSilla7 = glm::translate(glm::mat4(1.0f), glm::vec3(-42, 1.75, 62));
		matrixSilla7 = glm::rotate(matrixSilla7, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelSilla.render(matrixSilla7);

		//dispensador de agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureDispensadorID);
		box2.setShader(&shaderLighting);
		box2.setProjectionMatrix(projection);
		box2.setViewMatrix(view);
		box2.setPosition(glm::vec3(-24.0, 3.0, 58.0));
		box2.setScale(glm::vec3(1.0, 3.0, 1.0));
		box2.render(18, 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePizarronID);
		box2.render();
		//garrafon
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureGarrafonID);
		cylinder.setShader(&shaderTexture);
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.setPosition(glm::vec3(-24.0f, 5.5f, 58.0f));
		cylinder.setScale(glm::vec3(1.0, 2.0, 1.0));
		cylinder.render(0, cylinder.getSlices()* cylinder.getStacks() * 2 * 3);
		glBindTexture(GL_TEXTURE_2D, textureWaterID);
		//para la tapa superior
		cylinder.render(cylinder.getSlices()* cylinder.getStacks() * 2 * 3, cylinder.getSlices() * 3);
		//tapa inferior
		glBindTexture(GL_TEXTURE_2D, textureWaterID);
		cylinder.render(cylinder.getSlices()* cylinder.getStacks() * 2 * 3 + cylinder.getSlices() * 3, cylinder.getSlices() * 3);

		//brazo
		glm::mat4 matrixx1 = glm::translate(glm::mat4(1.0f), glm::vec3(27.2, 2.3, 31.8));
		glm::mat4 matrixs6 = glm::translate(matrixx1, glm::vec3(0.25f, 0.1f, 0.0f));
		matrixs6 = glm::rotate(matrixs6, rot0, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrix7 = glm::rotate(matrixs6, 1.6f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixs7 = glm::translate(matrix7, glm::vec3(0.5f, 0.0f, 0.0f));
		matrixs7 = glm::rotate(matrixs7, rot4, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrix8 = glm::rotate(matrixs7, -1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix8 = glm::translate(matrix8, glm::vec3(0.25f, 0.0f, 0.0f));
		glm::mat4 matrixs19 = glm::translate(matrix8, glm::vec3(0.3f, 0.0f, 0.0f));
		matrixs19 = glm::rotate(matrixs19, rot7, glm::vec3(0.0f, 0.0f, 1.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxm.setShader(&shaderLighting);
		glm::mat4 matrixd1 = glm::translate(matrixs19, glm::vec3(0.1f, 0.08, 0.0f));
		matrixd1 = glm::scale(matrixd1, glm::vec3(0.3f, 0.02f, 0.05f));
		boxm.setProjectionMatrix(projection);
		boxm.setViewMatrix(view);
		boxm.render(matrixd1);
		/*
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxm2.setShader(&shaderLighting);
		glm::mat4 matrixd2 = glm::translate(matrixs19, glm::vec3(0.1f, 0.0f, 0.0f));
		matrixd2 = glm::scale(matrixd2, glm::vec3(0.3f, 0.02f, 0.05f));
		boxm2.setProjectionMatrix(projection);
		boxm2.setViewMatrix(view);
		boxm2.render(matrixd2);
		*/

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxm3.setShader(&shaderLighting);
		glm::mat4 matrixd3 = glm::translate(matrixs19, glm::vec3(0.1f, -0.08, 0.0f));
		matrixd3 = glm::scale(matrixd3, glm::vec3(0.3f, 0.02f, 0.05f));
		boxm3.setProjectionMatrix(projection);
		boxm3.setViewMatrix(view);
		boxm3.render(matrixd3);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		sphere.setShader(&shaderLighting);
		matrixs19 = glm::scale(matrixs19, glm::vec3(0.01f, 0.01f, 0.01f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixs19);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxb2.setShader(&shaderTexture);
		boxb2.setProjectionMatrix(projection);
		boxb2.setViewMatrix(view);
		matrix8 = glm::scale(matrix8, glm::vec3(0.5f, 0.15f, 0.15f));
		boxb2.render(matrix8);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		sphere.setShader(&shaderLighting);
		matrixs7 = glm::scale(matrixs7, glm::vec3(0.01f, 0.01f, 0.01f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixs7);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxb1.setShader(&shaderTexture);
		boxb1.setProjectionMatrix(projection);
		boxb1.setViewMatrix(view);
		matrix7 = glm::translate(matrix7, glm::vec3(0.25f, 0.0f, 0.0f));
		matrix7 = glm::scale(matrix7, glm::vec3(0.5f, 0.15f, 0.15f));
		boxb1.render(matrix7);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureRojoID);
		sphere.setShader(&shaderLighting);
		matrixs6 = glm::scale(matrixs6, glm::vec3(0.01f, 0.01f, 0.01f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.render(matrixs6);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBrazoID);
		boxb1.setShader(&shaderTexture);
		boxb1.setProjectionMatrix(projection);
		boxb1.setViewMatrix(view);
		matrixx1 = glm::translate(matrixx1, glm::vec3(0.25f, 0.0f, 0.0f));
		matrixx1 = glm::scale(matrixx1, glm::vec3(0.5f, 0.15f, 0.15f));
		boxb1.render(matrixx1);


		//termina brazo

		/*
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWaterID);
		boxWater.setShader(&shaderLighting);
		boxWater.setProjectionMatrix(projection);
		boxWater.setViewMatrix(view);
		boxWater.setPosition(glm::vec3(3.0, 2.0, -5.0));
		boxWater.setScale(glm::vec3(10.0, 0.001, 10.0));
		boxWater.offsetUVS(glm::vec2(0.0001, 0.0001));
		boxWater.render();
		*/

		if (angle > 2 * M_PI)
			angle = 0.0;
		else
			angle += 0.001;

		sphere.setShader(&shaderColor);
		sphere.setColor(glm::vec3(0.4f, 0.3f, 0.6f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.setScale(glm::vec3(1.0f, 1.0f, 1.0f));
		sphere.enableWireMode();
		sphere.render(lightModelmatrix);

		// Se Dibuja el Skybox
		shaderCubeTexture.turnOn();
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTextureID);
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		GLuint cubeTextureId = shaderCubeTexture.getUniformLocation("skybox");
		glUniform1f(cubeTextureId, 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		sphere.setShader(&shaderCubeTexture);
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(glm::mat4(glm::mat3(view)));
		sphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));
		sphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		shaderCubeTexture.turnOff();

		

		if (finishRotation) {
			if (direcionAirCraft)
				aircraftZ -= 0.05;
			else
				aircraftZ += 0.05;
			if (direcionAirCraft && aircraftZ < -8.0) {
				direcionAirCraft = false;
				finishRotation = false;
				aircraftZ = -8.0;
			}if (!direcionAirCraft && aircraftZ > 8.0) {
				direcionAirCraft = true;
				finishRotation = false;
				aircraftZ = 8.0;
			}
		}
		else {
			rotationAirCraft += 0.05;
			if (!direcionAirCraft) {
				if (rotationAirCraft > glm::radians(180.0f)) {
					finishRotation = true;
					rotationAirCraft = glm::radians(180.0f);
				}
			}
			else {
				if (rotationAirCraft > glm::radians(360.0f)) {
					finishRotation = true;
					rotationAirCraft = glm::radians(0.0f);
				}
			}
		}

		if (direcionNano == 0 && cuartoVuelta == 0) {
			NanoZ += 0.04;
			if (direcionNano == 0 && NanoZ > 20.0) {
				direcionNano = 1;
				cuartoVuelta = 1;
				NanoZ = 20.0;
			}
		}
		if (direcionNano == 1 && cuartoVuelta == 1) {
			rotationNano += 0.03;
			if (rotationNano > glm::radians(90.0f)) {
				cuartoVuelta = 2;
				rotationNano = glm::radians(90.0f);
			}
		}
		if (direcionNano == 1 && cuartoVuelta == 2) {
			NanoX += 0.04;
			if (direcionNano == 1 && NanoX > 18.0) {
				direcionNano = 2;
				cuartoVuelta = 2;
				NanoX = 18.0;
			}
		}
		if (direcionNano == 2 && cuartoVuelta == 2) {
			rotationNano += 0.03;
			if (rotationNano > glm::radians(180.0f)) {
				cuartoVuelta = 3;
				rotationNano = glm::radians(180.0f);
			}
		}
		if (direcionNano == 2 && cuartoVuelta == 3) {
			NanoZ -= 0.04;
			if (direcionNano == 2 && NanoZ < 0.0) {
				direcionNano = 3;
				cuartoVuelta = 4;
				NanoZ = 0.0;
			}
		}
		if (direcionNano == 3 && cuartoVuelta == 4) {
			rotationNano += 0.03;
			if (rotationNano > glm::radians(270.0f)) {
				cuartoVuelta = 5;
				rotationNano = glm::radians(270.0f);
			}
		}
		if (direcionNano == 3 && cuartoVuelta == 5) {
			NanoX -= 0.04;
			if (direcionNano == 3 && NanoX < 0.0) {
				direcionNano = 0;
				cuartoVuelta = 6;
				NanoX = 0.0;
			}
		}
		if (direcionNano == 0 && cuartoVuelta == 6) {
			rotationNano += 0.03;
			if (rotationNano > glm::radians(360.0f)) {
				cuartoVuelta = 0;
				rotationNano = glm::radians(0.0f);
			}
		}
		//brincos eje Y
		if (Brincar) {
			if (Salto)
				saltoY -= 0.07;
			else
				saltoY += 0.07;
			if (Salto && saltoY < 0.0) {
				Salto = false;
				saltoY = 0.0;
			}if (!Salto && saltoY > 2.0) {
				Salto = true;
				saltoY = 2.0;
			}
		}


		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "========Zenteno Vision========", false);
	applicationLoop();
	destroy();
	return 1;
}