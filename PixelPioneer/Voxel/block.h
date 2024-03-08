#pragma once

class Block {
public: Block();
	  ~Block();
	  bool isActive() { return m_active; }
	  void setActive(bool active) { m_active = active; }
	  bool isTransparent();

private:
	bool m_active;
	int blockId;
};