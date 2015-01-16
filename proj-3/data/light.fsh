#version 410 core
layout (location = 0) out vec4 fragColor;


void main (void)
{
    // output for debugging
    fragColor=vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1.0);

}
