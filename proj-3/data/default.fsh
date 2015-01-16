/*! \file default.fsh
 *
 * \brief Coloring of the skeleton or wireframe.
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
uniform bool isFloor;

uniform sampler2D colorMap;     //color texture for walls
uniform sampler2DShadow ShadowMap;    // shadow map... depth buffer


uniform vec3 primColor;	// color of the primtive


//lighting information
vec4 ambient=vec4(0.25,0.25,0.25,1.0);
vec4 intensity=vec4(1.0,1.0,1.0,1.0);
vec3 direction=vec3(1.0,1.0,0.5);

in vec3 Coord;
in vec3 Norm;
in vec2 TexCoord;
in vec4 ShadowCoord;



layout (location = 0) out vec4 fragColor;


void main (void)
{
    float shadow=1.0;
    vec4 light;

    vec4 color=vec4(primColor,1.0);

    switch(renderMode){
        case SKELETON:
        case WIREFRAME:
            break;      //if WIREFRAME or SKELETON mode, just use primColor
        case MODEL:
            if(!isFloor){
                //use the texture if in MODEL mode and not the floor
                color = texture(colorMap,TexCoord);
            }
            break;
        case SHADOWS:
            
            // calculating the shadow "factor"
            // subtract ShadowCoord by .005 to help get rid of some shadow acne
            shadow=0.5+0.5*texture(ShadowMap,(ShadowCoord.xyz-.005));
            
            //calculate directional lighting
            light=vec4(vec3(ambient.xyz)+shadow*vec3(intensity.xyz)*clamp(dot(direction,Norm),0,1),1);
            
            if(isFloor){       //if it is the floor
                color=color*light;
            } else {            // if guard
                color=texture(colorMap,TexCoord)*light;
            }
            break;
    }
    fragColor=color;
    
}
