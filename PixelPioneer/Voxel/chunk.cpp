#include "chunk.h"
#include "chunkLoader.h"
#include "../Graphics/shaderLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../debug.h"
#include"tempStorage.h"

// #define ENABLE_MESHING

const short Chunk::faceOffsets[][3] = {{0,-1,0},{0,1,0},{0,0,-1},{0,0,1},{-1,0,0},{1,0,0}};
const short Chunk::xPerFace[][3] = { {0,0,1},{0,0,1},{0,1,0},{0,1,0},{1,0,0},{1,0,0} };
const short Chunk::yPerFace[][3] = { {1,0,0},{1,0,0},{0,0,1},{0,0,1},{0,1,0},{0,1,0} };
const short Chunk::zPerFace[][3] = { {0,1,0},{0,1,0},{1,0,0},{1,0,0},{0,0,1},{0,0,1} };

Chunk::Chunk(int x, int y, int z, int posX, int posY, int posZ, ChunkLoader& parent) :_parentLoader(parent)
{
	m_pBlocks = new Block **[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		m_pBlocks[i] = new Block * [CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_pBlocks[i][j] = new Block[CHUNK_SIZE];
		}
	}

	int c;
	for (int i = 0; i < 6; i++) {
		m_existance_bitmask[i] = new bitType *[CHUNK_SIZE];
		m_block_cull_bitmask[i] = new bitType *[CHUNK_SIZE];
#ifdef ENABLE_MESHING
		m_horizontal_merge[i] = new unsigned char** [CHUNK_SIZE];
#endif
		for(c = 0;c<4;c++)
			m_ao_bitmask[c][i] = new bitType *[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; j++) {
			m_existance_bitmask[i][j] = new bitType[CHUNK_SIZE];
			m_block_cull_bitmask[i][j] = new bitType[CHUNK_SIZE];
#ifdef ENABLE_MESHING
			m_horizontal_merge[i][j] = new unsigned char* [CHUNK_SIZE];
#endif
			for (c = 0; c < 4; c++) {
				m_ao_bitmask[c][i][j] = new bitType[CHUNK_SIZE];
				memset(m_ao_bitmask[c][i][j], 0, sizeof(bitType) * CHUNK_SIZE);
			}
			memset(m_existance_bitmask[i][j], 0, sizeof(bitType) * CHUNK_SIZE);
			memset(m_block_cull_bitmask[i][j], 0, sizeof(bitType) * CHUNK_SIZE);
			for (int k = 0; k < CHUNK_SIZE; k++) {
#ifdef ENABLE_MESHING
				m_horizontal_merge[i][j][k] = new unsigned char[CHUNK_SIZE];
				for (int t = 0; t < CHUNK_SIZE; t++) {
					m_horizontal_merge[i][j][k][t] = CHUNK_SIZE - t;
			}
#endif // ENABLE_MESHING
			}
		}
	}

	m_posX = posX;
	m_posY = posY;
	m_posZ = posZ;

	m_X = x;
	m_Y = y;
	m_Z = z;
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

void Chunk::update(float dt, bool createAO)
{
	if (getRenderState() == UPDATING)
		return;
	setRenderState(UPDATING);
	if (m_rendermode == OPTIMAL)
		createGreedyMesh(createAO);
	else
		createFastMesh(createAO);
	needRefreshMesh = false;
	m_ao_built = createAO;
	setRenderState(ENDUPDATE);
}

void Chunk::createFastMesh(bool createAO) {
	m_model->startBuild();
	int x, y, z;
	bitType ao0, ao1, ao2, ao3;

	for (int face = 0; face < 6; face++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				bitType mask = 1;

				for (int depth = 0; depth < CHUNK_SIZE; depth++) {
					if (m_block_cull_bitmask[face][i][j] & mask) {
						x = getX(face, depth, i, j); y = getY(face, depth, i, j); z = getZ(face, depth, i, j);
						m_model->addQuad(x, y, z, m_pBlocks[y][z][x].getBlockTex(face), face, 1, 1, getAOBitmask(face,i,j,depth));
					}
					mask <<= 1;
				}
			}
		}
	}
}

void Chunk::finishUpdate()
{
	m_model->endBuild();
	setRenderState(UPTODATE);
}

