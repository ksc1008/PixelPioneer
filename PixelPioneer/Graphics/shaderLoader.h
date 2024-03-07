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

	GLShader* getDefaultShader();

private:
	void initialize();
	bool initialized = false;
	static ShaderLoader Instance;
	GLShader* defaultShader;	
};

