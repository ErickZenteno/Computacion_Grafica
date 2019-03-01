//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shaders code
const GLchar* vertexShaderSource = { "#version 400\n"

"layout(location=0) in vec3 position;\n"
"layout(location=1) in vec3 color;\n"
"out vec3 ourColor;\n"

"void main(void)\n"
"{\n"
"  gl_Position = vec4(position, 1.0);\n"
"  ourColor = color;\n"
"}\n" };
const GLchar* fragmentShaderSource = { "#version 400\n"

"in vec3 ourColor;\n"
"out vec4 out_Color;\n"

"void main(void)\n"
"{\n"
"  out_Color = vec4(ourColor, 1.0);\n"
"}\n" };


bool render1 = true;

GLuint VBO, VAO, VBO2, VAO2;
GLint vertexShader, fragmentShader, shaderProgram;

typedef struct {
	float XYZ[3];
	float RGB[3];
} Vertex;

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

	// Build and compile our shader program
	// Vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check for compile time errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check for compile time errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog
			<< std::endl;
	}
	// Link shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
			<< std::endl;
	}

	Vertex vertices[] =
	{
		//lado izquierdo
		{ {-0.6f, -0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {-0.9f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ {-0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo dos
		{ {-0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ {-0.9f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ {-0.6f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//triangulo tres
		{ {-0.6f, -0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ {-0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo cuatro
		{ {-0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ {-0.6f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//lado derecho
		{ { 0.6f, -0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.9f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo dos
		{ { 0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ { 0.9f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.6f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//triangulo tres
		{ { 0.6f, -0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo cuatro
		{ { 0.6f, 0.0f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.6f, 0.2f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//TRIANGULO INFERIOR
		{ {-0.2f, -0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, -0.9f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, -0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo dos
		{ { 0.2f, -0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, -0.9f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, -0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo tres
		{ {-0.2f, -0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, -0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		//triangulo cuatro
		{ { 0.2f, -0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, -0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
				//TRIANGULO SUPERIOR
		{ {-0.2f, 0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.9f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo dos
		{ { 0.2f, 0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.9f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		//triangulo tres
		{ {-0.2f, 0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		//triangulo cuatro
		{ { 0.2f, 0.6f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 0.0f, 0.0f, 0.0f } ,{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, 0.6f, 0.0f } ,{ 0.0f, 0.0f, 1.0f } },
	};

	Vertex vertices2[] =
	{
<<<<<<< HEAD
=======
		//piso
		{ { -1.0f, -0.73f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ {  1.0f, -0.73f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ {  1.0f, -0.7f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ {  1.0f, -0.7f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -1.0f, -0.7f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -1.0f, -0.73f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		//Cuerpo de la casa
		{ { -0.5f, -0.7f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		{ { 0.5f, -0.7f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		{ { 0.5f, 0.3f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		{ { 0.5f, 0.3f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		{ { -0.5f, 0.3f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		{ { -0.5f, -0.7f, 0.0f } ,{ 0.5f, 1.0f, 1.0f } },
		//Marco de Puerta
		{ { -0.45f, -0.7f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		{ { -0.05f, -0.7f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		{ { -0.05f, 0.05f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		{ { -0.05f, 0.05f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		{ { -0.45f, 0.05f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		{ { -0.45f, -0.7f, 0.0f } ,{ 0.1f, 0.0f, 0.0f } },
		//Puerta
		{ { -0.4f, -0.65f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ { -0.1f, -0.65f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ { -0.1f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ { -0.1f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ { -0.4f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ { -0.4f, -0.65f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
<<<<<<< HEAD
=======
		//Picaporte
		{ { -0.17f, -0.3f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -0.13f, -0.3f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -0.13f, -0.35f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -0.13f, -0.35f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -0.17f, -0.35f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		{ { -0.17f, -0.3f, 0.0f } ,{ 0.23f, 0.08f, 0.0f } },
		//chimenea entrada
		{ {  0.45f, 0.63f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.25f, 0.63f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.25f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.25f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.45f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.45f, 0.63f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//Chimenea cuerpo
		{ {  0.27f, 0.3f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
		{ {  0.42f, 0.3f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
		{ {  0.42f, 0.63f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
		{ {  0.42f, 0.63f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
		{ {  0.27f, 0.63f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
		{ {  0.27f, 0.3f, 0.0f } ,{ 0.6f, 0.0f, 0.0f } },
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		//Marquesina
		{ { -0.5f, 0.1f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		{ {  0.5f, 0.1f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		{ {  0.5f, 0.3f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		{ {  0.5f, 0.3f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		{ { -0.5f, 0.3f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		{ { -0.5f, 0.1f, 0.0f } ,{ 0.1f, 0.1f, 0.0f } },
		//Atico
		{ { -0.5f, 0.3f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.5f, 0.3f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.0f, 0.7f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
<<<<<<< HEAD
=======
		//Marco ventana atico
		{ { -0.07f, 0.4f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		{ {  0.07f, 0.4f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		{ {  0.07f, 0.6f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		{ {  0.07f, 0.6f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		{ { -0.07f, 0.6f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		{ { -0.07f, 0.4f, 0.0f } ,{ 1.0f, 1.0f, 1.0f } },
		//ventana atico
		{ { -0.05f, 0.42f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.05f, 0.42f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.05f, 0.58f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.05f, 0.58f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ { -0.05f, 0.58f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ { -0.05f, 0.42f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		//Techo izquierdo
		{ { -0.6f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { -0.55f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.75f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { -0.6f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//Techo derecho
		{ {  0.6f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.55f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.7f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.0f, 0.75f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ {  0.6f, 0.25f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		//Marco ventana
		{ {  0.4f, -0.4f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.05f, -0.4f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.05f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.05f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.4f, 0.0f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		{ {  0.4f, -0.4f, 0.0f } ,{ 1.0f, 0.5f, 0.0f } },
		//Ventana
		{ {  0.38f, -0.38f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.07f, -0.38f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.07f, -0.02f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.07f, -0.02f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.38f, -0.02f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		{ {  0.38f, -0.38f, 0.0f } ,{ 0.0f, 0.5f, 1.0f } },
		//Barrote horizontal
		{ {  0.07f, -0.12f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		{ {  0.38f, -0.12f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		{ {  0.38f, -0.1f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		{ {  0.38f, -0.1f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		{ {  0.07f, -0.1f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		{ {  0.07f, -0.12f, 0.0f } ,{ 1.0f, 0.5f, 1.0f } },
		//Barrote vertical
		{ {  0.215f, -0.38f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		{ {  0.235f, -0.38f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
<<<<<<< HEAD
		{ {  0.235f, -0.02f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		{ {  0.235f, -0.02f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		{ {  0.215f, -0.02f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
=======
		{ {  0.235f, -0.12f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		{ {  0.235f, -0.12f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		{ {  0.215f, -0.12f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		{ {  0.215f, -0.38f, 0.0f } ,{ 1.0f, 1.5f, 0.0f } },
		//Barra ventana
		{ {  0.45f, -0.5f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },
		{ {  0.0f, -0.5f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },
		{ {  0.0f, -0.4f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },
		{ {  0.0f, -0.4f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },
		{ {  0.45f, -0.4f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },
		{ {  0.45f, -0.5f, 0.0f } ,{ 0.0f, 0.1f, 0.3f } },

<<<<<<< HEAD
=======

	};

	Vertex vertices2[] =
	{
		{ { 0.0f, 0.0f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 0.0f } ,{ 1.0f, 0.0f, 0.0f } }
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
	};

	const size_t bufferSize = sizeof(vertices);
	const size_t vertexSize = sizeof(vertices[0]);
	const size_t rgbOffset = sizeof(vertices[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize << std::endl;
	std::cout << "Vertex Size:" << vertexSize << std::endl;
	std::cout << "Buffer size:" << rgbOffset << std::endl;
	
	//esto es para el primer tirnagulo
	glGenBuffers(1, &VBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize,
		(GLvoid*)rgbOffset);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//esto es para el segundo tirnagulo

	const size_t bufferSize2 = sizeof(vertices2);
	const size_t vertexSize2 = sizeof(vertices2[0]);
	const size_t rgbOffset2 = sizeof(vertices2[0].XYZ);

	std::cout << "Buffer Size:" << bufferSize2 << std::endl;
	std::cout << "Vertex Size:" << vertexSize2 << std::endl;
	std::cout << "Buffer size:" << rgbOffset2 << std::endl;
	glGenBuffers(1, &VBO2);

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, bufferSize2, vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize2, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize2,
		(GLvoid*)rgbOffset2);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	glUseProgram(0);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
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
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		case GLFW_KEY_E:
			render1 = true;
			break;
		case GLFW_KEY_C:
<<<<<<< HEAD
=======
=======
		case GLFW_KEY_F:
			render1 = true;
			break;
		case GLFW_KEY_S:
>>>>>>> a75ac869e3e4c1516ece79dce49f1185b864568d
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
			render1 = false;
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

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		glUseProgram(shaderProgram);
		if(render1) {
			glBindVertexArray(VAO);
<<<<<<< HEAD
=======
<<<<<<< HEAD
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
			glDrawArrays(GL_TRIANGLES, 0, 48);
		}
		else {
			glBindVertexArray(VAO2);
<<<<<<< HEAD
			glDrawArrays(GL_TRIANGLES, 0, 90);
=======
			glDrawArrays(GL_TRIANGLES, 0, 105);
=======
			//si queremos manejar mas tirnagulos debemos cabiar el 3 por el numero de vertices
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}
		else {
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 3);
>>>>>>> a75ac869e3e4c1516ece79dce49f1185b864568d
>>>>>>> bb5b59dc08fa2a95e9ad34f6bb24c1baf293ac99
		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}

