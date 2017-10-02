#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader 
{
	public: 
		
		Shader(std::string vertexPath, std::string fragmentPath);
		GLuint getProgramID();
	
	private:
		GLuint ID;
		GLint addShader(std::string shaderPath, GLuint type);
		std::string parseShader(std::string shaderPath);
};
#endif
