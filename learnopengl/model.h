#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "mesh.h"
#include "shader.h"

class Model {
private:
	std::vector<Mesh> meshes;

public:
	void Draw(Shader &shader);
};


#endif
