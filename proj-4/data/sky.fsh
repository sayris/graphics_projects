// FRAGMENT SHADER
// Project 4

#version 410 core

const int SKY = 0; //!< enables skybox
const int NOSKY = 1;

in vec4 Coord;
in vec3 Norm;
in vec2 TexCoord;

out vec4 frag_color;

uniform sampler2D skyMap;  


void main(void)
{
	frag_color = texture(skyMap, TexCoord);
}