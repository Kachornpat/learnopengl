#include "player.h"
#include "model.h"

Player::Player(Animation *animation, Model *model)
{
    this->animation = animation;
    this->model = model;
    currentTime = 0.0f;
    transformations.reserve(100);
    for (unsigned int i = 0; i < 100; i++)
        transformations.push_back(glm::mat4(1.0f));
}

void Player::updateTime(float deltaTime)
{
    if (animation)
    {
        currentTime += animation->getTickPerSecond() * deltaTime;
        currentTime = fmod(currentTime, animation->getDuration());
        calculateTransformation(&animation->getRootNode(), glm::mat4(1.0f));
    }
}

void Player::calculateTransformation(const AnimationNode *node, glm::mat4 parentTransform)
{
    Keyframe *keyframe = animation->getKeyframe(node->name);
    glm::mat4 nodeTransform = glm::mat4(1.0f);

    if (keyframe)
    {   
        keyframe->update(currentTime);
        nodeTransform = keyframe->getLocalTransform();
    }
    else
    {
        nodeTransform = node->transformation;
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    int boneID = model->getBoneID(node->name);
    if (boneID != -1)
    {
        Bone bone = model->getBones()[boneID];
        transformations[boneID] = globalTransformation * bone.offset;
    }



    for (unsigned int i = 0; i < node->children.size(); i++)
    {
        calculateTransformation(&node->children[i], globalTransformation);
    }

}

std::vector<glm::mat4> Player::getTranformations()
{
    return transformations;
}

void Player::setAnimation(Animation *animation)
{
    this->animation = animation;
}
