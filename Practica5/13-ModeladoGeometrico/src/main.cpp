//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
// Geometry primitives
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"

Sphere sphere(20, 20);
Cylinder cylinder(20, 20, 0.5, 0.5);
Box box;

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader shader;


float movZ = -8.0f;
float movX = 0.0f;
float movY = 0.0f;


int screenWidth;
int screenHeight;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

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

	shader.initialize("../../Shaders/transformaciones.vs", "../../Shaders/transformaciones.fs");

	sphere.init();
	sphere.setShader(&shader);
	sphere.setColor(glm::vec3(0.3, 0.3, 1.0));

	cylinder.init();
	cylinder.setShader(&shader);
	cylinder.setColor(glm::vec3(0.8, 0.3, 1.0));

	box.init();
	box.setShader(&shader);
	box.setColor(glm::vec3(0.2, 0.8, 0.4));

}

void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();

	shader.destroy();
	sphere.destroy();
	cylinder.destroy();
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
		case GLFW_KEY_UP:
			movY += 0.5f;//Arriba 
			break;
		case GLFW_KEY_DOWN:
			movY -= 0.5f;//Abajo
			break;
		case GLFW_KEY_RIGHT:
			movX += 0.5f;//Derecha 
			break;
		case GLFW_KEY_LEFT:
			movX -= 0.5f;//Izquierda
			break;
		case GLFW_KEY_I:
			movZ += 0.5f;//LEJOS 
			break;
		case GLFW_KEY_O:
			movZ -= 0.5f;//CERCA
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
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
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		psi = processInput(true);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//matriz de proyeccion en perspectiva
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / screenWidth, 0.01f, 100.0f);
		//matriz de proyeccion en vista
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));

		//matriz con diagonal unitaria
		//matriz del cilindro del torso
		glm::mat4 matrix0 = glm::mat4(1.0f);
		//se coloca el toso en la coordenada 0.0 0.0 -1.0
		matrix0 = glm::translate(matrix0, glm::vec3(0.0f, 0.0f, -1.0f));
		//matriz de la esfera 1 se colo en -0.5 unidades en el eje Y debajo del torso
		glm::mat4 matrixs1 = glm::translate(matrix0, glm::vec3(0.0f, -0.5f, 0.0f));


		glm::mat4 matrixs5 = glm::translate(matrix0, glm::vec3(0.0f, 0.5f, 0.0f));

		//Brazo derecho
		glm::mat4 matrixs6 = glm::translate(matrixs5, glm::vec3(0.3f, 0.0f, 0.0f));
		glm::mat4 matrix7 = glm::rotate(matrixs6, -0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixs7 = glm::translate(matrix7, glm::vec3(0.5f, 0.0f, 0.0f));
		glm::mat4 matrix8 = glm::rotate(matrixs7, -1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix8 = glm::translate(matrix8, glm::vec3(0.25f, 0.0f, 0.0f));

		glm::mat4 matrixs12 = glm::translate(matrix8, glm::vec3(0.3f, 0.0f, 0.0f));
		matrixs12 = glm::scale(matrixs12, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs12);

		matrix8 = glm::scale(matrix8, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix8);


		matrixs7 = glm::scale(matrixs7, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs7);


		matrix7 = glm::translate(matrix7, glm::vec3(0.25f, 0.0f, 0.0f));
		matrix7 = glm::scale(matrix7, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix7);

		matrixs6 = glm::scale(matrixs6, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs6);
		//termina el brazo derecho

		//Brazo izquierdo

		glm::mat4 matrixs9 = glm::translate(matrixs5, glm::vec3(-0.3f, 0.0f, 0.0f));
		glm::mat4 matrix9 = glm::rotate(matrixs9, 0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 matrixs8 = glm::translate(matrix9, glm::vec3(-0.5f, 0.0f, 0.0f));
		glm::mat4 matrix10 = glm::rotate(matrixs8, -1.5f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix10 = glm::translate(matrix10, glm::vec3(0.25f, 0.0f, 0.0f));

		glm::mat4 matrixs11 = glm::translate(matrix10, glm::vec3(0.3f, 0.0f, 0.0f));
		matrixs11 = glm::scale(matrixs11, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs11);


		matrix10 = glm::scale(matrix10, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix10);


		matrixs8 = glm::scale(matrixs8, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs8);


		matrix9 = glm::translate(matrix9, glm::vec3(-0.25f, 0.0f, 0.0f));
		matrix9 = glm::scale(matrix9, glm::vec3(0.5f, 0.15f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix9);


		matrixs9 = glm::scale(matrixs9, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs9);


		//termina el brazo izquierdo

		//cabeza
		glm::mat4 matrix12 = glm::translate(matrixs5, glm::vec3(0.0f, 0.3f, 0.0f));
		matrix12 = glm::scale(matrix12, glm::vec3(0.4f, 0.5f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix12);



		matrixs5 = glm::scale(matrixs5, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs5);

		//se escala el cilindro del torso en 6 unidades
		matrix0 = glm::scale(matrix0, glm::vec3(0.6f, 1.0f, 0.6f));
		//se dibuja el cilindro
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix0);


		glm::mat4 matrixs2 = glm::translate(matrixs1, glm::vec3(-0.225f, 0.0f, 0.0f));
		glm::mat4 matrixs3 = glm::translate(matrixs1, glm::vec3(0.225f, 0.0f, 0.0f));
		matrixs1 = glm::scale(matrixs1, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs1);


		//pierna izquierda inicio
		glm::mat4 matrix1 = glm::rotate(matrixs2, -0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix1 = glm::translate(matrix1, glm::vec3(0.0, -0.4, 0.0));

		glm::mat4 matrixs4 = glm::translate(matrix1, glm::vec3(0.0f, -0.4f, 0.0f));

		glm::mat4 matrix2 = glm::rotate(matrixs4, 0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix2 = glm::translate(matrix2, glm::vec3(0.0f, -0.3f, 0.0f));


		glm::mat4 matrixs15 = glm::translate(matrix2, glm::vec3(0.0f, -0.35f, 0.0f));
		matrixs15 = glm::scale(matrixs15, glm::vec3(0.1f, 0.1f, 0.3f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs15);

		matrix2 = glm::scale(matrix2, glm::vec3(0.15, 0.6, 0.15));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix2);

		matrixs4 = glm::scale(matrixs4, glm::vec3(0.1f, 0.1f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs4);

		matrix1 = glm::scale(matrix1, glm::vec3(0.15f, 0.8f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix1);
		//pierna izquierda fin


		matrixs2 = glm::scale(matrixs2, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs2);


		//pierna derecha inicio

		glm::mat4 matrix13 = glm::rotate(matrixs3, 0.2f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix13 = glm::translate(matrix13, glm::vec3(0.0, -0.4, 0.0));

		glm::mat4 matrixs13 = glm::translate(matrix13, glm::vec3(0.0f, -0.4f, 0.0f));

		glm::mat4 matrix14 = glm::rotate(matrixs13, -0.3f, glm::vec3(0.0f, 0.0f, 1.0f));
		matrix14 = glm::translate(matrix14, glm::vec3(0.0f, -0.3f, 0.0f));


		glm::mat4 matrixs16 = glm::translate(matrix14, glm::vec3(0.0f, -0.35f, 0.0f));
		matrixs16 = glm::scale(matrixs16, glm::vec3(0.1f, 0.1f, 0.3f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs16);



		matrix14 = glm::scale(matrix14, glm::vec3(0.15, 0.6, 0.15));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix14);

		matrixs13 = glm::scale(matrixs13, glm::vec3(0.1f, 0.1f, 0.2f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs13);

		matrix13 = glm::scale(matrix13, glm::vec3(0.15f, 0.8f, 0.15f));
		cylinder.setProjectionMatrix(projection);
		cylinder.setViewMatrix(view);
		cylinder.enableWireMode();
		cylinder.render(matrix13);

		//pierna derecha fin

		matrixs3 = glm::scale(matrixs3, glm::vec3(0.1f, 0.1f, 0.1f));
		sphere.setProjectionMatrix(projection);
		sphere.setViewMatrix(view);
		sphere.enableWireMode();
		sphere.render(matrixs3);



		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}