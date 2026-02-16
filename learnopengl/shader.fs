#version 330
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light {
	vec3 direction;
	vec3 position;
	float cutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//atenuation
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float distance = length(light.position - FragPos);
	float attenuation =  1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	if ( theta > light.cutOff )
	{	
		vec3 ambient = light.ambient * attenuation * vec3(texture(material.diffuse, TexCoords));
		// vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

		float antenuation = 0.0f;
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0f);
		vec3 diffuse = diff * light.diffuse * attenuation * vec3(texture(material.diffuse, TexCoords));

		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
		vec3 specular = spec * light.specular * attenuation * vec3(texture(material.specular, TexCoords));

		vec3 result = ambient + diffuse + specular;
		FragColor = vec4(result, 1.0f);
	}
	else
	{
		FragColor = vec4(light.ambient * attenuation * vec3(texture(material.diffuse, TexCoords)), 1.0f);
	}

}