#version 410 core

uniform int renderMode;         // mode of renderer
uniform mat4 lightMatrix;         // mode of renderer





layout (location = 0) in vec3 coord;	// vertex position in model space


void main ()
{
    //transform into light view
    gl_Position = lightMatrix*vec4(coord,1);	


}