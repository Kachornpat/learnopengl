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

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//atenuation
	float constant;
	float linear;
	float quadratic;
};

struct Light {
	vec3 direction;
	vec3 position;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//atenuation
	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform vec3 viewPos;

vec3 directionalLight(DirLight light,vec3 normal, vec3 lightDir);
vec3 pointLighting(PointLight light, vec3 normal);

void main()
{

	vec3 normal = normalize(Normal);
	// vec3 lightDir = normalize(light.position - FragPos);
	// float theta = dot(lightDir, normalize(-light.direction));
	// float distance = length(light.position - FragPos);
	// float attenuation =  1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	// float epsilon = light.cutOff - light.outerCutOff;
	// float intensity = clamp((theta - light.outerCutOff)/ epsilon, 0.0f, 1.0f);

	// vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// float antenuation = 0.0f;
	// vec3 norm = normalize(Normal);
	// float diff = max(dot(norm, lightDir), 0.0f);
	// vec3 diffuse = diff * light.diffuse * attenuation * intensity * vec3(texture(material.diffuse, TexCoords));

	// vec3 viewDir = normalize(viewPos - FragPos);
	// vec3 reflectDir = reflect(-lightDir, norm);
	// float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	// vec3 specular = spec * light.specular * attenuation * intensity * vec3(texture(material.specular, TexCoords));

	// vec3 result = ambient + diffuse + specular;
	// result += directionalLight(dirLight, normal, -dirLight.direction);
	vec3 result = pointLighting(pointLight, normal);
	FragColor = vec4(result, 1.0f);

}

vec3 directionalLight(DirLight light,vec3 normal, vec3 lightDir)
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 pointLighting(PointLight light, vec3 normal)
{
	vec3 lightDir = normalize(light.position - FragPos);
	float distance = length(light.position - FragPos);
	float attenuation =  1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	float antenuation = 0.0f;
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * light.diffuse * attenuation * vec3(texture(material.diffuse, TexCoords));

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
	vec3 specular = spec * light.specular * attenuation * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}
