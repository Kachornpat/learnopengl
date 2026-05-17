#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;


const int MAX_BONES = 100;
const int MAX_BONES_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
	mat4 transform = mat4(1.0f);

	for (int i = 0; i < 65; i++)
	{
		if (boneIds[0] == i)
			transform = finalBonesMatrices[boneIds[0]] * weights[0];
	}

	

	gl_Position = projection * view * model * transform * vec4(aPos, 1.0f);
	FragPos = vec3(model * transform * vec4(aPos, 1.0f));
	Normal = normalMat * aNormal;
	TexCoords = aTexCoords;
}