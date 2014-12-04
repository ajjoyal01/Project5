#version 330 core

out vec4 fragColor;

uniform sampler2D tex;

uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightDirection;
uniform vec3 HalfVector; // surface orientation for shiniest spot
uniform float Shininess;
uniform float Strength;
uniform float directionalLightingOn;

in vec4 vertColor;
in vec2 vertTexCoord;
in vec3 vertNormal;
flat in int vertIsTextured;
flat in int textureID;

void main()
{
	// to see how much light would be reflected
	float diffuse = max(0.0, dot(vertNormal, LightDirection));
	float specular = max(0.0, dot(vertNormal, HalfVector));

	// surfaces facing away from the light (negative dot products)
	// wont be lit by the directional light
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess); // sharpen the highlight
	vec3 scatteredLight = Ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;

	// dont modulate the underlying color with reflected light,
	// only with scattered light
	vec3 rgb = min(vertColor.rgb * scatteredLight + reflectedLight, vec3(1.0));

	if(directionalLightingOn == 1) {
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
		{
			fragColor = texture(tex, vertTexCoord);
				
		}
		else 
			fragColor = vertColor;

}