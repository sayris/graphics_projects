//VERTEX SHADER
//Project 4

#version 410 core

layout (location = 0) in vec4 coord;        // object coords for vertex position
layout (location = 1) in vec3 norm;         // object coords for vertex normal
layout (location = 2) in vec2 texCoord;     // texture coordinate


//output for fragment shader
out vec4 Coord;
out vec2 TexCoord;

uniform mat4 modelView;                     // model-view transform
uniform mat4 projection;                    // projection transform

void main (void)
{
    gl_Position = projection * modelView * vec4(coord.xyz, 1.0);
    Coord = modelView * vec4(coord.xyz, 1.0);
    TexCoord = texCoord;
}
