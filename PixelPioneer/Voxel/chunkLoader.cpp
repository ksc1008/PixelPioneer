#include "chunkLoader.h"
#include "../World/world_generator.h"
#include "../debug.h"
#include "../Graphics/shaderLoader.h"
#include "../WorkerThread/chunkUpdateRequest.h"
#include "../WorkerThread/chunkLoadRequest.h"
#include "../WorkerThread/channel.h"

void ChunkLoader::generateChunks(int w, int h, int d)
{
	
}

void ChunkLoader::generateLargeChunk(int x, int y, int z, int n) {
	WorldGenerator gen;
	auto landscape = gen.createLandscape(n * Chunk::CHUNK_SIZE, 10);

	for (int i = 0; i < m_height - 1; i++) {
		for(int j = 0;j<m_width;j++){
			for (int k = 0; k < m_depth; k++) {
				for (int t1 = 0; t1 < Chunk::CHUNK_SIZE; t1++) {
					for (int t2 = 0; t2 < Chunk::CHUNK_SIZE; t2++) {
						for (int t3 = 0; t3 < Chunk::CHUNK_SIZE; t3++) {
							m_chunks[i][k][j]->setBlock(0, t1, t2, t3, true);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int land_i = 0; land_i < Chunk::CHUNK_SIZE; land_i++) {
				for (int land_j = 0; land_j < Chunk::CHUNK_SIZE; land_j++) {
					for (int k = 0; k <= landscape[Chunk::CHUNK_SIZE*j + land_j][Chunk::CHUNK_SIZE * i + land_i]; k++) {
						if(k== landscape[Chunk::CHUNK_SIZE * j + land_j][Chunk::CHUNK_SIZE * i + land_i])
							m_chunks[m_height - 1][z + j][x + i]->setBlock(3, land_i, k, land_j, true);
						else
							m_chunks[m_height - 1][z + j][x + i]->setBlock(2, land_i, k, land_j, true);
					}
				}
			}
		}
	}
}



void ChunkLoader::setWorldSize(int w, int h, int d)
{
	m_chunks = new Chunk ***[h];
	loaded = new bool**[h];
	for (int i = 0; i < h; i++) {
		m_chunks[i] = new Chunk * *[d];
		loaded[i] = new bool* [d];
		for (int j = 0; j < d; j++) {
			m_chunks[i][j] = new Chunk * [w];
			loaded[i][j] = new bool[w];
			for (int k = 0; k < w; k++) {
				m_chunks[i][j][k] = nullptr;
				loaded[i][j][k] = false;
			}
		}
	}
	m_height = h;
	m_width = w;
	m_depth = d;

	initiateChunks();
}


void ChunkLoader::loadChunk(int x, int y, int z) {
	m_chunks[y][z][x] = new Chunk(x,y,z,x - m_width/2,y,z - m_depth/2, *this);
	loaded[y][z][x] = true;
}

void ChunkLoader::updateChunk(int x, int y, int z)
{

}

void ChunkLoader::renderChunks(GLenum renderFace, GLenum renderMode)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(renderFace, renderMode);
	ShaderLoader::getInstance()->getDefaultShader()->setBool("enableAO", m_ao_enabled);

	fpsCounter.startStopwatch();
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_depth; j++) {
			for (int k = 0; k < m_width; k++) {
				if (loaded[i][j][k]) {					
					m_chunks[i][j][k]->render();
				}
			}
		}
	}
	fpsCounter.endStopwatch();
}

void ChunkLoader::unloadChunk(int x, int y, int z)
{
	delete m_chunks[y][z][x];
	m_chunks[y][z][x] = nullptr;
	loaded[y][z][x] = false;
}

void ChunkLoader::initiateChunks()
{
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_depth; j++) {
			for (int k = 0; k < m_width; k++) {
				if (m_chunks[i][j][k] == nullptr)
					loadChunk(k, i, j);
			}
		}
	}
}

void ChunkLoader::updateChunks()
{
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_depth; j++) {
			for (int k = 0; k < m_width; k++) {
				if (loaded[i][j][k]) {
					auto rq = new ChunkUpdateRequest(*m_chunks[i][j][k],true);
					Channel::GetChunkLoadChannel().executeRequest(rq);
				}
			}
		}
	}
}

unsigned long long ChunkLoader::getAdjacentExistanceBitmask(int x, int y, int z, int face, int i, int j)
{
	x %= m_width;
	z %= m_depth;
	if (y < 0 || y >= m_height)
		return 0u;
	if (x < 0) {
		x = m_width - 1;
	}
	if (z < 0) {
		z = m_depth - 1;
	}

	return m_chunks[y][z][x]->getExsistanceBitmask(face, i, j);
}

void ChunkLoader::updateAdjacentChunkExistanceBitmask(int chunkX, int chunkY, int chunkZ, int face, int i, int j, bool enabled, bool pendUpdate)
{
	int compX = (chunkX + Chunk::faceOffsets[face][0])%m_width;
	int compY = chunkY + Chunk::faceOffsets[face][1];
	int compZ = (chunkZ + Chunk::faceOffsets[face][2])%m_depth;

	if (compY < 0 || compY >= m_height)
		return;
	if (compX < 0) {
		compX = m_width - 1;
	}
	if (compZ < 0) {
		compZ = m_depth - 1;
	}
	m_chunks[compY][compZ][compX]->setBorderBlockEnabled(enabled, face, i, j, pendUpdate);
}

void ChunkLoader::loadChunks() {
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_depth; j++) {
			for (int k = 0; k < m_width; k++) {
				if (loaded[i][j][k]) {
					auto rq = new ChunkLoadRequest(*m_chunks[i][j][k]);
					Channel::GetChunkLoadChannel().executeRequest(rq);
				}
			}
		}
	}
}

void ChunkLoader::switchMeshUpdateMode() {
	fast = !fast;

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_depth; j++) {
			for (int k = 0; k < m_width; k++) {
				if (loaded[i][j][k]) {
					m_chunks[i][j][k]->SetRenderMode(fast ? FAST : OPTIMAL);
				}
			}
		}
	}

	updateChunks();
}

void ChunkLoader::setAO(bool enabled)
{
	m_ao_enabled = enabled;

	updateChunks();
}
