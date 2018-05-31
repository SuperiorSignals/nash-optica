/*
Author: Stewart Nash
*/
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "customlens.h"
#include "lens.h"
#include "matrix.h"
#include "position.h"
#include "shader.h"
#include "sphere.h"
#include "sphericallens.h"
#include "test_functions.h"

#define PI 3.14159265359
#define SAMPLE_COUNT 10240
#define STEP 6
#define AXIS_POINTS 2
#define BACKGROUND_RED 0.2f
#define BACKGROUND_GREEN 0.3f
#define BACKGROUND_BLUE 0.3f
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

//Get rid of quickTransformVertices as soon as possible
void configureLens(SphericalLens &input);
int displayCustomMenu(CustomLens &myCustomLens, GLfloat (&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP]);
int displayGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber);
int displayRotatingGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber);
int displayStationaryGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber);
int displayMenu(int key);
void generateCustomLensVertices(CustomLens input, GLfloat *vertices);
void generateCustomLensVerticesProfile(CustomLens input, GLfloat *vertices);
void generateCustomLensVerticesWire(CustomLens input, GLfloat *vertices);
void generateLensMesh(SphericalLens input, GLfloat *vertices);
void generateLensVertices(SphericalLens input, GLfloat *vertices);
void generateVertices(Sphere<GLfloat> input, GLfloat vertices[SAMPLE_COUNT * STEP], GLfloat startAngle, GLfloat endAngle);
void makeCustomIntensityProfile(CustomLens input, std::vector<double> lensAngle, std::vector<double> intensity);
void quickTransformVertices(GLfloat *vertices, int count);
void quickTransformVertices(GLfloat vertices[SAMPLE_COUNT * STEP]);
void quickTransformCustomVertices(GLfloat vertices[SAMPLE_COUNT * STEP]);
void sphereSurface(Sphere<GLfloat> input, GLfloat angle, Position<GLfloat> &output);

int main(int argc, char *argv[])
{
	int menuSelection;
	int exitStatus;
	SphericalLens myLens;
	CustomLens myCustomLens;
	//GLfloat vertices[] =
	//{
	// Positions         // Colors
	//0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
	//-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
	//0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
	//};
	GLfloat vertices[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP];

	test_function_1();
	menuSelection = 1;
	while (menuSelection != 0) {
		switch (menuSelection) {
		case 0:
			break;
		case 1: //Display menu
			menuSelection = displayMenu(menuSelection);
			break;
		case 2: //Display spherical lens
			configureLens(myLens);
			generateLensVertices(myLens, vertices);
			quickTransformVertices(vertices, SAMPLE_COUNT * 2 + AXIS_POINTS);
			//exitStatus = displayGraphics(vertices, static_cast<GLsizei>(SAMPLE_COUNT * 2 + AXIS_POINTS));
			exitStatus = displayRotatingGraphics(vertices, static_cast<GLsizei>(SAMPLE_COUNT * 2 + AXIS_POINTS));
			menuSelection = 1;
			break;
		case 3: //Display spherical lens intensity
			std::cout << "This function has not yet been implemented." << std::endl;
			menuSelection = 1;
			break;
		case 4: //Display custom lens
			exitStatus = displayCustomMenu(myCustomLens, vertices);
			generateCustomLensVertices(myCustomLens, vertices);
			quickTransformCustomVertices(vertices);
			//quickTransformVertices(vertices);
			//exitStatus = displayRotatingGraphics(vertices, static_cast<GLsizei>(SAMPLE_COUNT));
			exitStatus = displayRotatingGraphics(vertices, static_cast<GLsizei>(SAMPLE_COUNT));
			menuSelection = 1;
			break;
		case 5: //Display custom lens intensity
			std::cout << "This function has not yet been implemented." << std::endl;
			menuSelection = 1;
			break;
		default:
			break;
		}
	}

	return 0;
}

void configureLens(SphericalLens &input)
{
	GLfloat temporary;

	std::cout << "Front radius: ";
	std::cin >> temporary;
	input.changeFrontRadius(temporary);
	std::cout << "Back radius: ";
	std::cin >> temporary;
	input.changeBackRadius(temporary);
	std::cout << "Thickness: ";
	std::cin >> temporary;
	input.setThickness(temporary);
}

