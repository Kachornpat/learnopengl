#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"
#include "keyframe.h"
#include "animation.h"
#include "helper.h"

void Animation::loadAnimation(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    if (!scene || !scene->mRootNode)
    {
        std::cout << "ERROR:ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    aiAnimation* animation = scene->mAnimations[0];
    duration = animation->mDuration;
    tickPerSecond = animation->mTicksPerSecond;
    processKeyframe(animation);
    processNode(root, scene->mRootNode);
}

void Animation::processKeyframe(const aiAnimation *animation)
{
    for (unsigned int i = 0; i < animation->mNumChannels; i++)
    {
        aiNodeAnim *channel = animation->mChannels[i];
        std::string nodeName = channel->mNodeName.C_Str();
        keyframes.push_back(Keyframe(nodeName, channel));
    }
}

void Animation::processNode(AnimationNode &parent, const aiNode *node)
{
    parent.name = node->mName.data;
    parent.transformation = Mat4ToGLM(node->mTransformation);
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        AnimationNode child;
        processNode(child, node->mChildren[i]);
        parent.children.push_back(child);

    }
}

Keyframe* Animation::getKeyframe(std::string boneName){
    for (unsigned int i = 0; i < keyframes.size(); i++)
    {
        if (keyframes[i].getBoneName().compare(boneName) == 0)
        {
            return &keyframes[i];
        }
 
    }
    return NULL;
}

int Animation::getTickPerSecond()
{
    return tickPerSecond;
}

float Animation::getDuration()
{
    return duration;
}

AnimationNode& Animation::getRootNode()
{
    return root;
}
