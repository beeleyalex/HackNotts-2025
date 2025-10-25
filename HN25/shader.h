#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum class ShaderType
{
	Object, Lighting, Grid
};

class Shader
{
public:
	Shader()
	{}

	/// <summary>
	/// Create fully compiled and linked encapsulated GLSL Shader Program.
	/// </summary>
	/// <param name="vertexShaderSource">String filepath to vertex shader source code.</param>
	/// <param name="fragmentShaderSource">String filepath to fragment shader source code.</param>
	Shader(std::string vertexShaderSource, std::string fragmentShaderSource)
	{
		std::ifstream reader;
		std::stringstream buf;

		// read vertex shader into string
		reader.open(vertexShaderSource);
		if (reader.fail())
		{
			std::cout << "Error : Failed to open vertex shader " << vertexShaderSource << std::endl;
			exit(-1);
		}

		buf << reader.rdbuf();
		if (buf.fail())
		{
			std::cout << "Error : Failed to read vertex shader " << vertexShaderSource << std::endl;
			exit(-1);
		}
		std::string vert = buf.str();
		const char* vertexShaderCode = vert.c_str();
		//std::cout << "S: " << s << "\nC: " << vertexShaderCode << std::endl;
		// clear buffer
		buf.str(std::string());
		buf.clear();

		reader.close();
		reader.clear();

		// read fragment shader into string
		reader.open(fragmentShaderSource);
		if (reader.fail())
		{
			std::cout << "Error : Failed to open fragment shader " << vertexShaderSource << std::endl;
			exit(-1);
		}

		buf << reader.rdbuf();
		if (buf.fail())
		{
			std::cout << "Error : Failed to read fragment shader " << fragmentShaderSource << std::endl;
			exit(-1);
		}
		std::string frag = buf.str();
		const char* fragmentShaderCode = frag.c_str();
		// clear buffer
		buf.str(std::string());
		buf.clear();

		// ============

		// shader setup
		int success;
		char errorLog[512];

		unsigned int vertexShader, fragmentShader;

		// vertex shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
			std::cout << "Error : Vertex Shader Compilation Failed\n" << errorLog << std::endl;
		}

		// fragment shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
			std::cout << "Error : Fragment Shader Compilation Failed\n" << errorLog << std::endl;
		}
		// ============

		// shader program
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);
			std::cout << "Error : Shader Program Linking Failed\n" << errorLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		// ==============
	}

	/// <summary>
	/// Activate shader program, calling `glUseProgram`
	/// </summary>
	void activate()
	{
		glUseProgram(shaderProgram);
	}

	void setUniform1Int(std::string uniformName, int value)
	{
		int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
		glUniform1i(uniformLocation, value);
	}

	void setUniform1Float(std::string uniformName, float value)
	{
		int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
		glUniform1f(uniformLocation, value);
	}

	void setUniform4Float(std::string uniformName, float value1, float value2, float value3, float value4)
	{
		int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
		glUniform4f(uniformLocation, value1, value2, value3, value4);
	}

	void setUniformMat4(std::string uniformName, glm::mat4 matrix)
	{
		int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void setUniformVec3(std::string uniformName, glm::vec3 vector)
	{
		int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
		glUniform3fv(uniformLocation, 1, &vector[0]);
	}

private:
	// shader program
	unsigned int shaderProgram;
	// ==============

};

#endif