int displayCustomMenu(CustomLens &myCustomLens, GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP])
{
	int exitStatus;
	float focalLength;
	float thickness;
	double deviation;
	double standard;

	exitStatus = 0;
	while (exitStatus != 1 && exitStatus != 2 && exitStatus != 3) {
		std::cout << "Beam Profile" << std::endl;
		std::cout << "------------" << std::endl;
		std::cout << "1. Gaussian" << std::endl;
		std::cout << "2. Maxwell" << std::endl;
		std::cout << "3. Custom" << std::endl;
		std::cout << "@Nash-Optica>Beam Profile: ";
		std::cin >> exitStatus;
	}
	std::cout << "Original Gaussian STDDEV: ";
	std::cin >> standard;
	std::cout << "Beam STDDEV: ";
	std::cin >> deviation;
	switch (exitStatus) {
	case 1:
		myCustomLens = matrix(GAUSSIAN, standard, deviation);
		break;
	case 2:
		myCustomLens = matrix(MAXWELL, standard, deviation);
		break;
	case 3:
		myCustomLens = matrix(CUSTOM, standard, deviation);
		break;
	}
	std::cout << "Enter focal length: ";
	std::cin >> focalLength;
	std::cout << "Enter thickness: ";
	std::cin >> thickness;
	myCustomLens.setFocalLength(static_cast<GLfloat>(focalLength));
	myCustomLens.setBaseThickness(static_cast<GLfloat>(thickness));

	return exitStatus;
}

int displayGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber)
{
	GLFWwindow *window;
	int screenWidth, screenHeight;
	GLuint VBO, VAO;

	//glm::mat4 model;
	//glm::mat4 view;
	//glm::mat4 projection;
	glm::mat4 transformation;
	//unsigned int modelLocation;
	//unsigned int viewLocation;
	GLint transformationLocation;

	//glfwGetTime()

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

	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	//projection = glm::perspective(glm::radians(15.0f), (float)screenWidth / (float)screenHeight, 0.1f, 25.0f);
	//modelLocation = glGetUniformLocation(ourShader.Program, "model");
	//viewLocation = glGetUniformLocation(ourShader.Program, "view");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	//glUniformMatrix3fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, &projection[0][0]);
	
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

		transformation = glm::rotate(transformation, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationLocation = glGetUniformLocation(ourShader.Program, "transformation");
		glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, glm::value_ptr(transformation));

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, vertexNumber);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return EXIT_SUCCESS;
}

int displayRotatingGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber)
{
	GLFWwindow *window;
	int screenWidth, screenHeight;
	GLuint VBO, VAO;
	//glm::mat4 model;
	//glm::mat4 view;
	//glm::mat4 projection;
	glm::mat4 transformation;
	//unsigned int modelLocation;
	//unsigned int viewLocation;
	GLint transformationLocation;
	double startTime;
	double endTime;
	const double ROTATION_SPEED = 0.005;

	//glfwGetTime()

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
	startTime = glfwGetTime();
	//model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
	//projection = glm::perspective(glm::radians(15.0f), (float)screenWidth / (float)screenHeight, 0.1f, 25.0f);
	//modelLocation = glGetUniformLocation(ourShader.Program, "model");
	//viewLocation = glGetUniformLocation(ourShader.Program, "view");
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	//glUniformMatrix3fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, &projection[0][0]);

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

		endTime = glfwGetTime();
		if (endTime - startTime > ROTATION_SPEED) {
			transformation = glm::rotate(transformation, glm::radians(0.05f), glm::vec3(0.0f, 1.0f, 0.0f));
			startTime = glfwGetTime();
		}
		transformationLocation = glGetUniformLocation(ourShader.Program, "transformation");
		glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, glm::value_ptr(transformation));

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, vertexNumber);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return EXIT_SUCCESS;
}

int displayStationaryGraphics(GLfloat(&vertices)[SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP], GLsizei vertexNumber)
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
		glDrawArrays(GL_LINE_STRIP, 0, vertexNumber);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return EXIT_SUCCESS;
}

