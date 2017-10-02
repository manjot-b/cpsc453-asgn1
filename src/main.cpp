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

using namespace std;

vector<float> verticies =
{
	// Triangle 1
	// positions		// colou
	-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,

	// Triangle 2
	0.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f
};
glm::mat4 transform(1.0f);

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
	float dy = 0;
	float dx = 0;
	float rot = 0;
	float speed = 0.002f;
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dy = speed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dy = -speed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		dx = speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		dx = -speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		rot = speed;
	
	transform = glm::translate(transform, glm::vec3(dx, dy, 0.0f));
	transform = glm::rotate(transform, rot, glm::vec3(0.0f, 0.0f, 1.0f));
	/*for (int i = 0; i < verticies.size(); i += 6)
	{
		verticies[i] += dx;		//x-coord
		verticies[i+1] += dy;	//y-coord of each point
	}*/
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
	HilbertCurveGen hbCurve(1);

	GLuint VAO, VBO;
	glGenBuffers(1, &VBO); // gen 1 buffer and store id in VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);

	const int COLOR_COMPONENTS = 3;
	const int VERTEX_COMPONENTS = 3;
	const int COMPONENTS_PER_VERTEX = COLOR_COMPONENTS + VERTEX_COMPONENTS;
	const int STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
	//position attributes
	glVertexAttribPointer(0, VERTEX_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);
	//color attributes
	glVertexAttribPointer(1, COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)(VERTEX_COMPONENTS * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glm::vec4 vec(2.0f, 3.0f, 5.0f, 1.0f);
		
		glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader.getProgramID());
		GLint transformLoc = glGetUniformLocation(shader.getProgramID(), "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
		/*// set color of the triangle by uniform
		float time = glfwGetTime();
		float blueValue = sin(time / 2.0f + 0.5f);
		GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "uColor");
		glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueValue, 1.0f);
		*/

		// update the verticies of the triangles
		glBindVertexArray(VAO);
		
		//NO NEED TO TRANSFORM HERE, DO IT IN SHADER INSTEAD
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//overwrite the exising buffer instead of allocating new memory
		//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * verticies.size(), verticies.data());
		
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

