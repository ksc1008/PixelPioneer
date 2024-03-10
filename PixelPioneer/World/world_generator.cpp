#include "world_generator.h"
#include "../noiseutils.h"

void WorldGenerator::generateGrass(Chunk& chunk)
{
	module::Perlin noiseModule;
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	heightMapBuilder.SetSourceModule(noiseModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(64, 64);
}