int displayMenu(int key)
{
	int menuSelection;

	switch (key) {
	case 1:
		std::cout << "Menu" << std::endl;
		std::cout << "----------" << std::endl;
		std::cout << "0. Exit" << std::endl;
		std::cout << "1. Display Menu" << std::endl;
		std::cout << "2. Display Spherical Lens" << std::endl;
		std::cout << "3. Display Spherical Lens Intensity" << std::endl;
		std::cout << "4. Display Custom Lens" << std::endl;
		std::cout << "5. Display Custom Lens Intensity" << std::endl;
		std::cout << "@Nash-Optica> ";
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;

	}
	std::cin >> menuSelection;

	return menuSelection;
}

void generateCustomLensVertices(CustomLens input, GLfloat *vertices)
{
	int i, j;
	int profileCount;
	int profileNumber;
	bool isTruncated;
	GLfloat deltaHeight;
	GLfloat positionX, positionY;
	double startX, startY;
	double endX, endY;
	double deltaX, deltaY;
	double angle, lensAngle;
	double heightLimit;
	double widthLimit;
	double profileAngle; // Angle to rotate profile;
	std::vector<double> angles, lensAngles;
	std::ofstream outputFile;

	angles = input.getAngles();
	lensAngles = input.getLensAngles();

	// Focal point is at origin (0, 0)
	positionX = input.getFocalLength();
	positionY = 0;

	widthLimit = input.getBaseThickness() + input.getFocalLength();
	profileCount = 0;
	profileAngle = 0;
	profileNumber = SAMPLE_COUNT / angles.size();
	std::cout << "Profile Number: " << profileNumber << std::endl;
	for (i = 0; i < profileNumber; i++) {
		isTruncated = false;
		startX = static_cast<double>(positionX);
		startY = static_cast<double>(positionY);
		for (j = 0; j < angles.size(); j++) {
			if (1 + (tan(angles[j]) * tan(lensAngles[j])) == 0.0) {
				if ((startX * tan(angles[j]) - startY) == 0.0) {
					deltaY = 0.0;
				} else {
					std::cout << "ERROR! DIVISION BY ZERO.\n";
				}
			} else {
				deltaY = (startX * tan(angles[j]) - startY) / (1 + tan(angles[j]) * tan(lensAngles[j]));
			}
			deltaX = deltaY * tan(lensAngles[j]);
			endX = startX - deltaX;
			endY = startY + deltaY;
			if (endX < widthLimit && isTruncated == false) {
				vertices[(i * angles.size() + j) * STEP] = endX;
				vertices[(i * angles.size() + j) * STEP + 1] = endY * cos(profileAngle);
				vertices[(i * angles.size() + j) * STEP + 2] = endY * sin(profileAngle);
				vertices[(i * angles.size() + j) * STEP + 3] = 1.0f;
				vertices[(i * angles.size() + j) * STEP + 4] = 1.0f;
				vertices[(i * angles.size() + j) * STEP + 5] = 1.0f;
			} else {
				isTruncated = true;
				vertices[(i * angles.size() + j) * STEP] = vertices[(i * angles.size() + j - 1) * STEP];
				vertices[(i * angles.size() + j) * STEP + 1] = vertices[(i * angles.size() + j - 1) * STEP + 1];
				vertices[(i * angles.size() + j) * STEP + 2] = vertices[(i * angles.size() + j - 1) * STEP + 2];
				vertices[(i * angles.size() + j) * STEP + 3] = 1.0f;
				vertices[(i * angles.size() + j) * STEP + 4] = 1.0f;
				vertices[(i * angles.size() + j) * STEP + 5] = 1.0f;
			}
			startX = endX;
			startY = endY;
		}
		vertices[(i * angles.size() + angles.size() - 1) * STEP + 1] = 0;
		vertices[(i * angles.size() + angles.size() - 1) * STEP + 2] = 0;
		profileAngle += 2 * PI / profileNumber;
	}

	outputFile.open("optics_output_2.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(5);
		for (i = 0; i < SAMPLE_COUNT; i++) {
			outputFile << vertices[i * STEP] << "\t\t" << vertices[i * STEP + 1] << std::endl;
		}
	} else {
		std::cout << "ERROR: Unable to open file optics_output_2.txt" << std::endl;
	}
	outputFile.close();
}

