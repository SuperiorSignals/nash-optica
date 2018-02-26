/*
Author: Stewart Nash
*/
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "shader.h"
#include "position.h"
#include "sphere.h"
#include "lens.h"
#include "matrix.h"

#define PI 3.14159265359
#define SAMPLE_COUNT 1000
#define STEP 6
#define AXIS_POINTS 2
#define BACKGROUND_RED 0.2f
#define BACKGROUND_GREEN 0.3f
#define BACKGROUND_BLUE 0.3f
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

//Get rid of quickTransformVertices as soon as possible
int displayMenu(int key);
void configureLens(Lens &input);
int displayGraphics(GLfloat (&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP]);
void quickTransformVertices(GLfloat *vertices, int count);
void quickTransformVertices(GLfloat vertices[SAMPLE_COUNT * STEP]);
void generateLensVertices(Lens input, GLfloat *vertices);
void generateVertices(Sphere<GLfloat> input, GLfloat vertices[SAMPLE_COUNT * STEP], GLfloat startAngle, GLfloat endAngle);
void sphereSurface(Sphere<GLfloat> input, GLfloat angle, Position<GLfloat> &output);

int main(int argc, char *argv[])
{
	int menuSelection;
	int exitStatus;
	Lens myLens;
	//GLfloat vertices[] =
	//{
	// Positions         // Colors
	//0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
	//-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
	//0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
	//};
	GLfloat vertices[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP];
	//Sphere<GLfloat> mySphere;
	//mySphere.setRadius(0.10);
	//mySphere.center.setX(0.25);
	//mySphere.center.setY(0.5);
	//generateVertices(mySphere, vertices, 0, 2 * PI);
	//quickTransformVertices(vertices);
	
	menuSelection = 3;
	while (menuSelection != 0) {
		switch (menuSelection) {
		case 0:
			break;
		case 1:
			generateLensVertices(myLens, vertices);
			quickTransformVertices(vertices, SAMPLE_COUNT * 2 + AXIS_POINTS);
			exitStatus = displayGraphics(vertices);
			menuSelection = 3;
			break;
		case 2:
			configureLens(myLens);
			menuSelection = 3;
			break;
		case 3:
			menuSelection = displayMenu(menuSelection);
			break;
		case 4:
			matrix();
			menuSelection = 3;
			break;
		default:
			break;
		}
	}


	return 0;
}

int displayMenu(int key)
{
	int menuSelection;

	switch (key) {
	case 1:
		break;
	case 2:
		break;
	case 3:
		std::cout << "Menu" << std::endl;
		std::cout << "----------" << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "1. Display Graph" << std::endl;
		std::cout << "2. Configure Lens" << std::endl;
		std::cout << "3. Display Menu" << std::endl;
		std::cout << "4. Display Lens Matrix" << std::endl;
		std::cout << "@Nash-Optica> ";
		break;
	case 4:
		break;
	default:
		break;

	}
	std::cin >> menuSelection;

	return menuSelection;
}

void configureLens(Lens &input)
{
	GLfloat temporary;

	std::cout << "Front radius: " << input.front.getRadius() << std::endl;
	std::cout << "Front center: (" << input.front.center.getX() << ", " << input.front.center.getY() << ")" << std::endl;
	std::cout << "Back radius: " << input.back.getRadius() << std::endl;
	std::cout << "Back center: (" << input.back.center.getX() << ", " << input.back.center.getY() << ")" << std::endl;
	std::cout << "Front radius: ";
	std::cin >> temporary;
	input.front.setRadius(temporary);
	std::cout << "Front center-x: ";
	std::cin >> temporary;
	input.front.center.setX(temporary);
	std::cout << "Front center-y: ";
	std::cin >> temporary;
	input.front.center.setY(temporary);
	std::cout << "Back radius: ";
	std::cin >> temporary;
	input.back.setRadius(temporary);
	std::cout << "Back center-x: ";
	std::cin >> temporary;
	input.back.center.setX(temporary);
	std::cout << "Back center-y: ";
	std::cin >> temporary;
	input.back.center.setY(temporary);
}

int displayGraphics(GLfloat (&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP])
{
	GLFWwindow *window;
	int screenWidth, screenHeight;
	GLuint VBO, VAO;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Optics Sample 1", nullptr, nullptr);
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	Shader ourShader("core.vert", "core.frag");
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, SAMPLE_COUNT * 2 + AXIS_POINTS);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return EXIT_SUCCESS;
}

void quickTransformVertices(GLfloat *vertices, int count)
{
	int i;
	GLfloat horizontalScale;
	//GLfloat verticalScale;

	horizontalScale = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);
	for (i = 0; i < count; i++) {
		vertices[i * STEP] = 10 * (vertices[i * STEP] - 1.0) * horizontalScale + 4.5;
		vertices[i * STEP + 1] = 10 * (vertices[i * STEP + 1] - 0.5);
		//vertices[i * STEP + 2] = vertices[i * STEP + 2];
		//vertices[i * STEP + 3] = 1.0f;
		//vertices[i * STEP + 4] = 1.0f;
		//vertices[i * STEP + 5] = 1.0f;
	}
}

