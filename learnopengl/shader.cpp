#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexSource;
	std::string fragmentSource;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexSource = vShaderStream.str();
		fragmentSource = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	const char* vShaderSource = vertexSource.c_str();
	const char* fShaderSource = fragmentSource.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" <<
			infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	modelLoc = glGetUniformLocation(ID, "model");
	viewLoc = glGetUniformLocation(ID, "view");
	projectionLoc = glGetUniformLocation(ID, "projection");

}

void Shader::use() const{
	glUseProgram(ID);
}

void Shader::updateModel(glm::mat4 model) const
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::updateView(glm::mat4 view) const
{
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::updateProjection(glm::mat4 projection) const
{
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::setInt(std::string uniform, int value)
{
	unsigned int valueLoc = glGetUniformLocation(ID, uniform.c_str());
	glUniform1i(valueLoc, value);
}

void Shader::setFloat(std::string uniform, float value)
{
	unsigned int transLoc = glGetUniformLocation(ID, uniform.c_str());
	glUniform1f(transLoc, value);
}


void Shader::setVec(std::string uniform, glm::vec3 vector)
{
	unsigned int transLoc = glGetUniformLocation(ID, uniform.c_str());
	glUniform3fv(transLoc, 1, glm::value_ptr(vector));
}

void Shader::setMat(std::string uniform, glm::mat4 trans)
{
	unsigned int transLoc = glGetUniformLocation(ID, uniform.c_str());
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Shader::setMat(std::string uniform, glm::mat3 trans)
{
	unsigned int transLoc = glGetUniformLocation(ID, uniform.c_str());
	glUniformMatrix3fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));
}