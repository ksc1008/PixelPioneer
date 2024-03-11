#version 330 core

layout(location = 0) in ivec4 aPos;

out vec2 TexCoord;
out vec3 normal;
flat out int texIdx;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 getNormal(){
	float t = (aPos.w&1)*2-1;
	if( (aPos.w&4)>0)
		return vec3(t,0,0);	
	if( (aPos.w&2)>0)	
		return vec3(0,0,t);
	return vec3(0,t,0);		
}

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x * 0.33,aPos.y * 0.33,aPos.z * 0.33, 1.0);
	//gl_Position = projection * view * vec4(aPos.x * 0.5,aPos.y * 0.5,aPos.z * 0.5, 1.0);
	TexCoord = vec2(float( (aPos.w & 8) / 8), float( (aPos.w & 16) / 16));
	texIdx = aPos.w / 32;
	normal = getNormal();
}