#version 330 core

layout(location = 0) in ivec3 aPos;

out vec2 TexCoord;
out vec3 normal;

out float ao;

out vec2 wh;
flat out int texIdx;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool enableAO;

vec3 getNormal(){
	float t = (aPos.y&1)*2-1;
	return vec3(((aPos.y & 4) >> 2),
		((7 - (aPos.y & 7)) >> 2),
		((aPos.y & 2) >> 1)) * t;
}

vec3 getPos() {
	return vec3(float(aPos.x % 256), float((aPos.x >> 8) % 256), float(aPos.x >> 16));
}


void main()
{
	gl_Position = projection * view * model * vec4(getPos() * 0.66, 1.0);
	//gl_Position = projection * view * vec4(aPos.x * 0.5,aPos.y * 0.5,aPos.z * 0.5, 1.0);
	TexCoord = vec2(
		float( (((aPos.y & 8) >> 3) * (aPos.z % 65536))), 
		float( (((aPos.y & 16) >> 4) * (aPos.z >> 16))));
	texIdx = aPos.y >> 6;
	if(enableAO)
		ao = float((aPos.y >> 5) & 1);
	else
		ao = 0;

	normal = getNormal();
	wh = vec2(aPos.z % 65536, aPos.z >> 16);
}