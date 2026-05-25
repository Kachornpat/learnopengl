#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "animation.h"
#include "model.h"

class Player{
public:
    Player(Animation *animation, Model *model);
    void updateTime(float deltaTime);
    void setAnimation(Animation *animation);
    std::vector<glm::mat4> getTranformations();

private:
    std::vector<Bone> bones;
    Animation* animation;
    Model* model;
    float currentTime;
    std::vector<glm::mat4> transformations;
    void calculateTransformation(const AnimationNode* node, glm::mat4 parentTransform);
};

#endif