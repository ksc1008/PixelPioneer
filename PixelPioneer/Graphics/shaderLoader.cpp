#include "shaderLoader.h"

ShaderLoader ShaderLoader::Instance;

GLShader* ShaderLoader::getDefaultShader()
{
	return defaultShader;
}

void ShaderLoader::initialize()
{
	defaultShader = new GLShader("shaders/vertex.glsl", "shaders/fragment.glsl");
	initialized = true;
}
