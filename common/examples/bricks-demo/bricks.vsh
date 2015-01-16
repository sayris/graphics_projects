#version 150

// The specular and diffuse contributions
const float SpecularContribution = 0.3;
const float DiffuseContribution  = 1.0 - SpecularContribution;

uniform mat4 modelView;
uniform mat3 normalMatrix;
uniform mat4 projection;

in vec3 coord;		// vertex position
in vec3 norm;		// vertex normal

// Outputs for the fragment shader
out VS_OUT {
    vec3  coord;	// eye coordinates fragment position
    vec3  norm;		// surface normal
    vec2  mcPos;	// "texture" coordinates
} vsOut;

void main ()
{
  // vertex position in eye coordinates
    vsOut.coord = vec3 (modelView * vec4(coord, 1.0));

  // the transformed surface normal
    vsOut.norm = normalize (normalMatrix * norm);

  // The position of the vertex in the texture
    vsOut.mcPos = coord.xy;

  // the clip coordinate of the vertex
    gl_Position = projection * vec4(vsOut.coord, 1);
}