void Chunk::createGreedyMesh(bool createAO) {

	m_model->startBuild();
	unsigned short baked_mesh_widths[CHUNK_SIZE] = { 0u };

	for (int f = 0; f < 6; f++) {
		for (int depth = 0; depth < CHUNK_SIZE; depth++) {
			unsigned short min_width_j = 0;
			unsigned short min_width_i = 0;
			for (int t = 0; t < CHUNK_SIZE; t++) {
				baked_mesh_widths[t] = 0;
			}
			do {
				int height = 1;
				int i = min_width_i;
				const int j = min_width_j;
#ifdef ENABLE_MESHING
				const int width = m_horizontal_merge[f][depth][i][j];
#else
				const int width = 1;
#endif // ENABLE_MESHING

				const int x = getX(f, depth, i, j);
				const int y = getY(f, depth, i, j);
				const int z = getZ(f, depth, i, j);
				const int mergeType = (((m_block_cull_bitmask[f][i][j] >> depth) & 1) == 0)?-1:m_pBlocks[y][z][x].getBlockTex(f);
				const unsigned int mergeAO = getAOBitmask(f, i, j, depth);

				i++;
				if (mergeType == -1) {
					for (; i < CHUNK_SIZE; i++) {
						if ((((m_block_cull_bitmask[f][i][j] >> depth) & 1) > 0)
#ifdef ENABLE_MESHING
							|| m_horizontal_merge[f][depth][i][j] != width
#endif
							|| baked_mesh_widths[i] != j) {
							break;
						}
						height++;
					}
				}
				else {
					int dx = x;
					int dy = y;
					int dz = z;
					for (; i<CHUNK_SIZE; i++) {
						dx += xPerFace[f][1];
						dy += yPerFace[f][1];
						dz += zPerFace[f][1];

						if ((((getAOBitmask(f, i, j, depth) != mergeAO
#ifdef ENABLE_MESHING
							|| m_horizontal_merge[f][depth][i][j] != width
#endif
							|| m_pBlocks[dy][dz][dx].getBlockTex(f) != mergeType
							|| m_block_cull_bitmask[f][i][j] >> depth) & 1) == 0)
							|| baked_mesh_widths[i] != j) {
							break;
						}
						height++;
					}
					m_model->addQuad(x, y, z, mergeType, f, height, width, mergeAO);
				}

				for (int t = 0; t < height; t++) {
					baked_mesh_widths[min_width_i + t] += width;
				}
				for (int t = 0; t < CHUNK_SIZE; t++) {
					if (baked_mesh_widths[t] < baked_mesh_widths[min_width_i])
						min_width_i = t;
				}
				min_width_j = baked_mesh_widths[min_width_i];
			} while (min_width_j < CHUNK_SIZE);
		}
	}
}


void Chunk::render() {
	auto state = getRenderState();
	if (state == NOTLOADED)
		return;
	if (state == ENDUPDATE) {
		bind();
		m_model->renderMesh();
		finishUpdate();
	}
	else if (state == UPDATING || state == UPTODATE) {
		bind();
		m_model->renderMesh();
	}
}

void Chunk::bind() {
	m_model->bindVAO();
	//ShaderLoader::getInstance()->getDefaultShader()->setModelTransform(glm::translate(glm::mat4(1.0f),
	//	glm::vec3(CHUNK_SIZE * m_chunkX, CHUNK_SIZE * m_chunkY, CHUNK_SIZE * m_chunkZ) * 0.66f));
	
	ShaderLoader::getInstance()->getDefaultShader()->setFloat("land_y", CHUNK_SIZE * (m_posY));
	ShaderLoader::getInstance()->getDefaultShader()->setModelTransform(glm::translate(glm::mat4(1.0f),
		glm::vec3(CHUNK_SIZE * m_posX, CHUNK_SIZE * m_posY, CHUNK_SIZE * m_posZ)));
}

void Chunk::updateAllMasks()
{
	for (int f = 0; f < 6; f++) {
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int j = 0; j < CHUNK_SIZE; j++) {
				updateCullingMask(f, i, j);
				updateAOMask(f, i, j);

#ifdef ENABLE_MESHING
				for(int d = 0;d<CHUNK_SIZE;d++)
					updateHorizontalMerge(f, d, i, j);
#endif
			}
		}
	}
}

void Chunk::setRenderState(RenderState state)
{
	meshStateMutex.lock();
	m_currentRenderState = state;
	meshStateMutex.unlock();
}

RenderState Chunk::getRenderState()
{

	meshStateMutex.lock();
	RenderState result = m_currentRenderState;
	meshStateMutex.unlock();
	return result;
}

int Chunk::getPolygonNumber()
{
	return m_model->getNumberOfPolygons();
}

void Chunk::SetRenderMode(RenderMode mode)
{
	if (mode != m_rendermode) {
		m_rendermode = mode;
		needRefreshMesh = true;
	}
}

