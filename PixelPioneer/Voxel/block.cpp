#include "block.h"

Block::Block()
{
	blockId = -1;
	m_active = false;
}

Block::~Block()
{
}

bool Block::isTransparent()
{
	return false;
}
