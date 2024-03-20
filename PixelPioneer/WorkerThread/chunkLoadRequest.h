#pragma once
#include "request.h"

class ChunkLoadRequest :public Request {
	class Chunk& loadingChunk;
	void loadChunk();
public:
	ChunkLoadRequest(Chunk& chunk):loadingChunk(chunk){}

	void execute() {
		loadChunk();
	}
};