const bitType Chunk::getExsistanceBitmask(int face, int i, int j) {

	if (i < 0) {
		int x = m_X - xPerFace[face][1];
		int y = m_Y - yPerFace[face][1];
		int z = m_Z - zPerFace[face][1];

		return _parentLoader.getAdjacentExistanceBitmask(x, y, z, face, CHUNK_SIZE - 1, j);
	}
	if (i >= CHUNK_SIZE) {
		int x = m_X + xPerFace[face][1];
		int y = m_Y + yPerFace[face][1];
		int z = m_Z + zPerFace[face][1];

		return _parentLoader.getAdjacentExistanceBitmask(x, y, z, face, 0, j);
	}
	if (j < 0) {
		int x = m_X - xPerFace[face][2];
		int y = m_Y - yPerFace[face][2];
		int z = m_Z - zPerFace[face][2];

		return _parentLoader.getAdjacentExistanceBitmask(x, y, z, face, i, CHUNK_SIZE - 1);
	}
	if (j >= CHUNK_SIZE) {
		int x = m_X + xPerFace[face][2];
		int y = m_Y + yPerFace[face][2];
		int z = m_Z + zPerFace[face][2];

		return _parentLoader.getAdjacentExistanceBitmask(x, y, z, face, i, 0);
	}

	return m_existance_bitmask[face][i][j];
}

void Chunk::setBorderBlockEnabled(bool enabled, int face, int i, int j, bool pendUpdate)
{
	bitType bit;

	if (face % 2 == 0)
		bit = (bitType)1 << (CHUNK_SIZE + 1);
	else
		bit = (bitType)1;

	m_existance_bitmask[face][i][j] |= bit;
	if (!enabled)
		m_existance_bitmask[face][i][j] ^= bit;

	if (!pendUpdate) {
		updateCullingMask(face, i, j);
	}
}

void Chunk::setBlock(int type, int x, int y, int z, bool pendUpdate)
{
	needRefreshMesh = true;

	m_pBlocks[y][z][x].setId(type);
	updateExsistanceBitmask(x, y, z, true, pendUpdate);

	if (!pendUpdate) {
		updateAdjacentCullingMask(x,y,z);
		updateAdjacentAO(x, y, z); 
#ifdef ENABLE_MESHING
		updateAdjacentHorizontalMerge(x, y, z);
#endif
	}
	else {
		needRefreshMask = true;
	}
}

void Chunk::updateExsistanceBitmask(int x, int y, int z, bool enabled, bool pendUpdate)
{
	m_existance_bitmask[0][z][x] |= (bitType)1 << (y + 1);
	m_existance_bitmask[1][z][x] |= (bitType)1 << (y + 1);
	m_existance_bitmask[2][x][y] |= (bitType)1 << (z + 1);
	m_existance_bitmask[3][x][y] |= (bitType)1 << (z + 1);
	m_existance_bitmask[4][y][z] |= (bitType)1 << (x + 1);
	m_existance_bitmask[5][y][z] |= (bitType)1 << (x + 1);

	if (!enabled) {
		m_existance_bitmask[0][z][x] ^= (bitType)1 << (y + 1);
		m_existance_bitmask[1][z][x] ^= (bitType)1 << (y + 1);
		m_existance_bitmask[2][x][y] ^= (bitType)1 << (z + 1);
		m_existance_bitmask[3][x][y] ^= (bitType)1 << (z + 1);
		m_existance_bitmask[4][y][z] ^= (bitType)1 << (x + 1);
		m_existance_bitmask[5][y][z] ^= (bitType)1 << (x + 1);
	}

	if (x == 0) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 4, y, z, enabled, pendUpdate);
	}
	else if (x == CHUNK_SIZE - 1) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 5, y, z, enabled, pendUpdate);
	}
	if (y == 0) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 0, z, x, enabled, pendUpdate);
	}
	else if (y == CHUNK_SIZE - 1) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 1, z, x, enabled, pendUpdate);
	}
	if (z == 0) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 2, x, y, enabled, pendUpdate);
	}
	else if (z == CHUNK_SIZE - 1) {
		_parentLoader.updateAdjacentChunkExistanceBitmask(m_X, m_Y, m_Z, 3, x, y, enabled, pendUpdate);
	}
}

void Chunk::updateCullingMask(int face, int i, int j)
{
	bitType left_shift, right_shift;
	left_shift = m_existance_bitmask[face][i][j];
	if (face % 2 == 0) {
		right_shift = (m_existance_bitmask[face + 1][i][j]<<1);
	}
	else {
		right_shift = (m_existance_bitmask[face - 1][i][j]>>1);
	}
	m_block_cull_bitmask[face][i][j] = (left_shift & (left_shift ^ right_shift))>>1;
}


