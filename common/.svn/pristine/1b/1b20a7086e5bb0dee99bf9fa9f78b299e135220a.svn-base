#version 150

// Constants that control the appearance of the texture
const vec3  BrickColor = vec3(1.0, 0.3, 0.2);
const float  BrickSpec = 0.3;
const vec3  MortarColor = vec3(0.85, 0.86, 0.84);
const float  MortarSpec = 0.1;
const vec2  BrickSize = vec2(0.30, 0.15);
const vec2  BrickPct = vec2(0.90, 0.85);

// The position of a single light source in eye coordinates
uniform vec3 lightPosition;

// Inputs from the vertex shader
in VS_OUT {
    vec3  coord;	// eye coordinates fragment position
    vec3  norm;		// surface normal
    vec2  mcPos;	// "texture" coordinates
} fsIn;

out vec4 fragColor;

void main ()
{
  // compute coordinate in brick-space
    vec2 position = fsIn.mcPos / BrickSize;

  // adjustment for odd row offset
    if (fract(position.y * 0.5) > 0.5)
        position.x += 0.5;

  // position in the brick
    position = fract(position);

  // is it brick or mortar?
    vec2 useBrick = step(position, BrickPct);

  // compute color and specular/diffuse contributions
    vec3 color = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    float specularContribution = mix(MortarSpec, BrickSpec, useBrick.x * useBrick.y);
    float diffuseContribution = 1.0 - specularContribution; 

  // the unit vector in the direction from the vertex to the light
    vec3 lightVec   = normalize (lightPosition - fsIn.coord);
  // the unit reflection vector
    vec3 reflectVec = reflect (-lightVec, fsIn.norm);
  // the unit view vector
    vec3 viewVec = normalize (-fsIn.coord);
  // compute the diffuse lighting
    float diffuse = max(dot(lightVec, fsIn.norm), 0.0);
  
  // compute the specular lighting
    float spec;
    if (diffuse > 0.0) {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }
    else
	spec = 0.0;

  // The light intensity combines diffuse and specular
    float intensity  = diffuseContribution * diffuse +
                      specularContribution * spec;

  // scale by light intensity
    color *= intensity;

  // output for the frame buffer
    fragColor = vec4 (color, 1.0);
}
