/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 

in vec4 vColor; 
out vec4 vColor_out;

void main(void)
{
	vColor_out = vColor; 
}