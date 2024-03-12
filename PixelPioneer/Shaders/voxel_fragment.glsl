#version 330 core
out vec4 FragColor;

smooth in float ao;
in vec2 TexCoord;
in vec3 normal;
in vec2 wh;
flat in int texIdx;

// texture samplers
uniform sampler2DArray texArray;

uniform vec3 lightDirection;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform vec3 lightColor;
uniform bool solid;
uniform bool enableAO;

float centerDist() {
	return abs(TexCoord.x / wh.x - 0.5) + abs(TexCoord.y/ wh.y - 0.5);
}

float centerDist0() {
	return max(abs(TexCoord.x / wh.x - 0.5), abs(TexCoord.y / wh.y - 0.5));
}

float cornerDist0() {
	return 0;
	return min(min(abs(TexCoord.x / wh.x), abs(1 - TexCoord.x / wh.x)),
		min(abs(TexCoord.y / wh.y), abs(1 - TexCoord.y / wh.y)));
}

void main()
{

	if (solid) {
		FragColor = vec4(0, 0, 0, 1);
		return;
	}
    vec3 ambient = ambientIntensity * lightColor;
	float diff = max(dot(normal, normalize(-lightDirection)), 0.0) * diffuseIntensity;
	vec3 diffuse = diff * lightColor;
	float fixedAO;

	fixedAO = max(ao-0.3,0);

	FragColor = vec4( (ambient + diffuse )*(1- fixedAO), 1.0) * texture(texArray, vec3(TexCoord.xy, texIdx));
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}