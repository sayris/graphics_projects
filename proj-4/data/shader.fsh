// FRAGMENT SHADER
// Project 4

#version 330 core

// enums for rendering
const int WIREFRAME = 0;
const int TEXTURE = 1;

// enums for lighting
const int NO_LIGHT = 0;
const int DIR_LIGHT = 1;

// enums for fog
const int FOG = 0; //!< enables fog
const int NOFOG = 1;

uniform mat4 modelView;                // model-view transform
uniform mat4 projection;               // projection transform

uniform int lightingMode;              // NO_LIGHT, DIR_LIGHT
uniform int texturingMode;			// WIREFRAME, TEXTURE
uniform int fogMode;                    // FOG, NO_FOG

uniform sampler2D colorMap;            // texture map
uniform sampler2D normMap;				// height map

uniform vec3 primColor;					// color of primitive

uniform vec3 sunDir;
uniform vec3 intensity;
uniform vec3 ambient;

uniform float hScale;

//vec4 fog_Color = vec4(0.2f, 0.2f, 0.4f, 1.0f);
const vec4 fog_Color = vec4(1.0, 1.0, 1.0, 1.0);
const float start = 10.0f;
const float end = 100.0f;

in vec4 Coord;
in vec3 Norm;
in vec3 TanVec;
in vec2 TexCoord;

out vec4 frag_color;

void main(void)
{
    vec4 color;
	vec3 Norm = 2.0 * texture(normMap, TexCoord).xyz - vec3(1.0,1.0,1.0);
    Norm = normalize(Norm);
	vec4 light = vec4(1.0,1.0,1.0,1.0);
    
    
    

	
	
    if(texturingMode == WIREFRAME){
        color=vec4(primColor,1.0);
    } else if(texturingMode == TEXTURE){
        
        if(lightingMode == DIR_LIGHT){
            light = 3.5 * vec4(((max(0,dot(Norm,sunDir)))*intensity)+ambient, 1.0);

        }
        color=texture(colorMap,TexCoord)*light;
    }
    
    if(fogMode == FOG){
        float dist = length(-vec4(Coord.x,Coord.y,Coord.z,1.0));
        float f = (dist - start) / (hScale * end - start);
        f = clamp(f, 0.0, 1.0);
        color = mix(color, fog_Color, f);
//        color=vec4(1.0,0.0,0.0,1.0);
 
    }
    
    
    frag_color = color;
    
}