void Chunk::updateAdjacentCullingMask(int x, int y, int z)
{
	updateCullingMask(0, z, x);
	updateCullingMask(1, z, x);
	updateCullingMask(2, x, y);
	updateCullingMask(3, x, y);
	updateCullingMask(4, y, z);
	updateCullingMask(5, y, z);
}

void Chunk::updateAdjacentHorizontalMerge(int x, int y, int z)
{
	return;
	updateHorizontalMerge(0, y, z, x);
	updateHorizontalMerge(1, y, z, x);
	updateHorizontalMerge(2, z, x, y);
	updateHorizontalMerge(3, z, x, y);
	updateHorizontalMerge(4, x, y, z);
	updateHorizontalMerge(5, x, y, z);

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			for (int t = -1; t <= 1; t +=2) {
				updateHorizontalMerge(0, y + t, z + i, x + j);
				updateHorizontalMerge(1, y + t, z + i, x + j);
				updateHorizontalMerge(2, z + t, x + i, y + j);
				updateHorizontalMerge(3, z + t, x + i, y + j);
				updateHorizontalMerge(4, x + t, y + i, z + j);
				updateHorizontalMerge(5, x + t, y + i, z + j);
			}
		}
	}
}

void Chunk::updateAdjacentAO(int x, int y, int z)
{
	for (int i = -1; i <= 1 ; i++) {
		for (int j = -1; j <= 1; j++) {
			updateAOMask(0, z + i, x + j);
			updateAOMask(1, z + i, x + j);
			updateAOMask(2, x + i, y + j);
			updateAOMask(3, x + i, y + j);
			updateAOMask(4, y + i, z + j);
			updateAOMask(5, y + i, z + j);
		}
	}
}


void Chunk::updateAOMask(int face, int i, int j)
{
	if (i < 0 || i >= CHUNK_SIZE || j < 0 || j >= CHUNK_SIZE) {
		return;
	}
	bitType a0,a1,a2,a3;
	bitType c0,c1,c2,c3;
	bitType r0,r1,r2,r3;

	if (face % 2 == 0) {
		a0 = getExsistanceBitmask(face, i - 1, j) << 1;
		a1 = getExsistanceBitmask(face, i, j + 1) << 1;
		a2 = getExsistanceBitmask(face, i + 1, j) << 1;
		a3 = getExsistanceBitmask(face, i, j - 1) << 1;

		c0 = getExsistanceBitmask(face, i - 1, j - 1) << 1;
		c1 = getExsistanceBitmask(face, i - 1, j + 1) << 1;
		c2 = getExsistanceBitmask(face, i + 1, j + 1) << 1;
		c3 = getExsistanceBitmask(face, i + 1, j - 1) << 1;
	}
	else {
		a0 = getExsistanceBitmask(face, i, j - 1) >> 1;
		a1 = getExsistanceBitmask(face, i + 1, j) >> 1;
		a2 = getExsistanceBitmask(face, i, j + 1) >> 1;
		a3 = getExsistanceBitmask(face, i - 1, j) >> 1;

		c0 = getExsistanceBitmask(face, i - 1, j - 1) >> 1;
		c1 = getExsistanceBitmask(face, i + 1, j - 1) >> 1;
		c2 = getExsistanceBitmask(face, i + 1, j + 1) >> 1;
		c3 = getExsistanceBitmask(face, i - 1, j + 1) >> 1;
	}

	r0 = (a3 | a0 | c0)>>1;
	r1 = (a0 | a1 | c1)>>1;
	r2 = (a1 | a2 | c2)>>1;
	r3 = (a2 | a3 | c3)>>1;
	for (int x = 0; x < 4; x++) {
		m_ao_bitmask[x][face][i][j] = 0u;
		m_ao_bitmask[x][face][i][j] = 0u;
		m_ao_bitmask[x][face][i][j] = 0u;
		m_ao_bitmask[x][face][i][j] = 0u;
		for (int y = 0; y < CHUNK_SIZE/4; y++) {
			auto bit0 = (r0 >> (y + 4*x)) & 1u;
			auto bit1 = (r1 >> (y + 4*x)) & 1u;
			auto bit2 = (r2 >> (y + 4*x)) & 1u;
			auto bit3 = (r3 >> (y + 4*x)) & 1u;
			m_ao_bitmask[x][face][i][j] |= (bit0 << (y * 4));
			m_ao_bitmask[x][face][i][j] |= (bit1 << (y * 4 + 1));
			m_ao_bitmask[x][face][i][j] |= (bit2 << (y * 4 + 2));
			m_ao_bitmask[x][face][i][j] |= (bit3 << (y * 4 + 3));
		}
	}
}

