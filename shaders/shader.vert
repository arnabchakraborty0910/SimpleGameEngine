#version 330 core

/*
* This is the vertex shader for all entities. 
* This is very projection matrix calculation happen
*	projection * view * model
* This is also where normals are calculated
*/

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;


void main()
{
	//makes sure the normals match the model
	Normal = mat3(transpose(inverse(model))) * aNormal;

	//this is getting the world space position for objects for lighting in fragment shader
	FragPos = vec3(model * vec4(aPos, 1.0));

	//projection matrix calculations
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	//texture cords
	TexCoord = aTexCoord;
}