#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "keyframe.h"
#include "helper.h"

Keyframe::Keyframe(const std::string &name, const aiNodeAnim *channel) : boneName(name)
{
    for (unsigned int i = 0; i < channel->mNumPositionKeys; i++)
    {
        KeyPosition keyframe;
        keyframe.position = Vec3ToGLM(channel->mPositionKeys[i].mValue);
        keyframe.timestamp = channel->mPositionKeys[i].mTime;
        positions.push_back(keyframe);
    }
    for (unsigned int i = 0; i < channel->mNumRotationKeys; i++)
    {
        KeyRotation keyframe;
        keyframe.orientation = QuatToGLM(channel->mRotationKeys[i].mValue);
        keyframe.timestamp = channel->mRotationKeys[i].mTime;
        rotations.push_back(keyframe);
    }

    for (unsigned int i = 0; i < channel->mNumScalingKeys; i++)
    {
        KeyScale keyframe;
        keyframe.scale = Vec3ToGLM(channel->mScalingKeys[i].mValue);
        keyframe.timestamp = channel->mScalingKeys[i].mTime;
        scales.push_back(keyframe);
    }
}

void Keyframe::update(float currentTime)
{
    glm::mat4 translation = interpolatePosition(currentTime);
    glm::mat4 rotation = interpolateRotation(currentTime);
    glm::mat4 scale = interpolateScale(currentTime);
    localTransform = translation * rotation * scale;

}

glm::mat4 Keyframe::interpolatePosition(float currentTime)
{
    if (positions.size() == 1)
    {
        glm::mat4 trans = glm::translate(glm::mat4(1.0f), positions[0].position);
        return trans;
    }
    int currentKeyframe = getPositionKeyframe(currentTime);
    float factor = interpolateTime(positions[currentKeyframe].timestamp,
                                   positions[currentKeyframe + 1].timestamp,
                                   currentTime);
    glm::vec3 interpolateVector = glm::mix(positions[currentKeyframe].position,
                                           positions[currentKeyframe + 1].position,
                                           factor);
    return glm::translate(glm::mat4(1.0f), interpolateVector);
}

int Keyframe::getPositionKeyframe(float time)
{
    for (unsigned int i = 0; i < positions.size() - 1; i++)
    {
        if (time < positions[i + 1].timestamp)
            return i;
    }
    return positions.size() - 2;
}

glm::mat4 Keyframe::interpolateRotation(float currentTime)
{
    if (rotations.size() == 1)
    {
        glm::quat interpolateQuat = glm::normalize(rotations[0].orientation);
        return glm::toMat4(interpolateQuat);
    }
    int currentKeyframe = getRotationKeyframe(currentTime);
    float factor = interpolateTime(rotations[currentKeyframe].timestamp,
                                   rotations[currentKeyframe + 1].timestamp,
                                   currentTime);
    glm::quat interpolateQuat = glm::slerp(rotations[currentKeyframe].orientation,
                                           rotations[currentKeyframe + 1].orientation,
                                           factor);
    interpolateQuat = glm::normalize(interpolateQuat);
    return glm::toMat4(interpolateQuat);
}

int Keyframe::getRotationKeyframe(float time)
{
    for (unsigned int i = 0; i < rotations.size() - 1; i++)
    {
        if (time < rotations[i + 1].timestamp)
            return i; 
    }
    return rotations.size() - 2;
}

glm::mat4 Keyframe::interpolateScale(float currentTime)
{
    if (scales.size() == 1)
    {
        return glm::scale(glm::mat4(1.0f), scales[0].scale);
    }
    int currentKeyframe = getScaleKeyframe(currentTime);
    float factor = interpolateTime(scales[currentKeyframe].timestamp,
                                   scales[currentKeyframe + 1].timestamp,
                                   currentTime);
    glm::vec3 interpolateVector = glm::mix(scales[currentKeyframe].scale,
                                           scales[currentKeyframe + 1].scale,
                                           factor);
    return glm::scale(glm::mat4(1.0f), interpolateVector);
}

int Keyframe::getScaleKeyframe(float time)
{
    for (unsigned int i = 0; i < scales.size() - 1; i++)
    {
        if (time < scales[i + 1].timestamp)
            return i;
    }
    return scales.size() - 2;
}

float Keyframe::interpolateTime(float previousTime, float nextTime, float currentTime)
{
    float delta = nextTime - previousTime;
    float diff = currentTime - previousTime;
    float interpolation = diff / delta;
    return interpolation;
}


std::string Keyframe::getBoneName()
{
    return boneName;
}

glm::mat4 Keyframe::getLocalTransform()
{
    return localTransform;
}