#ifndef MODEL_H
#define MODEL_H
#include <vector>

#include "mesh.h"
#include "shader.h"

class Model {
private:
	std::string directory;
	std::vector<Mesh> meshes;
	void loadModel(std::string path);

public:
	void Draw(Shader &shader);
};


#endif
