#version 330 core

layout(location = 0) in ivec4 aPos;

out vec2 TexCoord;
flat out int texIdx;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x * 0.5,aPos.y * 0.5,aPos.z * 0.5, 1.0);
	//gl_Position = projection * view * vec4(aPos.x * 0.5,aPos.y * 0.5,aPos.z * 0.5, 1.0);
	TexCoord = vec2(float(aPos.w & 1), float(aPos.w & 2));
	texIdx = aPos.w / 4;
}