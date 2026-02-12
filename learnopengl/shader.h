#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	void setVec3f();
	void setMat4f();
	void use() const;
};

#endif