void generateCustomLensVerticesProfile(CustomLens input, GLfloat *vertices)
{
	int i;
	bool isTruncated = false;
	GLfloat deltaHeight;
	GLfloat positionX, positionY;
	double startX, startY;
	double endX, endY;
	double deltaX, deltaY;
	double angle, lensAngle;
	double heightLimit = 0.05;
	double widthLimit;
	std::vector<double> angles, lensAngles;
	std::ofstream outputFile;
	
	angles = input.getAngles();
	lensAngles = input.getLensAngles();

	// Focal point is at origin (0, 0)
	positionX = input.getFocalLength();
	positionY = 0;

	startX = static_cast<double>(positionX);
	startY = static_cast<double>(positionY);

	widthLimit = static_cast<double>(input.getFocalLength() + input.getBaseThickness());
	for (i = 0; i < angles.size(); i++) {
		//std::cout << "Position: " << startX << " " << startY << " ";
		if (1 + (tan(angles[i]) * tan(lensAngles[i])) == 0.0) {
			if ((startX * tan(angles[i]) - startY) == 0.0) {
				deltaY = 0.0;
			} else {
				std::cout << "ERROR! DIVISION BY ZERO.\n";
			}
		} else {
			deltaY = (startX * tan(angles[i]) - startY) / (1 + tan(angles[i]) * tan(lensAngles[i]));
		}		
		deltaX = deltaY * tan(lensAngles[i]);
		endX = startX - deltaX;
		endY = startY + deltaY;
		if (i < (SAMPLE_COUNT - 2)) {
			if (i > 0) {
				if (endY < heightLimit && endX < widthLimit && !isTruncated) {
					if (endY > vertices[(i - 1) * STEP + 1]) {
						vertices[i * STEP] = endX;
						vertices[i * STEP + 1] = endY;
						vertices[i * STEP + 2] = 0.0f;
						vertices[i * STEP + 3] = 1.0f;
						vertices[i * STEP + 4] = 1.0f;
						vertices[i * STEP + 5] = 1.0f;
					} else {
						std::cout << i << " ";
						vertices[i * STEP] = vertices[(i - 1) * STEP];
						vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
						vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
						vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
						vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
						vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
					}
				} else {
					isTruncated = true;
					vertices[i * STEP] = vertices[(i - 1) * STEP];
					vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
					vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
					vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
					vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
					vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
				}
			} else {
				vertices[i * STEP] = endX;
				vertices[i * STEP + 1] = endY;
				vertices[i * STEP + 2] = 0.0f;
				vertices[i * STEP + 3] = 1.0f;
				vertices[i * STEP + 4] = 1.0f;
				vertices[i * STEP + 5] = 1.0f;
			}
			
		}
		/*
		if (!(i % DISPLAY_NUMBER)) {
			std::cout << "Actual: " << endX << " " << endY << " ";
			std::cout << "Display: " << vertices[i * STEP] << " " << vertices[i * STEP + 1] << std::endl;
		}
		*/
		startX = endX;
		startY = endY;
	}
	while (i < (SAMPLE_COUNT - 2)) {
		vertices[i * STEP] = vertices[(i - 1) * STEP];
		vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
		vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
		vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
		vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
		vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
		i++;
	}

	// Check that there are more than three samples?
	vertices[(SAMPLE_COUNT - 2) * STEP] = input.getFocalLength() + input.getBaseThickness();
	vertices[(SAMPLE_COUNT - 2) * STEP + 1] = vertices[(SAMPLE_COUNT - 3) * STEP + 1];
	vertices[(SAMPLE_COUNT - 2) * STEP + 2] = vertices[(SAMPLE_COUNT - 3) * STEP + 2];
	vertices[(SAMPLE_COUNT - 2) * STEP + 3] = vertices[(SAMPLE_COUNT - 3) * STEP + 3];
	vertices[(SAMPLE_COUNT - 2) * STEP + 4] = vertices[(SAMPLE_COUNT - 3) * STEP + 4];
	vertices[(SAMPLE_COUNT - 2) * STEP + 5] = vertices[(SAMPLE_COUNT - 3) * STEP + 5];

	vertices[(SAMPLE_COUNT - 1) * STEP] = vertices[(SAMPLE_COUNT - 2) * STEP];
	vertices[(SAMPLE_COUNT - 1) * STEP + 1] = 0.0;
	vertices[(SAMPLE_COUNT - 1) * STEP + 2] = vertices[(SAMPLE_COUNT - 2) * STEP + 2];
	vertices[(SAMPLE_COUNT - 1) * STEP + 3] = vertices[(SAMPLE_COUNT - 2) * STEP + 3];
	vertices[(SAMPLE_COUNT - 1) * STEP + 4] = vertices[(SAMPLE_COUNT - 2) * STEP + 4];
	vertices[(SAMPLE_COUNT - 1) * STEP + 5] = vertices[(SAMPLE_COUNT - 2) * STEP + 5];

	outputFile.open("optics_output_2.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(5);
		for (i = 0; i < SAMPLE_COUNT; i++) {
			outputFile << vertices[i * STEP] << "\t\t" << vertices[i * STEP + 1] << std::endl;
		}
	} else {
		std::cout << "ERROR: Unable to open file optics_output_2.txt" << std::endl;
	}
	outputFile.close();
}

