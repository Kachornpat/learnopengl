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
	vec4 total = vec4(0.0f);
	vec3 localNormal = vec3(0.0f);

	for (int i = 0; i < MAX_BONES_INFLUENCE; i++)
	{
		if (boneIds[i] == -1)
			continue;
		if (boneIds[i] >= MAX_BONES)
		{
			total = vec4(aPos, 1.0f);
			break;
		}
		vec4 local = finalBonesMatrices[boneIds[i]] * vec4(aPos, 1.0f);
		total += local * weights[i];
		localNormal = mat3(finalBonesMatrices[boneIds[i]]) * aNormal;
	}

	gl_Position = projection * view * model * total;
	FragPos = vec3(model * total);
	Normal = normalMat * localNormal;
	TexCoords = aTexCoords;
}