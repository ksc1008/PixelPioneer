#include "chunk.h"
#include "../Graphics/shaderLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../debug.h"
#include"tempStorage.h"

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

	delete m_model;
}

void Chunk::update(float dt)
{
	if (needRefresh) {
		if (m_rendermode == OPTIMAL)
			createGreedyMesh();
		else
			createFastMesh();
		needRefresh = false;
	}
}

void Chunk::createFastMesh() {
	m_model->startBuild();
	auto ao = TemporaryStorage::getInstance()->ao;
	if (m_createAO) {
		for (int i = 0; i < CHUNK_SIZE; ++i) {
			for (int j = 0; j < CHUNK_SIZE; ++j) {
				for (int k = 0; k < CHUNK_SIZE; ++k) {
					if (!m_pBlocks[i][j][k].isActive()) {
						continue;
					}
					for (int face = 0; face < 6; face++) {
						ao[face][i][j][k] = checkCorner(k, i, j, face);						
						addFace(k, i, j, face);
					}
				}
			}
		}
	}
	else {
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
	}
	m_model->endBuild();
}

void Chunk::createGreedyMesh() {
	TemporaryStorage::getInstance()->initiateFaceTypes();
	auto faceType = TemporaryStorage::getInstance()->faceTypes;
	auto ao = TemporaryStorage::getInstance()->ao;
	if (m_createAO) {
		for (int i = 0; i < CHUNK_SIZE; ++i) {
			for (int j = 0; j < CHUNK_SIZE; ++j) {
				for (int k = 0; k < CHUNK_SIZE; ++k) {
					for (int face = 0; face < 6; face++) {
						if (!m_pBlocks[i][j][k].isActive() || checkAdjacent(k, i, j, face))
							faceType[face][i][j][k] = -1;
						else {
							faceType[face][i][j][k] = m_pBlocks[i][j][k].getId();
							ao[face][i][j][k] = checkCorner(k, i, j, face);
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < CHUNK_SIZE; ++i) {
			for (int j = 0; j < CHUNK_SIZE; ++j) {
				for (int k = 0; k < CHUNK_SIZE; ++k) {
					for (int face = 0; face < 6; face++) {
						if (!m_pBlocks[i][j][k].isActive() || checkAdjacent(k, i, j, face))
							faceType[face][i][j][k] = -1;
						else {
							faceType[face][i][j][k] = m_pBlocks[i][j][k].getId();
						}
					}
				}
			}
		}
	}

	int gridX, gridY, subgridX, subgridY, width, height;
	int meshingType = 0;
	unsigned char meshingAO = 0;
	bool done = false;

	m_model->startBuild();

	for (int f = 0; f < 6; f++) {
		switch (f / 2) {
		case 0: {
			for (int i = 0; i < CHUNK_SIZE; i++) {
				for (int x = 0; x < CHUNK_SIZE; x++) {
					for (int y = 0; y < CHUNK_SIZE;) {
						gridX = x; gridY = y;

						if (faceType[f][i][y][x] == -1) {
							y++;
							continue;
						}

						meshingType = faceType[f][i][y][x];
						meshingAO = ao[f][i][y][x];
						height = 0;

						for (int dy = gridY; dy < CHUNK_SIZE; dy++) {
							subgridX = gridX; subgridY = dy;
							if (faceType[f][i][subgridY][subgridX] != meshingType || ao[f][i][subgridY][subgridX] != meshingAO)
								break;
							height++;
						}

						width = 1;
						done = false;
						for (int dx = gridX + 1; dx < CHUNK_SIZE; dx++) {
							for (int dy = gridY; dy < gridY + height && dy < CHUNK_SIZE; dy++) {
								subgridX = dx; subgridY = dy;
								if (faceType[f][i][dy][dx] != meshingType || ao[f][i][dy][dx] != meshingAO) {
									done = true;
									break;
								}
							}
							if (done)
								break;
							width++;
						}

						for (int dx = gridX; dx < gridX + width; dx++) {
							for (int dy = gridY; dy < gridY + height; dy++) {
								faceType[f][i][dy][dx] = -1;
							}
						}

						m_model->addQuad(gridX, i, gridY, meshingType, f, width, height, meshingAO);
					}
				}
			}
			break;
		}
		case 1: {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				for (int x = 0; x < CHUNK_SIZE; x++) {
					for (int y = 0; y < CHUNK_SIZE;) {
						gridX = x; gridY = y;

						if (faceType[f][x][j][y] == -1) {
							y++;
							continue;
						}

						meshingType = faceType[f][x][j][y];
						meshingAO = ao[f][x][j][y];
						height = 0;

						for (int dy = gridY; dy < CHUNK_SIZE; dy++) {
							subgridX = gridX; subgridY = dy;
							if (faceType[f][subgridX][j][subgridY] != meshingType || ao[f][subgridX][j][subgridY] != meshingAO)
								break;
							height++;
						}

						width = 1;
						done = false;
						for (int dx = gridX + 1; dx < CHUNK_SIZE; dx++) {
							for (int dy = gridY; dy < gridY + height && dy < CHUNK_SIZE; dy++) {
								subgridX = dx; subgridY = dy;
								if (faceType[f][dx][j][dy] != meshingType || ao[f][dx][j][dy] != meshingAO) {
									done = true;
									break;
								}
							}
							if (done)
								break;
							width++;
						}

						for (int dx = gridX; dx < gridX + width; dx++) {
							for (int dy = gridY; dy < gridY + height; dy++) {
								faceType[f][dx][j][dy] = -1;
							}
						}

						m_model->addQuad(gridY, gridX, j, meshingType, f, width, height, meshingAO);
					}
				}
			}
			break;
		}
		case 2: {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				for (int x = 0; x < CHUNK_SIZE; x++) {
					for (int y = 0; y < CHUNK_SIZE;) {
						gridX = x; gridY = y;

						if (faceType[f][x][y][k] == -1) {
							y++;
							continue;
						}

						meshingType = faceType[f][x][y][k];
						meshingAO = ao[f][x][y][k];
						height = 0;

						for (int dy = gridY; dy < CHUNK_SIZE; dy++) {
							subgridX = gridX; subgridY = dy;
							if (faceType[f][subgridX][subgridY][k] != meshingType || ao[f][subgridX][subgridY][k] != meshingAO)
								break;
							height++;
						}

						width = 1;
						done = false;
						for (int dx = gridX + 1; dx < CHUNK_SIZE; dx++) {
							for (int dy = gridY; dy < gridY + height && dy < CHUNK_SIZE; dy++) {
								subgridX = dx; subgridY = dy;
								if (faceType[f][dx][dy][k] != meshingType || ao[f][dx][dy][k] != meshingAO) {
									done = true;
									break;
								}
							}
							if (done)
								break;
							width++;
						}

						for (int dx = gridX; dx < gridX + width; dx++) {
							for (int dy = gridY; dy < gridY + height; dy++) {
								faceType[f][dx][dy][k] = -1;
							}
						}

						m_model->addQuad(k, gridX, gridY, meshingType, f, width, height, meshingAO);
					}
				}
			}
			break;
		}
		default: {
			break;
		}
		}
	}

	m_model->endBuild();
}

void Chunk::addFace(int x, int y, int z, int face)
{
	if (checkAdjacent(x, y, z, face))
		return;
	m_model->addQuad(x, y, z, m_pBlocks[y][z][x].getId(), face,1,1);
}

/*
	check corner : 한 블럭의 해당 면과 인접한 블럭이 존재하는지 체크.
	해당 면과 수직인 축에 대해 면이 바라보는 방향으로 1 만큼 앞에 있으면서, 평행한 축에 대해 해당 면과 맨허튼 거리가 1~2인 블럭을 체크
	
	return : 면의 첫 정점부터 반시계 방향으로 돌아가는 각 4 정점에 대한 ao값을 리턴, 
	끝자리 비트부터 각 정점별 2비트 씩 (0~2), 8비트에 대해 ao 값이 할당.
	
	ao value : 0 = 인접한 블럭 없음,  1 = 대각선 방향에 인접한 블럭 존재, 2 = 정점을 포함하는 한 모서리에 인접한 블럭 존재.
*/
unsigned char Chunk::checkCorner(int x, int y, int z, int face)
{
	bool occludeLine[4] = {false};
	bool occludePoint[4] = { false };
	int dx1, dy1, dz1, dx2, dy2, dz2, nx, ny, nz, ax, ay, az;

	switch (face) {
	case 0: {
		nx = x; ny = y - 1; nz = z;
		dx1 = 0; dy1 = 0; dz1 = -1;
		dx2 = 1; dy2 = 0; dz2 = 0;
		break;
	}
	case 1: {
		nx = x; ny = y + 1; nz = z;
		dx1 = -1; dy1 = 0; dz1 = 0;
		dx2 = 0; dy2 = 0; dz2 = 1;
		break;
	}
	case 2: {
		nx = x; ny = y; nz = z - 1;
		dx1 = -1; dy1 = 0; dz1 = 0;
		dx2 = 0; dy2 = 1; dz2 = 0;
		break;
	}
	case 3: {
		nx = x; ny = y; nz = z + 1;
		dx1 = 0; dy1 = -1; dz1 = 0;
		dx2 = 1; dy2 = 0; dz2 = 0;
		break;
	}
	case 4: {
		nx = x - 1; ny = y; nz = z;
		dx1 = 0; dy1 = -1; dz1 = 0;
		dx2 = 0; dy2 = 0; dz2 = 1;
		break;
	}
	default: {
		nx = x + 1; ny = y; nz = z;
		dx1 = 0; dy1 = 0; dz1 = -1;
		dx2 = 0; dy2 = 1; dz2 = 0;
		break;
	}
	}

	if (nx < 0 || nx >= CHUNK_SIZE || ny < 0 || ny >= CHUNK_SIZE || nz < 0 || nz >= CHUNK_SIZE)
		return 0;
	
	int t1, t2;

	int bit = -1;
	for (int i = 1; i >= -1; i-=2) {
		for (int j = 1; j <= 2; j++) {
			bit++;
			t1 = j % 2 * i;
			t2 = j / 2 * i;
			
			ax = t1 * dx1 + t2 * dx2 + nx;
			ay = t1 * dy1 + t2 * dy2 + ny;
			az = t1 * dz1 + t2 * dz2 + nz;

			if (ax < 0 || ax >= CHUNK_SIZE || ay < 0 || ay >= CHUNK_SIZE || az < 0 || az >= CHUNK_SIZE)
				continue;
			if (m_pBlocks[ay][az][ax].isActive())
				occludeLine[bit] = true;
		}
	}

	bit = -1;
	for (int i = 1; i >= -1; i -= 2) {
		for (int j = -1; j <= 1; j += 2) {
			bit++;
			ax = i * dx1 + i * j * dx2 + nx;
			ay = i * dy1 + i * j * dy2 + ny;
			az = i * dz1 + i * j * dz2 + nz;

			if (ax < 0 || ax >= CHUNK_SIZE || ay < 0 || ay >= CHUNK_SIZE || az < 0 || az >= CHUNK_SIZE)
				continue;
			if (m_pBlocks[ay][az][ax].isActive())
				occludePoint[bit] = true;
		}
	}

	unsigned char result = 0;
	for (int i = 3; i >= 0; i--) {
		result = result << 2;
		if (occludeLine[i]|| occludeLine[(i+3)%4])
			result += 2;
		else if (occludePoint[i])
			result += 1;
	}
	//if (result > 0) {
	//	Debugger::getInstance()->writeLine(x, ", ", y, ", ", z);
	//}
	return result;
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
	bind();
	m_model->renderMesh();
}

void Chunk::bind() {
	m_model->bindVAO();
	ShaderLoader::getInstance()->getDefaultShader()->setModelTransform(glm::translate(glm::mat4(1.0f),
		glm::vec3(CHUNK_SIZE * m_chunkX, CHUNK_SIZE * m_chunkY, CHUNK_SIZE * m_chunkZ) * 0.66f));
}

int Chunk::getPolygonNumber()
{
	return m_model->getNumberOfPolygons();
}

void Chunk::SetRenderMode(RenderMode mode)
{
	if (mode != m_rendermode) {
		m_rendermode = mode;
		needRefresh = true;
	}
}

void Chunk::setBlock(int type, int x, int y, int z)
{
	m_pBlocks[y][z][x].setId(type);
}

void Chunk::setBlockEnabled(bool enabled, int x, int y, int z)
{
	m_pBlocks[y][z][x].setActive(enabled);
}
