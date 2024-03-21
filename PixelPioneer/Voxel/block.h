#pragma once
#include "blockDataStorage.h"

class Block {
public: Block();
	  ~Block();
	  bool isActive() { return m_active; }
	  void setActive(bool active) { m_active = active; }
	  bool isTransparent();
	  int getId() { return blockId; }
	  void setId(int id) { blockId = id; setActive(true); }
	  int getBlockTex(int side) { 	
		  return BlockDataStorage::getInstance()->getBlock(blockId)->m_textures[side]; }

private:
	bool m_active;
	int blockId;
};