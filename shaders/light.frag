#version 330 core
/*
* This is the light fragment shader. The lightColor variable is given a value manually through code
*/
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0f);


}