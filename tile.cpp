#include "tile.h"
#include <iostream>

Tile::Tile()
	: m_empty(true)
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

void Tile::placePiece(Piece* piece, int x, int y)
{
	m_pieceId = piece->getId();
	m_empty = false; 
	position newPos;
	newPos.first = x;
	newPos.second = y;
	piece->setLocation(newPos);
	piece->setHasMoved(1);
}