void Chunk::updateAdjacentChunkAO(int face, const Chunk& adjacent)
{
}

void Chunk::updateHorizontalMerge(int face, int depth, int i, int j, int x, int y, int z, int type)
{
	if (i < 0 || i >= CHUNK_SIZE || j < 0 || j >= CHUNK_SIZE|| depth <0 || depth>= CHUNK_SIZE)
		return;

	int id = ( ((m_block_cull_bitmask[face][i][j] >> depth) & 1) > 0) ? type : -1;
	unsigned int ao = getAOBitmask(face, i, j, depth);

	m_horizontal_merge[face][depth][i][j] = 
		horizontalMergeUpdatePositivePortion(face, depth, i, j + 1, id, ao,
		x + xPerFace[face][2], y + yPerFace[face][2], z + zPerFace[face][2]);

	horizontalMergeUpdateNegativePortion(face, depth, i, j - 1, id, ao,
		x - xPerFace[face][2], y - yPerFace[face][2], z - zPerFace[face][2],
		m_horizontal_merge[face][depth][i][j]);
}

void Chunk::updateHorizontalMerge(int face, int depth, int i, int j)
{
	if (i < 0 || i >= CHUNK_SIZE || j < 0 || j >= CHUNK_SIZE || depth < 0 || depth >= CHUNK_SIZE)
		return;

	int x = getX(face, depth, i, j);
	int y = getY(face, depth, i, j);
	int z = getZ(face, depth, i, j);
	updateHorizontalMerge(face, depth, i, j, x,y,z,m_pBlocks[y][z][x].getBlockTex(face));
}

void Chunk::horizontalMergeUpdateNegativePortion(int face, int depth, int i, int j, int type, unsigned char _ao, int x, int y, int z, int value)
{
	if (j < 0)
		return;

	int id = ( ((m_block_cull_bitmask[face][i][j] >> depth) & 1) > 0) ? m_pBlocks[y][z][x].getBlockTex(face) : -1;
	unsigned int ao = getAOBitmask(face, i, j, depth);
	bool same = (type == id && ao ==_ao);

	if (same) {
		m_horizontal_merge[face][depth][i][j] = value + 1;

		horizontalMergeUpdateNegativePortion(face, depth, i, j - 1, type, ao,
			x - xPerFace[face][2], y - yPerFace[face][2], z - zPerFace[face][2],
			m_horizontal_merge[face][depth][i][j]);
	}
	else {
		if (m_horizontal_merge[face][depth][i][j] > 1) {
			m_horizontal_merge[face][depth][i][j] = 1;
			horizontalMergeUpdateNegativePortion(face, depth, i, j - 1, id, ao,
				x - xPerFace[face][2], y - yPerFace[face][2], z - zPerFace[face][2],
				m_horizontal_merge[face][depth][i][j]);
		}
	}
}

unsigned char Chunk::horizontalMergeUpdatePositivePortion(int face, int depth, int i, int j, int type, unsigned char _ao, int x, int y, int z)
{
	if (j >= CHUNK_SIZE)
		return 1;

	int id = (((m_block_cull_bitmask[face][i][j] >> depth) & 1) > 0) ? m_pBlocks[y][z][x].getBlockTex(face) : -1;
	unsigned int ao = getAOBitmask(face, i, j, depth);
	bool same = (type == id && _ao == ao);

	if (same) {
		return m_horizontal_merge[face][depth][i][j] + 1;
	}
	else {
		return 1;
	}
}

unsigned int Chunk::getAOBitmask(int face, int i, int j, int depth)
{
	int subIdx = depth / (CHUNK_SIZE / 4);
	int bitStart = (depth % (CHUNK_SIZE / 4)) * 4;	

	return (m_ao_bitmask[subIdx][face][i][j] >> bitStart) & 15u;
}

void Chunk::setBlockEnabled(bool enabled, int x, int y, int z, bool pendUpdate)
{
	if (m_pBlocks[y][z][x].isActive() == enabled)
		return;

	needRefreshMesh = true;
	m_pBlocks[y][z][x].setActive(enabled);

	updateExsistanceBitmask(x, y, z, enabled, pendUpdate);

	if (!pendUpdate) {
		updateAdjacentCullingMask(x, y, z);
	}
	else {
		needRefreshMask = true;
	}
}

