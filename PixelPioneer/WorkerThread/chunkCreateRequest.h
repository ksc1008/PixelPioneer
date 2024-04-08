#pragma once
#include "request.h"

class Chunk;

struct ChunkCreationOptions {
public:
	int seed, scale, scaleHeight, minHeight, octaves, persistanace, lacunarity;
};

class ChunkCreateRequest :public Request {
	ChunkCreationOptions& creationOptions;
	Chunk& updatingChunk;
	float x_offset, y_offset, z_offset;
	int m_width, m_height, m_depth;

public:
	ChunkCreateRequest(int x, int y, int z, int width, int height, int depth, ChunkCreationOptions& options, class Chunk& chunk);
	void execute();
};