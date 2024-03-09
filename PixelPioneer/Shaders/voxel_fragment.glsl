#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
flat in int texIdx;

// texture samplers
uniform sampler2DArray texArray;

void main()
{
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	FragColor = texture(texArray, vec3(TexCoord.xy,texIdx));
}