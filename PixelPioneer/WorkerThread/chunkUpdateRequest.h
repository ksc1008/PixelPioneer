#pragma once
#include "request.h"
#include <iostream>

class Chunk;


class ChunkUpdateRequest :public Request {
	class Chunk& updatingChunk;
	void updateChunk();

public:
	ChunkUpdateRequest(Chunk& chunk, bool meshing) :updatingChunk(chunk){		
	}

	void execute() {
		updateChunk();
		std::cout << "Chunk update end\n";
	}

private:
};