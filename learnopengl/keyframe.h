#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <iostream>

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct KeyPosition
{
    glm::vec3 position;
    float timestamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timestamp;
};

struct KeyScale
{
    glm::vec3 scale; 
    float timestamp;
};

class Keyframe
{
public:
    Keyframe(const std::string &name, int ID, const aiNodeAnim *channel);
    int getBoneID();
    std::string getBoneName();
    glm::mat4 getLocalTransform();
    void update(float currentTime);

private:
    int boneID;
    std::string boneName;
    glm::mat4 localTransform;
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;

    int getPositionKeyframe(float time);
    int getRotationKeyframe(float time);
    int getScaleKeyframe(float time);

    glm::mat4 interpolatePosition(float currentKeyframe);
    glm::mat4 interpolateRotation(float currentKeyframe);
    glm::mat4 interpolateScale(float currentKeyframe);
    float interpolateTime(float previousKeyframe, float nextKeyframe, float currentKeyframe);
};

#endif