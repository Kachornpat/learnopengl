#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>

#include "animation.h"

class Player{
public:
    Player(Animation *animation);
    void updateTime(float deltaTime);
    void setAnimation(Animation *animation);
    std::vector<glm::mat4> getTranformations();

private:
    std::vector<Bone> bones;
    Animation* animation;
    float currentTime;
    std::vector<glm::mat4> transformations;
    void calculateTransformation(const AnimationNode* node, glm::mat4 parentTransform);
};

#endif