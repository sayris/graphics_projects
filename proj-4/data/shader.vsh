//VERTEX SHADER
//Project 4

#version 330 core

layout (location = 0) in vec4 coord;        // object coords for vertex position
layout (location = 1) in vec3 norm;         // object coords for vertex normal
layout (location = 2) in vec2 texCoord;     // texture coordinate
layout (location = 3) in vec3 tanCoord;     // texture coordinate



//output for fragment shader
out vec4 Coord;
out vec3 Norm;
out vec2 TexCoord;
out vec3 TanVec;


uniform mat4 modelView;                     // model-view transform
uniform mat4 projection;

void main (void)
{
    // why isn't this segfaulting???
	// pretty sure I passed coord to be a vec4f...
    gl_Position =  projection * modelView * vec4(coord.xyz,1.0);
    Coord = coord;
    Norm = norm;
    TexCoord = texCoord;
    TanVec = tanCoord;
}
