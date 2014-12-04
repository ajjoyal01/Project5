#version 330 core

uniform mat4 MVMatrix;		
uniform mat4 MVPMatrix;
uniform vec3 LightPosition;

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

out float LightIntensity;
out vec2 MCposition;

uniform mat4 VPMatrix;		// view projection matrix

layout(location = 0) in vec4 MCvertex;
layout(location = 1) in vec2 in_texel;
layout(location = 2) in vec3 MCnormal;
layout(location = 3) in int in_textureID;
layout(location = 4) in int	in_isTextured;
layout(location = 5) in int	in_isTransformed;		// if the object is allowed to translate

layout(location = 6) in mat4 ModelMatrix;			// the model translation
layout(location = 10) in vec4 in_color;
layout(location = 11) in mat3 NormalMatrix;

void main()
{
	vec3 ecPosition = vec3(MVMatrix * MCvertex);
	vec3 tnorm = normalize(NormalMatrix * MCnormal);
	vec3 lightVec = normalize(LightPosition - ecPosition);
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = normalize(-ecPosition);
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	float spec = 0.0;

	if (diffuse > 0.0)
	{
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, 16.0);
	}

	LightIntensity = DiffuseContribution * diffuse +
	SpecularContribution * spec;
	MCposition = MCvertex.xy;
	gl_Position = MVPMatrix * MCvertex;
}