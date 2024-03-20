#include "chunkLoadRequest.h"
#include "../Voxel/chunk.h"

void ChunkLoadRequest::loadChunk()
{
	loadingChunk.updateAllMasks();
	loadingChunk.update(0);
}
