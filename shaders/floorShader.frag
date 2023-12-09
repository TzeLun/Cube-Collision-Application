#version 330 core
struct Material {
	sampler2D diffuse; // diffuse map only needed for this application
	float shininess; // the exponent of the pow()
};
struct Light {
	vec3 position; // to replace lightPos
	vec3 ambient; // functions as ambientStrength, set to a low value in a uniform vector
	vec3 diffuse; // to replace lightColor, can be adjusted
	vec3 specular; // to replace lightColor, kept at vec3(1.0f)
	vec3 spotlightDir; // direction of spotlight (aim)
	float innerCutOff; // the inner cut off angle of the spotlight
	float outerCutOff; // the outer cut off angle of the spotlight
	float constant;
	float linear;
	float quadratic;
};

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
out vec4 FragColor;
uniform Material floor;
uniform Light slight;
uniform vec3 viewPos;

void main()
{
	// Ambient lighting
	vec3 ambient = vec3(texture(floor.diffuse, texCoord)) * slight.ambient;
	
	// Diffuse lighting
	vec3 norm = normalize(normal);
	vec3 lightDirVec = normalize(slight.position - fragPos);
	float diffuseAmount = max(dot(norm, lightDirVec), 0.0);
	vec3 diffuse = diffuseAmount * vec3(texture(floor.diffuse, texCoord)) * slight.diffuse;
	
	// Specular lighting
	vec3 viewDirVec = normalize(viewPos - fragPos);
	vec3 reflectDirVec = reflect(-lightDirVec, norm);
	float specularAmount = pow(max(dot(viewDirVec, reflectDirVec), 0.0), floor.shininess);
	vec3 specular = vec3(texture(floor.diffuse, texCoord)) * specularAmount * slight.specular;
	
	// Spotlight intensity
	float theta = dot(lightDirVec, normalize(-slight.spotlightDir));
	float epsilon = slight.innerCutOff - slight.outerCutOff;
	float intensity = smoothstep(0.0, 1.0, (theta - slight.outerCutOff) / epsilon);

	// Light intensity attenuation due to distance
	float distance = length(slight.position - fragPos);
	float attenuation = 1.0 / (slight.constant + slight.linear * distance + slight.quadratic * (distance*distance));
	
	ambient *= attenuation;
	diffuse = diffuse * attenuation * intensity;
	specular = specular * attenuation * intensity;

	vec3 result = ambient + diffuse + specular;

	// Phong lighting
	FragColor = vec4(result, 1.0f);
}