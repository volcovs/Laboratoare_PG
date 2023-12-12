//
//  main.cpp
//  OpenGL Transformations
//

#if defined (__APPLE__)
	#define GLFW_INCLUDE_GLCOREARB
#else
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <iostream>

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Shader.hpp"
#include "Camera.hpp"

int glWindowWidth = 800;
int glWindowHeight = 600;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

//vertex data --- position and color
//vertex position data has been duplicated to ensure constant color across each face
GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //0
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //1
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //2
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //3
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //4
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //5
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, //6
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //7
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //8   0'
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, //9   1'
	0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, //10   2'
	-0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //11   3'
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //12   4'
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, //13   5'
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, //14   6'
	-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, //15   7'
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //16   0''
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, //17   1''
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, //18   2''
	-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, //19   3''
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, //20   4''
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, //21   5''
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, //22   6''
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, //23   7''
};

GLuint indices[] = {
	0, 1, 2, 0, 2, 3, // bottom plane triangles
	8, 9, 5, 8, 5, 4, // back plane triangles
	17, 10, 6, 17, 6, 13, // right plane triangles
	16, 12, 7, 16, 7, 11, // left plane triangles
	20, 21, 14, 20, 14, 15, // top plane triangles
	19, 18, 22, 19, 22, 23 // front plane triangles
};

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;

gps::Shader myCustomShader;

//matrice de model pentru vertex shader
glm::mat4 model;
glm::mat4 model2;
GLint modelLoc;

//matrice de transformare de vizualizare
glm::mat4 view;
GLint viewLoc;

//matrice de transformare de proiectie
glm::mat4 projection;
GLint projectionLoc;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -10.0f);

bool wireframeView = false;

void windowResizeCallback(GLFWwindow* window, int width, int height) {

	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {

		glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {

		wireframeView = !wireframeView;

		if (wireframeView) {

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

void initObjects() {

	glGenVertexArrays(1, &objectVAO);
	glBindVertexArray(objectVAO);

	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &verticesEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, verticesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//vertex colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

bool initOpenGLWindow() {

	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	// window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	// for multisampling/antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Transformations", NULL, NULL);

	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");		
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, key_callback);
	glfwMakeContextCurrent(glWindow);	

#if not defined (__APPLE__)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

//unghiul de rotatie al obiectului
float angle = 0;
float angle2 = 0;

void renderScene() {

	model = glm::mat4(1.0f);
	model2 = glm::mat4(1.0f);

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, retina_width, retina_height);

	if (glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_PRESS) {
		angle += 0.0002f;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_V) == GLFW_PRESS) {
		angle2 += 0.002f;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS) {
		//modificare camera position si camera target
		cameraPosition.x += 0.0001;
		cameraTarget.x += 0.0001;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition.x -= 0.0001;
		cameraTarget.x -= 0.0001;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition.y += 0.0001;
		cameraTarget.y += 0.0001;
	}

	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition.y -= 0.0001;
		cameraTarget.y -= 0.0001;
	}

	view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//!!!PRIMUL CUB
	//crearea matricii de rotatie
	model = glm::rotate(model, angle, glm::vec3(0, 1, 0));

	//trimiterea matricii catre vertex shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	myCustomShader.useShaderProgram();
	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


	//!!!AL DOILEA CUB
	//sarcina 2: rotatie in jurul centrului sau
	//T inverse * R * T
	model2 = glm::translate(model2, glm::vec3(2, 0, 0));
	model2 = glm::rotate(model2, angle2, glm::vec3(0, 1, 0));
	model2 = glm::translate(model2, glm::vec3(-2, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
	
	model2 = glm::translate(model2, glm::vec3(2, 0, 0));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
	myCustomShader.useShaderProgram();
	glBindVertexArray(objectVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void cleanup() {

	if (verticesVBO) {
		glDeleteBuffers(1, &verticesVBO);
	}

	if (verticesEBO) {
		glDeleteBuffers(1, &verticesEBO);
	}

	if (objectVAO) {
		glDeleteVertexArrays(1, &objectVAO);
	}

	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char * argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initObjects();

	glEnable(GL_DEPTH_TEST);

	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();


	//crearea matricii de transformare de model initiale si transmiterea acesteia ca variabila uniform 
	//vertex shader-ului
	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//initializarea matricelor de vizualizare si proiectie
	//lookAt primeste cameraPos, cameraTarget si vectorul UP
	view = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	projection = glm::perspective(70.0f, (float)glWindowWidth / (float)glWindowHeight, 0.1f, 1000.0f);

	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//inchiderea contextului GL si a altor resurse GLFW
	glfwTerminate();

	cleanup();

	return 0;
}
