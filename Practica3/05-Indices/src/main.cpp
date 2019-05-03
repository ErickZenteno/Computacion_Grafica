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

GLuint VBO, VAO, EBO;
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
void creaRec();
void creaEstrella();

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
	
	creaEstrella();
}

void creaEstrella() {
	Vertex vertices[] =
	{
		//cabeza superior trasera
		{ { -0.51f, 0.6f, 0.0f} , { 1.0f, 0.8f, 1.0f} },//0C
		{ { -0.15f, 0.53f, 0.0f } , { 1.0f, 0.8f, 1.0f} },//1D
		{ { -0.04f , 0.73f, 0.0f } , { 1.0f, 0.6f, 0.8f} },//2E
		{ { -0.28f , 0.76f , 0.0f } , { 1.0f, 0.6f, 0.8f} },//3F

		//cabeza superior frontal
		{ { -0.77f , 0.2f , 0.0f } , { 1.0f, 0.6f, 0.8f} },//4G
		{ { -0.49f, 0.11f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//5H
		{ { -0.35f , 0.3f, 0.0f } , { 1.0f, 0.6f, 0.8f} },//6I
		{ { -0.31f , 0.49f , 0.0f } , { 1.0f, 0.6f, 0.8f} },//7J
		{ { -0.57f , 0.14f , 0.0f } , { 1.0f, 0.6f, 0.8f} },//8K

		{ { -0.77f , 0.2f , 0.0f } , { 0.8f, 1.0f, 1.0f} },//9G
		{ { -0.57f , 0.14f , 0.0f } , { 0.6f, 0.8f, 1.0f} },//10K
		{ { -0.49f, 0.11f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//11H
		{ { -0.83f, -0.01f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//12L
		{ { -0.85f , -0.15f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//13M
		{ { -0.69f , -0.2f , 0.0f } , { 0.6f, 0.8f, 1.0f} },//14N


		{ { -0.67f , -0.21f , 0.0f } , { 0.4f, 0.5f, 0.5f} },//15O
		{ { -0.41f, 0.06f, 0.0f } , { 0.4f, 0.5f, 0.5f} },//16P
		{ { -0.36f , -0.09f, 0.0f } , { 0.2f, 0.25f, 0.5f} },//17Q



		{ { -0.49f, 0.11f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//18H
		{ { -0.41f, 0.06f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//19P
		{ { -0.27f, 0.21f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//20R
		{ { -0.35f , 0.3f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//21I


		{ { -0.35f , 0.3f, 0.0f } , { 1.0f, 1.0f, 1.0f} },//22I
		{ { -0.15f, 0.53f, 0.0f } , { 1.0f, 1.0f, 1.0f} },//23D
		{ { -0.31f , 0.49f , 0.0f } , { 1.0f, 1.0f, 1.0f} },//24J
		{ { -0.27f, 0.21f, 0.0f } , { 0.6f, 0.6f, 0.6f} },//25R
		{ { -0.09f, 0.24f, 0.0f } , { 0.6f, 0.6f, 0.6f} },//26S
		{ { -0.03f, 0.42f, 0.0f } , { 0.6f, 0.6f, 0.6f} },//27T


		{ { -0.41f, 0.06f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//28P
		{ { -0.36f , -0.09f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//29Q
		{ { -0.27f, 0.21f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//30R
		{ { -0.09f, 0.24f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//31S
		{ { -0.03f, 0.42f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//32T
		{ { -0.16f , -0.01f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//33U
		{ { 0.1f , 0.26f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//34V
		{ { 0.15f , 0.51f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//35W

		{ { -0.03f, 0.42f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//36T
		{ { 0.15f , 0.51f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//37W
		{ { -0.04f , 0.73f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//38E
		{ { -0.15f, 0.53f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//39D

	
		{ { -0.02f , 0.09f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//40B1
		{ { 0.22f , 0.16f, 0.0f } , { 1.0f, 0.8f, 1.0f} },//41F1
		{ { 0.02f, 0.18f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//42E1
		{ { -0.05f, 0.11f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//43A1
		{ { 0.4f , 0.21f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//44C1
		{ { 0.34f, 0.21f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//45D1

		
		{ { -0.02f , 0.09f, 0.0f } , { 0.2f, 0.25f, 0.5f} },//46B1
		{ { 0.03f , -0.16f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//47I1
		{ { 0.07f, -0.02f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//48J1
		{ { 0.09f, -0.45f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//49G1
		{ { 0.17f , -0.12f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//50H1


		{ { -0.16f , -0.01f, 0.0f } , { 0.2f, 0.25f, 0.5f} },//51U
		{ { -0.27f , -0.05f, 0.0f } , { 0.2f, 0.25f, 0.5f} },//52K1
		{ { -0.33f , -0.08f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//53L1
		{ { -0.44f , -0.36f, 0.0f } , { 0.8f, 1.0f, 1.0f} },//54M1


		{ { -0.44f , -0.36f, 0.0f } , { 0.4f, 0.2f, 0.8f} },//55M1
		{ { -0.05f , -0.59f, 0.0f } , { 0.4f, 0.2f, 0.8f} },//56N1
		{ { 0.19f , -0.66f, 0.0f } , { 0.6f, 0.8f, 1.0f} },//57O1

		{ { 0.09f, -0.45f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//58G1
		{ { 0.19f , -0.66f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//59O1
		{ { 0.28f , -0.54f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//60P1
		{ { 0.36f , -0.13f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//61Q1

		{ { 0.55f , -0.06f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//62R1
		{ { 0.54f , 0.09f, 0.0f } , { 0.5f, 0.05f, 0.1f} },//63S1

		{ { 0.54f , 0.09f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//64S1
		{ { 0.09f, -0.45f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//65G1
		{ { 0.17f , -0.12f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//66H1
		{ { -0.02f , 0.09f, 0.0f } , { 0.5f, 0.1f, 0.2f} },//67B1
		{ { 0.4f , 0.21f, 0.0f } , { 1.0f, 0.1f, 0.2f} },//68C1


		


		//ojo
		{ { -0.2f, 0.36f, 0.0f } , { 0.0f, 0.0f, 0.0f} },//?Z


	};

	GLuint indices[] = {
		//cabeza superior trasera
		0, 1, 2,
		2, 3, 0,
		//cabeza superior frontal
		0, 7, 1,
		4, 7, 0,
		4, 6, 7,
		8, 5, 6,
		4, 8, 6,
		//pico superior
		9, 12, 10,
		12, 13, 10,
		13, 11, 10,
		13, 14, 11,
		//pido lateral
		14, 16, 11,
		14, 16, 15,
		//pico lateral inferior
		15, 17, 16,
		//cabeza lateral delantera
		18, 19, 20,
		20, 21, 18,
		//ojo
		22,23,24,
		22,25,27,
		27,22,23,
		25,26,27,
		//cabeza lado inferior
		28,29,30,
		29,33,30,
		33,31,30,
		33,34,31,
		34,31,32,
		34,35,32,
		//cabeza lateral trasera
		36,37,38,
		38,39,36,
		//lomo
		40,41,42,
		42,43,40,
		41,44,45,
		45,42,41,
		//triangulo lateral
		46,47,48,
		47,49,50,
		50,48,47,
		//pecho
		53,54,49,
		53,49,47,
		47,52,53,
		51,52,47,
		51,47,46,
		40,43,51,
		//pecho inferior
		56,57,49,
		49,55,56,
		//Lateral inferior frontal
		58,59,60,
		60,61,58,
		//Lateral inferior trasero
		61,62,63,
		//Lateral superior
		67,64,68,
		64,67,66,
		65,64,66,
		//



	};

	const size_t VertexSize = sizeof(vertices);
	const size_t StrideSize = sizeof(vertices[0]);
	const size_t OffsetPos = sizeof(vertices[0].XYZ);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, StrideSize, 0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, StrideSize, (GLvoid*)OffsetPos);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void creaRec() {
	Vertex vertices[] =
	{
		//centro 
		{ { -0.5f, -0.5f, 0.0f } , { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f , -0.5f, 0.0f } , { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f , 0.5f , 0.0f } , { 0.0f, 0.0f, 1.0f } },
		{ {-0.5f , 0.5f , 0.0f } , { 1.0f, 0.0f, 1.0f } },
	};

	GLuint indices[] = {
		0 , 1, 2,
		0 , 2, 3
	};

	const size_t VertexSize = sizeof(vertices);
	const size_t StrideSize = sizeof(vertices[0]);
	const size_t OffsetPos = sizeof(vertices[0].XYZ);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexSize, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, StrideSize, 0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, StrideSize, (GLvoid*) OffsetPos);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EBO);

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
		
		// Draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// This is for the render with index element
		//primitiva, num indices, tipo de dato, apuntador a inicio de datos
		glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
		//esto es para dibujar sin indices
		//glDrawArrays(GL_TRIANGLES, 0, 4);
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

