#version 410 core

const int NO_LIGHT = 0;                     // lighting disabled
const int DIR_LIGHT = 1;                    // directional lighting
const int POINT_LIGHT = 2;                  // point lighting
const int NO_TEXTURE = 0;                   // texturing disabled
const int TEXTURE = 1;                      // walls are textured
const int BUMP_MAPPING = 2;                 // walls are textured and bump
											// mapping is enabled

uniform mat4 modelView;                     // model-view transform
uniform mat4 projection;                    // projection transform

uniform int lightingMode;                   // NO_LIGHT, DIR_LIGHT, or POINT_LIGHT
uniform int texturingMode;                  // NO_TEXTURE, TEXTURE, BUMP_MAPPING


uniform sampler2D colorMap;                 // color texture for walls
uniform sampler2D bumpMap;                  // bump-map texture for walls

uniform vec3 primColor;                     // the color of a non-texture-mapped
uniform vec3 ecLight;                       // primitive light vector; for DIR_LIGHT,
											// this is the light’s direction vector,
											// while for POINT_LIGHT this is the
											// light’s position. In both cases, the
											// vector is in eye coordinates.
vec4 ambient=vec4(0.25,0.25,0.25,1.0);
vec4 intensity=vec4(1.0,1.0,1.0,1.0);
vec3 direction=vec3(-1.0,-1.0,0);

in vec3 Coord;
in vec3 Norm;
in vec4 TanVec;
in vec2 TexCoord;



out vec4 frag_color;

void main(void)
{
    float i,clamped,l;
    vec4 light,I;
    vec3 normlight;
    float attenuation=1/(length(Coord-ecLight));		// attentuation variable, 1/distance
    
    vec3 n,t,b,pixelNormal,normalizedLightDirection;
    vec3 surfaceToLight;
    vec3 eye_light=mat3(modelView)*ecLight;				// transformation of ecLight into eye space
    

    mat3 tbn,normalMatrix;

    
	//handling of lighting options
    switch(lightingMode){
        case NO_LIGHT:
            light=vec4(1.0,1.0,1.0,1.0);		//no lighting, use flat shading
            break;
        case DIR_LIGHT:							// directional lighting
            i=max(0,dot(Norm,ecLight));			/* dot normal and ecLight (direction of
												 light)to find angle of incidence,
												"max" is used to prevent shading from
												 occuring if surface is facing away 
												 from the lighting source*/
            light=(i*intensity)+ambient;		// plug value (i=brightness) into equation for calculating lighting
			
			// handling of bump mapping under directional lighting
            if(texturingMode==BUMP_MAPPING){
                n=normalize((modelView*vec4(Norm,0.0)).xyz);
                t=normalize((modelView*vec4(TanVec.xyz,0.0)).xyz);
                b=normalize((modelView*vec4((cross(Norm,TanVec.xyz)*TanVec.w),0.0)).xyz);
                tbn=mat3(t,b,n);			// create tbn matrix, which converts from Tangent Space to World Space
                
				/*convert normal and direction vector into World Space, 
				 so we can take the dot product between the two
				 and find diffuse reflected light*/
                pixelNormal=tbn*normalize(texture(bumpMap,TexCoord.st).xyz*2.0-1.0);
                normalizedLightDirection=normalize(eye_light);
                i=max(0.0,dot(pixelNormal,normalizedLightDirection));
            }
            
           light=(i*intensity)+ambient;
            break;
        case POINT_LIGHT:						//point lighting
			
			/*calculate vector from surface to light source, use this to 
			 calculate the brightness (i)... clamp the value to prevent unnecessary
			 shading on surfaces that are facing away from the light source*/
			
            surfaceToLight=ecLight-Coord;
            //Lamont: This is not the calculation for diffuse component for light. -5
            i=dot(Norm,surfaceToLight)/(length(surfaceToLight)*length(Norm));
            i=clamp(i,0,1);
            
			//handling of bump mapping under point lighting... same process as in DIR_LIGHT
            if(texturingMode==BUMP_MAPPING){
                n=normalize((modelView*vec4(Norm,0.0)).xyz);
                t=normalize((modelView*vec4(TanVec.xyz,0.0)).xyz);
                b=normalize((modelView*vec4((cross(Norm,TanVec.xyz)*TanVec.w),0.0)).xyz);
                tbn=mat3(t,b,n);
                
                pixelNormal=tbn*normalize(texture(bumpMap,TexCoord.st).xyz*2.0-1.0);
                normalizedLightDirection=normalize(ecLight);
                l=max(0.0,dot(pixelNormal,normalizedLightDirection));
                i=i*l;
            }
            
            light=((i*intensity)+ambient)*attenuation;
            break;
  
    }
    //handlign of texture options
    switch(texturingMode){
        case NO_TEXTURE:		// no texture, just use light and color
            frag_color=vec4(primColor,1)*light;
            break;
        case TEXTURE:			// texture, use texture map to color the walls
            frag_color=texture(colorMap,TexCoord)*light;
            break;
        case BUMP_MAPPING:		// bump mapping
            normlight=(2*primColor)-vec3(1.0,1.0,1.0);	//"decode" normals
            //Lamont: You need to use the the former variable to get the correct texel value. 
            frag_color=texture(bumpMap,TexCoord)*light*vec4(normlight,1.0);
            break;
    }
    
}