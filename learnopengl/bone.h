#ifndef BONE_H
#define BONE_H

#include<glm/glm.hpp>

struct Bone {
    std::string name;
    glm::mat4 offset;
};

#endif