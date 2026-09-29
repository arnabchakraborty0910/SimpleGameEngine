#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

void main()
{
	//textures 

	//ambient light
	float ambientStrength = 0.7;
	vec3 ambient = ambientStrength * objectColor;

	//diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//specularLighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;

	float K_c = 1.0f;
	float K_l = 0.0f;
	float K_q = 0.032;
	float d = length(lightPos - FragPos);
	float att = 1.0 / (K_c + K_l * d + K_q * d * d);

	//vec3 result = (ambient + diffuse + specular) * objectColor; //adding color
	vec3 result = (ambient + diffuse * (att) + specular * (att));

	//FragColor = vec4(result, 1.0) *  mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2); //with textures
	//FragColor = vec4(result, 1.0); //without textures;
	FragColor = vec4(result, 1.0) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * vec4(objectColor, 1.0);

}
