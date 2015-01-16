#version 410 core
uniform vec4 color;
in vec3 vNorm;
out vec4 color_out;



vec4 light=vec4(1.0,1.0,1.0,1.0);
vec4 ambient=vec4(0.25,0.25,0.25,1.0);
vec3 direction=vec3(-1,-1,0); //Lamont: This is the direction from the light to the surface. You want the direction from the surface to light. -2 

void main(void)
{
	//Lamont: You need documentation for this.
    float intensity= dot(direction,vNorm);
    intensity =max(0,intensity);
    vec4 I=intensity*light;
    color_out = (I+ambient)*color;
}