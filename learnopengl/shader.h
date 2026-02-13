#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
private:
	unsigned int modelLoc;
	unsigned int viewLoc;
	unsigned int projectionLoc;
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void updateModel(glm::mat4 model) const;
	void updateView(glm::mat4 view) const;
	void updateProjection(glm::mat4 projection) const;
	void setInt(std::string uniform, int value);
	void setFloat(std::string uniform, float value);
	void setVec(std::string uniform, glm::vec3 vector);
	void setMat(std::string uniform, glm::mat4 trans);
	void setMat(std::string uniform, glm::mat3 trans);
	void use() const;
};

#endif