void generateCustomLensVerticesWire(CustomLens input, GLfloat *vertices)
{
	int i, j;
	int profileCount;
	bool isTruncated;
	GLfloat deltaHeight;
	GLfloat positionX, positionY;
	double startX, startY;
	double endX, endY;
	double deltaX, deltaY;
	double angle, lensAngle;
	double heightLimit = 0.05;
	double widthLimit;
	double profileAngle; // Angle to rotate profile;
	std::vector<double> angles, lensAngles;
	std::ofstream outputFile;

	angles = input.getAngles();
	lensAngles = input.getLensAngles();

	// Focal point is at origin (0, 0)
	positionX = input.getFocalLength();
	positionY = 0;

	profileCount = 0;
	profileAngle = 0;
	while (profileCount < SAMPLE_COUNT && profileAngle < 2 * PI) {
		startX = static_cast<double>(positionX);
		startY = static_cast<double>(positionY);
		isTruncated = false;
		widthLimit = static_cast<double>(input.getFocalLength() + input.getBaseThickness());
		for (i = profileCount, j = 0; j < angles.size(); i++, j++) {
			if (1 + (tan(angles[j]) * tan(lensAngles[j])) == 0.0) {
				if ((startX * tan(angles[j]) - startY) == 0.0) {
					deltaY = 0.0;
				}
				else {
					std::cout << "ERROR! DIVISION BY ZERO.\n";
				}
			}
			else {
				deltaY = (startX * tan(angles[j]) - startY) / (1 + tan(angles[j]) * tan(lensAngles[j]));
			}
			deltaX = deltaY * tan(lensAngles[j]);
			endX = startX - deltaX;
			endY = startY + deltaY;
			if (i < SAMPLE_COUNT) {
				if (i > 0) {
					if (endY < heightLimit && endX < widthLimit && !isTruncated) {
						if (endY > vertices[(i - 1) * STEP + 1]) {
							vertices[i * STEP] = endX;
							vertices[i * STEP + 1] = endY * cos(profileAngle);
							vertices[i * STEP + 2] = endY * sin(profileAngle);
							vertices[i * STEP + 3] = 1.0f;
							vertices[i * STEP + 4] = 1.0f;
							vertices[i * STEP + 5] = 1.0f;
						}
						else {
							vertices[i * STEP] = vertices[(i - 1) * STEP];
							vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
							vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
							vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
							vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
							vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
						}
					}
					else {
						if (isTruncated == false) {
							profileCount = i;
						}
						isTruncated = true;
						vertices[i * STEP] = vertices[(i - 1) * STEP];
						vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
						vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
						vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
						vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
						vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
					}
				}
				else {
					vertices[i * STEP] = endX;
					vertices[i * STEP + 1] = endY;
					vertices[i * STEP + 2] = 0.0f;
					vertices[i * STEP + 3] = 1.0f;
					vertices[i * STEP + 4] = 1.0f;
					vertices[i * STEP + 5] = 1.0f;
				}
			}
			//if (!(i % DISPLAY_NUMBER)) {
			//	std::cout << "Actual: " << endX << " " << endY << " ";
			//	std::cout << "Display: " << vertices[i * STEP] << " " << vertices[i * STEP + 1] << std::endl;
			//}
			startX = endX;
			startY = endY;
		}
		profileAngle += 2 * PI / 30;
		if (isTruncated == false) {
			profileCount = i;
		}
		i = profileCount;
		if (i < SAMPLE_COUNT - 1) {
			vertices[i * STEP] = input.getFocalLength() + input.getBaseThickness();
			vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
			vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
			vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
			vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
			vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
			i++;
			vertices[i * STEP] = vertices[(i - 1) * STEP];
			vertices[i * STEP + 1] = 0.0;
			vertices[i * STEP + 2] = 0.0;
			vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
			vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
			vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
			i++;
		}
		//profileCount = i;
		//std::cout << "Profile count: " << profileCount << std::endl;
	}


	while (i < SAMPLE_COUNT) {
		vertices[i * STEP] = vertices[(i - 1) * STEP];
		vertices[i * STEP + 1] = vertices[(i - 1) * STEP + 1];
		vertices[i * STEP + 2] = vertices[(i - 1) * STEP + 2];
		vertices[i * STEP + 3] = vertices[(i - 1) * STEP + 3];
		vertices[i * STEP + 4] = vertices[(i - 1) * STEP + 4];
		vertices[i * STEP + 5] = vertices[(i - 1) * STEP + 5];
		i++;
	}

	// Check that there are more than three samples?

	outputFile.open("optics_output_2.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(5);
		for (i = 0; i < SAMPLE_COUNT; i++) {
			outputFile << vertices[i * STEP] << "\t\t" << vertices[i * STEP + 1];
			outputFile << "\t\t" << vertices[i * STEP + 2] << std::endl;
		}
	} else {
		std::cout << "ERROR: Unable to open file optics_output_2.txt" << std::endl;
	}
	outputFile.close();
}

