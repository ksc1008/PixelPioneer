#include "chunkUpdateRequest.h"
#include "../Voxel/chunk.h"


void ChunkUpdateRequest::updateChunk()
{
	updatingChunk.update(0);
}
