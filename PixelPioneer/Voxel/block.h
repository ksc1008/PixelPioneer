#pragma once

class Block {
public: Block();
	  ~Block();
	  bool isActive() { return m_active; }
	  void setActive(bool active) { m_active = active; }
	  bool isTransparent();
	  int getId() { return blockId; }
	  void setId(int id) { blockId = id; setActive(true); }

private:
	bool m_active;
	int blockId;
};