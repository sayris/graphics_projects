/*! \file shader.vert
 *
 * \brief The vertex shader for a triangle 
 *
 * \author Lamont Samuels
 *
 */
 
#version 410 core

void main (void)
{
	//We declare the vetrices for the triangle, which are already in its unit coordinates 
   const vec4 vertices[3] = vec4[3](vec4( -1.0, 0.0, 0.0, 1.0), 
									vec4  (0.0, 1.0, 0.0, 1.0), 
									vec4(  0.0, 0.0, 0.0, 1.0)); 

	//We assign gl_Position the unit coordinates for the current vertex (i.e. using gl_VertexID)). 
    gl_Position =  vertices[gl_VertexID]; 
}
