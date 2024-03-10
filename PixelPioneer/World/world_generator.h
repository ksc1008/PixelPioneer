#pragma once
#include <noise/noise.h>
#include "../Voxel/chunk.h"

using namespace noise;


class WorldGenerator {
public:
	void generateGrass(Chunk& chunk);
};