void quickTransformVertices(GLfloat vertices[SAMPLE_COUNT * STEP])
{
	int i;
	GLfloat horizontalScale;
	//GLfloat verticalScale;

	horizontalScale = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);
	for (i = 0; i < SAMPLE_COUNT + AXIS_POINTS; i++) {
		vertices[i * STEP] = (vertices[i * STEP] - 1.0) * horizontalScale;
		vertices[i * STEP + 1] = vertices[i * STEP + 1] - 0.5;
		vertices[i * STEP + 2] = vertices[i * STEP + 2];
		//vertices[i * STEP + 3] = 1.0f;
		//vertices[i * STEP + 4] = 1.0f;
		//vertices[i * STEP + 5] = 1.0f;
	}	
}

void generateLensVertices(Lens input, GLfloat *vertices)
{
	int i;
	GLfloat startAngle, stopAngle;
	GLfloat angle, interval;
	Position<GLfloat> vertex;
	GLfloat angles[4];

	input.generateIntersectionAngle(angles);
	startAngle = angles[0];
	stopAngle = angles[1];
	std::cout << startAngle << std::endl;
	std::cout << stopAngle << std::endl;
	interval = (stopAngle - startAngle) / SAMPLE_COUNT;
	angle = startAngle;
	for (i = 0; i < SAMPLE_COUNT; i++) {
		sphereSurface(input.front, angle, vertex);
		vertices[i * STEP] = vertex.getX();
		vertices[i * STEP + 1] = vertex.getY();
		vertices[i * STEP + 2] = 0.0f;
		vertices[i * STEP + 3] = 1.0f;
		vertices[i * STEP + 4] = 1.0f;
		vertices[i * STEP + 5] = 1.0f;
		angle += interval;
	}
	vertices[(i - 1) * STEP + 3] = BACKGROUND_RED;
	vertices[(i - 1) * STEP + 4] = BACKGROUND_GREEN;
	vertices[(i - 1) * STEP + 5] = BACKGROUND_BLUE;
	startAngle = angles[2];
	stopAngle = angles[3];
	std::cout << startAngle << std::endl;
	std::cout << stopAngle << std::endl;
	interval = (stopAngle - startAngle) / SAMPLE_COUNT;
	angle = startAngle;
	for (i = 0; i < SAMPLE_COUNT; i++) {
		sphereSurface(input.back, angle, vertex);
		vertices[SAMPLE_COUNT * STEP + i * STEP] = vertex.getX();
		vertices[SAMPLE_COUNT * STEP + i * STEP + 1] = vertex.getY();
		vertices[SAMPLE_COUNT * STEP + i * STEP + 2] = 0.0f;
		vertices[SAMPLE_COUNT * STEP + i * STEP + 3] = 1.0f;
		vertices[SAMPLE_COUNT * STEP + i * STEP + 4] = 1.0f;
		vertices[SAMPLE_COUNT * STEP + i * STEP + 5] = 1.0f;
		angle += interval;
	}
	vertices[SAMPLE_COUNT * STEP + (i - 1) * STEP + 3] = BACKGROUND_RED;
	vertices[SAMPLE_COUNT * STEP + (i - 1) * STEP + 4] = BACKGROUND_GREEN;
	vertices[SAMPLE_COUNT * STEP + (i - 1) * STEP + 5] = BACKGROUND_BLUE;
	vertices[SAMPLE_COUNT * STEP + 3] = BACKGROUND_RED;
	vertices[SAMPLE_COUNT * STEP + 4] = BACKGROUND_GREEN;
	vertices[SAMPLE_COUNT * STEP + 5] = BACKGROUND_BLUE;
	// Draw axis
	vertices[SAMPLE_COUNT * STEP + i * STEP] = 0.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 1] = 0.5f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 2] = 0.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 3] = 1.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 4] = 0.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 5] = 0.0f;
	++i;
	vertices[SAMPLE_COUNT * STEP + i * STEP] = 1.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 1] = 0.5f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 2] = 0.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 3] = 1.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 4] = 0.0f;
	vertices[SAMPLE_COUNT * STEP + i * STEP + 5] = 0.0f;
}

void generateVertices(Sphere<GLfloat> input, GLfloat vertices[SAMPLE_COUNT * STEP], GLfloat startAngle, GLfloat endAngle)
{
	int i;
	GLfloat angle;
	GLfloat interval;
	Position<GLfloat> vertex;

	interval = (endAngle - startAngle) / SAMPLE_COUNT;
	angle = startAngle;
	for (i = 0; i < SAMPLE_COUNT; i++) {		
		sphereSurface(input, angle, vertex);
		vertices[i * STEP] = vertex.getX();
		vertices[i * STEP + 1] = vertex.getY();
		vertices[i * STEP + 2] = 0.0f;
		vertices[i * STEP + 3] = 1.0f;
		vertices[i * STEP + 4] = 1.0f;
		vertices[i * STEP + 5] = 1.0f;
		angle += interval;
	}
}

void sphereSurface(Sphere<GLfloat> input, GLfloat angle, Position<GLfloat> &output)
{
	GLfloat temporary;

	temporary = input.getRadius();
	temporary = temporary * cos(angle);
	temporary = temporary + input.center.getX();
	output.setX(temporary);
	temporary = input.getRadius();
	temporary = temporary * sin(angle);
	temporary = temporary + input.center.getY();
	output.setY(temporary);
}