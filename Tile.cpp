
#include "Tile.h"

Tile::Tile()
	: m_empty(true), m_pieceId(-1)
{
}


Tile::~Tile()
{
}

Tile::Tile(const Tile& TileOld)
{
	m_empty = TileOld.m_empty;
	m_pieceId = TileOld.m_pieceId;
}

bool Tile::placePiece(Piece* piece, position pos)
{
	m_pieceId = piece->getId();
	m_empty = false;
	piece->setLocation(pos);
	piece->setHasMoved(1);
	return true;
}