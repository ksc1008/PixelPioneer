#include "shaderLoader.h"

ShaderLoader ShaderLoader::Instance;

GLShader* ShaderLoader::getDefaultShader()
{
	return defaultShader;
}

void ShaderLoader::initialize()
{
	defaultShader = new GLShader("shaders/voxel_vertex.glsl", "shaders/voxel_fragment.glsl");
	initialized = true;
}
