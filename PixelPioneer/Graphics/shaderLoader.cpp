#include "shaderLoader.h"

ShaderLoader ShaderLoader::Instance;

void ShaderLoader::SetFlatMode(bool active)
{
	useFlatShader = active;
}

GLShader* ShaderLoader::getDefaultShader()
{
	if (useFlatShader)
		return flatShader;
	return defaultShader;
}


void ShaderLoader::initialize()
{
	defaultShader = new GLShader("shaders/voxel_vertex.glsl", "shaders/voxel_fragment.glsl");
	flatShader = new GLShader("shaders/voxel_vertex_origin.glsl", "shaders/voxel_fragment.glsl");

	initialized = true;
}
