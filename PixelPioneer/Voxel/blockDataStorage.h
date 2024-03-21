#pragma once
#include <string>

struct blockData{
	char m_name[128] = "unnamed";
	int m_textures[6] = { -1 };

	blockData() {
		strcpy_s(m_name, 128 * sizeof(char), "unnamed");
	}

	blockData(char* name, int texID) {
		strcpy_s(name, 128 * sizeof(char), m_name);
		for (int i = 0; i < 6; i++)
			m_textures[i] = texID;
	}

	blockData(char* name, int texTop, int texSide, int texBottom) {
		strcpy_s(name, 128 * sizeof(char), m_name);
		m_textures[0] = texBottom;
		m_textures[1] = texTop;
		m_textures[2] = texSide; m_textures[3] = texSide; m_textures[4] = texSide; m_textures[5] = texSide;
	}
};

class BlockDataStorage {
	blockData* blocks;
	const int BLOCK_CNT = 4;
	static BlockDataStorage _instance;
	static blockData _void;

public:
	BlockDataStorage() {
		blocks = new blockData[BLOCK_CNT]{};
		setBlock(0, "stone", 0);
		setBlock(1, "cobble", 1);
		setBlock(2, "dirt", 2);
		setBlockThreeSide(3, "grass", 4,2,3);
	}
	static BlockDataStorage* getInstance() { return &_instance; }

	void setBlock(int id, const char* name, int texId) {
		strcpy_s(blocks[id].m_name, 128 * sizeof(char), name);
		for (int i = 0; i < 6; i++)
			blocks[id].m_textures[i] = texId;
	}

	void setBlockThreeSide(int id, const char* name, int texTop, int texBottom, int texSide) {
		strcpy_s(blocks[id].m_name, 128 * sizeof(char), name);
		blocks[id].m_textures[0] = texBottom;
		blocks[id].m_textures[1] = texTop;
		blocks[id].m_textures[2] = texSide; blocks[id].m_textures[3] = texSide; 
		blocks[id].m_textures[4] = texSide; blocks[id].m_textures[5] = texSide;
	}

	const blockData* getBlock(int id) {
		if (id < 0 || id >= BLOCK_CNT)
			return &_void;
		return &blocks[id];
	}
};