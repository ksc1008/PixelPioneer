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
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (m_chunks[0][z + j][x + i] != nullptr)
				unloadChunk(x + i, 0, z + j);

			loadChunk(x + i, 0, z + j);

			for (int land_i = 0; land_i < Chunk::CHUNK_SIZE; land_i++) {
				for (int land_j = 0; land_j < Chunk::CHUNK_SIZE; land_j++) {
					for (int k = 0; k <= landscape[Chunk::CHUNK_SIZE*j + land_j][Chunk::CHUNK_SIZE * i + land_i]; k++) {
						if(k== landscape[Chunk::CHUNK_SIZE * j + land_j][Chunk::CHUNK_SIZE * i + land_i])
							m_chunks[0][z + j][x + i]->setBlock(3, land_i, k, land_j, true);
						else
							m_chunks[0][z + j][x + i]->setBlock(2, land_i, k, land_j, true);
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
			m_chunks[i][j] = new Chunk * [d];
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
}


void ChunkLoader::loadChunk(int x, int y, int z) {
	m_chunks[y][z][x] = new Chunk(x,y,z);
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