void generateLensMesh(SphericalLens input, GLfloat *vertices)
{

}

void generateLensVertices(SphericalLens input, GLfloat *vertices)
{
	int i, j;
	GLfloat startAngle, stopAngle;
	GLfloat angle, interval;
	Position<GLfloat> vertex;
	GLfloat angles[4];
	int sampleCount;
	const int DIVISION = 16;
	float rotation;
	std::vector<Position<GLfloat>> output;

	for (i = 0; i < SAMPLE_COUNT * STEP * 2 + AXIS_POINTS * STEP; i++)
	{
		vertices[i] = 0;
	}

	sampleCount = SAMPLE_COUNT / DIVISION;
	output = lensProfile(input, sampleCount);
	rotation = 0;
	//for (i = 0; i < output.size(); i++) {
	//	std::cout << output[i].getX() << " ";
	//	std::cout << output[i].getY() << std::endl;
	//}
	for (j = 0; j < DIVISION; j++) {
		//rotation = static_cast<float>(j * 2) * PI / static_cast<float>(DIVISION);
		rotation += 2.0 * PI / static_cast<float>(DIVISION);
		for (i = 0; i < sampleCount; i++) {
			//sphereSurface(input.front, angle, vertex);
			vertices[j * sampleCount * STEP + i * STEP] = (output[i]).getX();
			vertices[j * sampleCount * STEP + i * STEP + 1] = (output[i]).getY() * cos(rotation);
			vertices[j * sampleCount * STEP + i * STEP + 2] = (output[i]).getY() * sin(rotation);
			vertices[j * sampleCount * STEP + i * STEP + 3] = 1.0f;
			vertices[j * sampleCount * STEP + i * STEP + 4] = 1.0f;
			vertices[j * sampleCount * STEP + i * STEP + 5] = 1.0f;
		}
		/*
		vertices[j * sampleCount * STEP + (i - 1) * STEP + 3] = BACKGROUND_RED;
		vertices[j * sampleCount * STEP + (i - 1) * STEP + 4] = BACKGROUND_GREEN;
		vertices[j * sampleCount * STEP + (i - 1) * STEP + 5] = BACKGROUND_BLUE;
		startAngle = angles[2];
		stopAngle = angles[3];
		if (!(i % DISPLAY_NUMBER)) {
			std::cout << startAngle << std::endl;
			std::cout << stopAngle << std::endl;
		}
		interval = (stopAngle - startAngle) / sampleCount;
		angle = startAngle;
		for (i = 0; i < sampleCount; i++) {
			sphereSurface(input.back, angle, vertex);
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP] = vertex.getX();
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 1] = vertex.getY() * cos(rotation);
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 2] = vertex.getY() * sin(rotation);
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 3] = 1.0f;
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 4] = 1.0f;
			vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 5] = 1.0f;
			angle += interval;
		}
		vertices[j * sampleCount * STEP + sampleCount * STEP + (i - 1) * STEP + 3] = BACKGROUND_RED;
		vertices[j * sampleCount * STEP + sampleCount * STEP + (i - 1) * STEP + 4] = BACKGROUND_GREEN;
		vertices[j * sampleCount * STEP + sampleCount * STEP + (i - 1) * STEP + 5] = BACKGROUND_BLUE;
		vertices[j * sampleCount * STEP + sampleCount * STEP + 3] = BACKGROUND_RED;
		vertices[j * sampleCount * STEP + sampleCount * STEP + 4] = BACKGROUND_GREEN;
		vertices[j * sampleCount * STEP + sampleCount * STEP + 5] = BACKGROUND_BLUE;
		// Draw axis
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP] = 0.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 1] = 0.5f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 2] = 0.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 3] = 1.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 4] = 0.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 5] = 0.0f;
		++i;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP] = 1.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 1] = 0.5f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 2] = 0.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 3] = 1.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 4] = 0.0f;
		vertices[j * sampleCount * STEP + sampleCount * STEP + i * STEP + 5] = 0.0f;
		*/
	}
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

void makeCustomIntensityProfile(CustomLens input, std::vector<double> lensAngle, std::vector<double> intensity)
{

}

void quickTransformVertices(GLfloat *vertices, int count)
{
	int i;
	GLfloat aspectRatio;
	GLfloat horizontalScale;
	GLfloat horizontalShift;
	GLfloat verticalShift;
	GLfloat verticalScale;

	GLfloat horizontalMaximum;
	int hozMax = 0;
	GLfloat verticalMaximum;
	int verMax = 0;

	aspectRatio = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);
	//horizontalShift = 4.5;
	horizontalShift = 0.0;
	//verticalShift = -5.0;
	verticalShift = 0.0;
	horizontalScale = 1.0 * aspectRatio;
	verticalScale = 1.0;
	// Actually, the horizontal and vertical scale should reflect
	// the range of values while a shift should put the center
	// of the values in the middle of the screen
	if (count > 0) {
		horizontalMaximum = abs(vertices[0]);
		verticalMaximum = abs(vertices[1]);
	}
	for (i = 0; i < count; i++) {
		if (horizontalMaximum < abs(vertices[i * STEP])) {
			horizontalMaximum = abs(vertices[i * STEP]);
			hozMax = i;
		}
		if (verticalMaximum < abs(vertices[i * STEP + 1])) {
			verticalMaximum = abs(vertices[i* STEP + 1]);
			verMax = i;
		}
	}
	if (verticalMaximum > 0) {
		verticalScale = 1 / verticalMaximum;
	}
	if (horizontalMaximum > 0) {
		horizontalScale = 1 / horizontalMaximum;
	}
	/*
	if (horizontalScale < verticalScale) {
		horizontalScale = verticalScale;
	} else {
		verticalScale = horizontalScale;
	}
	*/
	verticalScale = verticalScale * 0.75;
	horizontalScale = horizontalScale * 0.75;
	std::cout << "Horizontal maximum: " << horizontalMaximum;
	std::cout << " at i = " << hozMax << std::endl;
	std::cout << "Vertical maximum: " << verticalMaximum;
	std::cout << " at i = " << verMax << std::endl;
	std::cout << "Pre-processed Horizontal scale: " << horizontalScale << std::endl;
	std::cout << "Pre-processed Vertical scale: " << verticalScale << std::endl;
	if (horizontalScale < verticalScale) {
		verticalScale = horizontalScale;
	} else {
		horizontalScale = verticalScale;
	}
	std::cout << "Post-processed Horizontal scale: " << horizontalScale << std::endl;
	std::cout << "Post-processed Vertical scale: " << verticalScale << std::endl;
	horizontalScale = horizontalScale * aspectRatio;
	for (i = 0; i < count; i++) {
		vertices[i * STEP] = horizontalScale * (vertices[i * STEP]) + horizontalShift;
		//vertices[i * STEP] = 10 * (vertices[i * STEP] - 1.0) + 4.5;
		vertices[i * STEP + 1] = verticalScale * (vertices[i * STEP + 1]) + verticalShift;
		vertices[i * STEP + 2] = aspectRatio * verticalScale * (vertices[i * STEP + 2]);
		//vertices[i * STEP + 2] = vertices[i * STEP + 2];
		//vertices[i * STEP + 3] = 1.0f;
		//vertices[i * STEP + 4] = 1.0f;
		//vertices[i * STEP + 5] = 1.0f;
	}
}

