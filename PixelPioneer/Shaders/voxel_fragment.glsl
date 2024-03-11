#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 normal;
flat in int texIdx;

// texture samplers
uniform sampler2DArray texArray;

uniform vec3 lightDirection;
uniform float ambientIntensity;
uniform vec3 lightColor;

void main()
{
    vec3 ambient = ambientIntensity * lightColor;
	float diff = max(dot(normal, -lightDirection), 0.0) * (1-ambientIntensity);
	vec3 diffuse = diff * lightColor;

	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	FragColor = vec4(ambient + diffuse,1.0)*texture(texArray, vec3(TexCoord.xy,texIdx));
}