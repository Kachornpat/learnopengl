#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>

#include <glm/glm.hpp>

#include "keyframe.h"

struct AnimationNode {
    glm::mat4 transformation;
    std::string name;
    std::vector<AnimationNode> children;
};


class Animation {

public:
    Animation(std::string path){
        loadAnimation(path);
    }
    Keyframe* getKeyframe(std::string boneName);
    int getTickPerSecond();
    float getDuration();
    AnimationNode& getRootNode();

private:
    float duration;
    int tickPerSecond;
    AnimationNode root;
    std::vector<Keyframe> keyframes;
    void loadAnimation(std::string path);
    void processKeyframe(const aiAnimation* animation);
    void processNode(AnimationNode &parent, const aiNode* node);
};

#endif