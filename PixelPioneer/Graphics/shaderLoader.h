#pragma once
#include "shader.h"

class ShaderLoader
{
public:
	static ShaderLoader* getInstance() {
		if (!Instance.initialized)
			Instance.initialize();
		return &Instance;
	}

	void SetFlatMode(bool active);

	GLShader* getDefaultShader();

private:
	bool useFlatShader = false;

	void initialize();
	bool initialized = false;
	static ShaderLoader Instance;
	GLShader* defaultShader;
	GLShader* flatShader;
};

