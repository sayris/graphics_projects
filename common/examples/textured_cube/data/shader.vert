/*! \file shader.vert
 *
 * \brief The vertex shader for a cube
 *
 * \author Lamont Samuels
 *
 */
#version 410 core

layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 norm; 
layout(location = 2) in vec2 tCoord; 

out vec3 norm_out; 
out vec2 tCoord_out; 

uniform mat4 modelView; 
uniform mat4 projection;

void main (void)
{
    gl_Position =  projection * modelView * vec4(position,1.0); 
    norm_out = norm; 
    tCoord_out = tCoord; 
}
