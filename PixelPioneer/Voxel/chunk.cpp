#include "chunk.h"
#include "../Graphics/shaderLoader.h"
#include <glm/gtc/matrix_transform.hpp>


Chunk::Chunk(int x, int y, int z)
{
	m_pBlocks = new Block **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pBlocks[i] = new Block * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pBlocks[i][j] = new Block[CHUNK_SIZE];
		}
	}

	m_chunkX = x;
	m_chunkY = y;
	m_chunkZ = z;

	m_model = new VoxelModel();
}

Chunk::~Chunk() { 
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_pBlocks[i][j];
		}
		delete[] m_pBlocks[i];
	}
	delete[] m_pBlocks;
}

void Chunk::update(float dt)
{
	if (needRefresh) {
		createMesh();
		needRefresh = false;
	}
}

void Chunk::createMesh() {
	m_model->startBuild();
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			for (int k = 0; k < CHUNK_SIZE; ++k) {
				if (!m_pBlocks[i][j][k].isActive()) {
					continue;
				}
				for (int face = 0; face < 6; face++) {
					addFace(k, i, j, face);
				}
			}
		}
	}
	m_model->endBuild();
}

void Chunk::addFace(int x, int y, int z, int face)
{
	if (checkAdjacent(x, y, z, face))
		return;
	m_model->addQuad(x, y, z, m_pBlocks[y][z][x].getId(), face);
}

bool Chunk::checkAdjacent(int x, int y, int z, int face) {
	int nx = 0; int ny = 0;int nz = 0;
	switch (face) {
	case 0: {
		nx = x; ny = y - 1; nz = z;
		break;
	}
	case 1: {
		nx = x; ny = y + 1; nz = z;
		break;
	}
	case 2: {
		nx = x; ny = y; nz = z - 1;
		break;
	}
	case 3: {
		nx = x; ny = y; nz = z + 1;
		break;
	}
	case 4: {
		nx = x - 1; ny = y; nz = z;
		break;
	}
	default: {
		nx = x + 1; ny = y; nz = z;
		break; 
	}
	}
	if (nx < 0 || nx >= CHUNK_SIZE || ny < 0 || ny >= CHUNK_SIZE || nz < 0 || nz >= CHUNK_SIZE)
		return false;
	if (m_pBlocks[ny][nz][nx].isActive())
		return true;
	return false;
}

void Chunk::render() {
	ShaderLoader::getInstance()->getDefaultShader()->setModelTransform(glm::translate(glm::mat4(1.0f),
		glm::vec3(CHUNK_SIZE * m_chunkX, CHUNK_SIZE * m_chunkY, CHUNK_SIZE * m_chunkZ)));
	m_model->renderMesh();
}

void Chunk::bind() {
	m_model->bindVAO();
}

void Chunk::setBlock(int type, int x, int y, int z)
{
	m_pBlocks[y][z][x].setId(type);
}

void Chunk::setBlockEnabled(bool enabled, int x, int y, int z)
{
	m_pBlocks[y][z][x].setActive(enabled);
}
