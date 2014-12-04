#version 330 core

out vec4 fragColor;

uniform sampler2D tex;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform float Shininess;
uniform float Strength;

uniform vec3 EyeDirection;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;

uniform float pointLightingOn;

in vec4 vertColor;
in vec3 vertNormal;
in vec2 vertTexCoord;
in vec4 vertPosition;

flat in int vertIsTextured;
flat in int textureID;

void main()
{

	// find the direction and distance of the light,
	// which changes fragment to fragment for a local light
	vec3 lightDirection = LightPosition - vec3(vertPosition);
	float lightDistance = length(lightDirection);

	// normalize the light direction
	lightDirection = lightDirection / lightDistance;

	// model how much light is available for this fragment
	float attenuation = 1.0 / (ConstantAttenuation + 
								LinearAttenuation * lightDistance +
								QuadraticAttenuation * lightDistance * lightDistance);

	vec3 halfVector = normalize(lightDirection + EyeDirection);


	// to see how much light would be reflected
	float diffuse = max(0.0, dot(vertNormal, lightDirection));
	float specular = max(0.0, dot(vertNormal, halfVector));

	// surfaces facing away from the light (negative dot products)
	// wont be lit by the directional light
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength;
	vec3 scatteredLight = Ambient + LightColor * diffuse * attenuation;
	vec3 reflectedLight = LightColor * specular * attenuation;

	// dont modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min(vertColor.rgb * scatteredLight + reflectedLight, vec3(1.0));

	if(pointLightingOn == 1) {
		if(vertIsTextured == 1)
		{
			rgb = min(texture(tex, vertTexCoord).rgb * scatteredLight + reflectedLight, vec3(1.0));
			fragColor = vec4(rgb, texture(tex, vertTexCoord).a);
		}
		else 
		{
			rgb = min(vertColor.rgb * scatteredLight + reflectedLight, vec3(1.0));
			fragColor = vec4(rgb, vertColor.a);
		}
	} else 
		if(vertIsTextured == 1)
			fragColor = texture(tex, vertTexCoord);
		else 
			fragColor = vertColor;

}