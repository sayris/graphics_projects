/*! \file default.vsh
 *
 * \brief Modelling and rendering of the skeleton or wireframe. 
 *
 * \author Lamont Samuels
 */

/* CMSC23700 Project 3 Sample code
 *
 * COPYRIGHT (c) 2014 Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#version 410 core
const int SKELETON=0;
const int WIREFRAME=1;
const int MODEL=2;
const int SHADOWS=3;


uniform mat4 modelView;			// model-view transform
uniform mat4 projection;		// projection transform
uniform int renderMode;         // mode of renderer
uniform mat4 lightMatrix;         // mode of renderer
uniform mat4 BiasMat;         // mode of renderer




layout (location = 0) in vec3 coord;	// vertex position
layout (location = 1) in vec3 norm;         // object coords for vertex normal
layout (location = 2) in vec2 texCoord;     // texture coordinate

out vec3 Coord;
out vec3 Norm;
out vec2 TexCoord;
out vec4 ShadowCoord;




void main ()
{
  // pass vertex data through to fragment shader
    gl_Position = projection * modelView * vec4(coord,1);	// view clip space 
    Coord=coord;
    Norm=norm;
    TexCoord=texCoord;
    
    if(renderMode==SHADOWS){
        ShadowCoord=BiasMat*lightMatrix*vec4(coord,1);  // light clip space
    }
}