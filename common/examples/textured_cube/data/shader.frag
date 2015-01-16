/*! \file shader.vert
 *
 * \brief The fragment shader for a cube 
 *
 * \author Lamont Samuels 
 *
 */
#version 410 core 


in vec2 tCoord_out;

uniform sampler2D texSampler;  

out vec4 frag_color;

void main(void)
{
	//frag_color = texelFetch(tex_object,ivec2(gl_FragCoord.xy),0); 
	frag_color = texture(texSampler,tCoord_out); 
}