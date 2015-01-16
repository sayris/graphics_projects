#version 410 core

layout(location = 0) in vec3 coord;
layout(location = 1) in vec4 color_in;
out vec4 color;

uniform mat4 modelView;
uniform mat4 projection;


void main (void)
{
    gl_Position =  projection * modelView * vec4(coord,1.0);
    color = color_in;
}