void quickTransformVertices(GLfloat vertices[SAMPLE_COUNT * STEP])
{
	quickTransformVertices(vertices, SAMPLE_COUNT * 2);
	/*
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
	*/
}

void quickTransformCustomVertices(GLfloat vertices[SAMPLE_COUNT * STEP])
{
	int i;
	GLfloat horizontalScale;
	GLfloat horizontalShift;
	GLfloat verticalScale;
	GLfloat verticalShift;
	GLfloat aspectRatio;
	GLfloat horizontalMaximum;
	GLfloat verticalMaximum;
	int hozMax = 0;
	int verMax = 0;

	//horizontalScale = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);
	aspectRatio = static_cast<GLfloat>(HEIGHT) / static_cast<GLfloat>(WIDTH);

	horizontalShift = 0;
	verticalShift = 0;

	horizontalMaximum = abs(vertices[0]);
	verticalMaximum = abs(vertices[1]);
	for (i = 0; i < SAMPLE_COUNT; i++) {
		if (horizontalMaximum < abs(vertices[i * STEP])) {
			horizontalMaximum = abs(vertices[i * STEP]);
			hozMax = i;
		}
		if (verticalMaximum < abs(vertices[i * STEP + 1])) {
			verticalMaximum = abs(vertices[i* STEP + 1]);
			verMax = i;
		}
	}
	if (verticalMaximum > 0) {
		verticalScale = 1 / verticalMaximum;
	}
	if (horizontalMaximum > 0) {
		horizontalScale = 1 / horizontalMaximum;
	}
	
	//horizontalScale = 10.0 * aspectRatio;
	//horizontalShift = 0.01;
	//verticalScale = 10;
	//verticalShift = 0.0;

	if (horizontalScale > verticalScale) {
		horizontalScale = verticalScale;
	} else {
		verticalScale = horizontalScale;
	}
	horizontalScale *= aspectRatio;

	std::cout << "Vertical scale: " << verticalScale << std::endl;
	std::cout << "Horizontal scale: " << horizontalScale << std::endl;
	
	for (i = 0; i < SAMPLE_COUNT + AXIS_POINTS; i++) {
		vertices[i * STEP] = (vertices[i * STEP] - horizontalShift) * horizontalScale;
		vertices[i * STEP + 1] = (vertices[i * STEP + 1] - verticalShift) * verticalScale;
		vertices[i * STEP + 2] = vertices[i * STEP + 2] * verticalScale * aspectRatio;
		//vertices[i * STEP + 3] = 1.0f;
		//vertices[i * STEP + 4] = 1.0f;
		//vertices[i * STEP + 5] = 1.0f;
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