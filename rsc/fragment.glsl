#version 410 core
out vec4 FragColor;
in vec4 vertexColor;	// used for input from vertex shader
uniform vec4 uColor;			// used for input from program (CPU)

void main()
{
	FragColor = vec4(0.75, 0.75, 0.75, 1.0);
}