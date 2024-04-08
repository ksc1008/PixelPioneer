#include "chunkCreateRequest.h"
#include "../Voxel/chunk.h"
#include "../World/perlin.h"

void ChunkCreateRequest::execute()
{
	PerlinNoise p;
	auto noiseMap = p.GenerateMap3D(m_width,
		m_height, m_depth, creationOptions.scale, creationOptions.octaves, creationOptions.persistanace, creationOptions.lacunarity, x_offset, y_offset, z_offset, creationOptions.minHeight, creationOptions.scaleHeight);
}

ChunkCreateRequest::ChunkCreateRequest(int x, int y, int z, int width, int height, int depth, ChunkCreationOptions& options, class Chunk& chunk):creationOptions(options), updatingChunk(chunk) {
	x_offset = x;
	y_offset = y;
	z_offset = z;
	m_width = width;
	m_height = height;
	m_depth = depth;
}