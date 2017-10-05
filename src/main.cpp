#include <glad/glad.h>
//#define GLFW_INCLUDE_GLCOREARB
//#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Shader.h"
#include "HilbertCurveGen.h"

using namespace std;

HilbertCurveGen hbCurve(1);
vector<float> curveVer;
bool curveChanged = false;
bool n_key_held = false;
bool b_key_held = false;

const int WIDTH = 512;
const int HEIGHT = 512;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !n_key_held)
	{
		n_key_held = true;
		hbCurve.increaseLevel();
		curveVer = hbCurve.getVerticies();
		curveChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE)
		n_key_held = false;

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !b_key_held)
	{
		b_key_held = true;
		hbCurve.decreaseLevel();
		curveVer = hbCurve.getVerticies();
		curveChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
		b_key_held = false;
	
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hilbert Curve", NULL, NULL);

	if (!window)
	{
		cout << "Failed to create GLFW window. TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	Shader shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	//HilbertCurveGen hbCurve(1);
	curveVer = hbCurve.getVerticies();

	/*for (float x : curveVer)
	{
		cout << x << endl;
	}*/

	GLuint VAO, VBO;
	glGenBuffers(1, &VBO); // gen 1 buffer and store id in VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, curveVer.size() * sizeof(float), curveVer.data(), GL_STATIC_DRAW);

	const int COLOR_COMPONENTS = 0;
	const int VERTEX_COMPONENTS = 3;
	const int COMPONENTS_PER_VERTEX = COLOR_COMPONENTS + VERTEX_COMPONENTS;
	const int STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
	//position attributes
	glVertexAttribPointer(0, VERTEX_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);
	//color attributes
	//glVertexAttribPointer(1, COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)(VERTEX_COMPONENTS * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 trans(1.0f);
	trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.getProgramID());
		//GLint transformLoc = glGetUniformLocation(shader.getProgramID(), "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		
		/*// set color of the triangle by uniform
		float time = glfwGetTime();
		float blueValue = sin(time / 2.0f + 0.5f);
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "uColor");
		glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueValue, 1.0f);
		*/

		// update the verticies of the triangles
		glBindVertexArray(VAO);
		
		//NO NEED TO TRANSFORM HERE, DO IT IN SHADER INSTEAD
		//create new buffer data
		if (curveChanged)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, curveVer.size() * sizeof(float), curveVer.data(), GL_DYNAMIC_DRAW);
			curveChanged = false;	
		}
		
		glDrawArrays(GL_LINE_STRIP, 0, curveVer.size() / 3);
		//glDrawArrays(GL_POINTS, 0, curveVer.size() / 3);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

