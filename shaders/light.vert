#version 330 core
/*
* this is the vertex shader for the light source. This takes the light model matrix, camera view matrix, 
* and projection matrix to determine the position of the light on the screen
*/
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

}