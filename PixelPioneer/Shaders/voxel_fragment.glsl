#version 330 core
out vec4 FragColor;

smooth in float ao;
in vec2 TexCoord;
in vec3 normal;
flat in int texIdx;

// texture samplers
uniform sampler2DArray texArray;

uniform vec3 lightDirection;
uniform float ambientIntensity;
uniform float diffuseIntensity;

uniform vec3 lightColor;
uniform bool solid;
uniform bool enableAO;

void main()
{

	if (solid) {
		FragColor = vec4(0, 0, 0, 1);
		return;
	}
    vec3 ambient = ambientIntensity * lightColor;
	float diff = max(dot(normal, normalize(-lightDirection)), 0.0) * diffuseIntensity;
	vec3 diffuse = diff * lightColor;
	
	float fixedAO = max(ao-0.3,0);

	FragColor = vec4( (ambient + diffuse ) * (1- fixedAO), 1.0) * texture(texArray, vec3(TexCoord.xy, texIdx));
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}