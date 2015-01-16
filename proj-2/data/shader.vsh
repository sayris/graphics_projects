#version 410 core

layout (location = 0) in vec3 coord;        // object coords for vertex position
layout (location = 1) in vec3 norm;         // object coords for vertex normal
layout (location = 2) in vec4 tanVec;       // tangent vector with bitangent sign
layout (location = 3) in vec2 texCoord;     // texture coordinate


//output for fragment shader
out vec3 Coord;
out vec3 Norm;
out vec4 TanVec;
out vec2 TexCoord;

uniform mat4 modelView;                     // model-view transform
uniform mat4 projection;                    // projection transform

void main (void)
{
    gl_Position =  projection * modelView * vec4(coord,1.0);
    Coord=vec3(modelView*vec4(coord,1.0));
			// coordinated/positions converted into World Space
    Norm=vec3(transpose(inverse(modelView))*vec4(norm,1.0));
			//normals converted into World Space
    TanVec=tanVec;
    TexCoord=texCoord;
}
