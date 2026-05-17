#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>

#include <glm/glm.hpp>

#include "model.h"
#include "keyframe.h"

struct AnimationNode {
    glm::mat4 transformation;
    std::string name;
    std::vector<AnimationNode> children;
};


class Animation {

public:
    Animation(std::string path, Model* model){
        loadAnimation(path, model);
    }
    Keyframe* getKeyframe(std::string boneName);
    int getTickPerSecond();
    float getDuration();
    AnimationNode& getRootNode();
    std::vector<Bone> getBones();

private:
    std::vector<Bone> bones;
    float duration;
    int tickPerSecond;
    AnimationNode root;
    std::vector<Keyframe> keyframes;
    void loadAnimation(std::string path, Model* model);
    void processKeyframe(const aiAnimation* animation, Model* model);
    void processNode(AnimationNode &parent, const aiNode* node, Model *model);
};

#endif