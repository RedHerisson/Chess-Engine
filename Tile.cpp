
#include "Tile.h"

/// tile header, the board is a array of tile

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

/// link tile pointer to a piece
bool Tile::placePiece(Piece* piece, position pos)
{
	m_pieceId = piece->getId(); // store the id of the piece
	m_empty = false; // set as full
	piece->setLocation(pos); 	// give to the piece his current location
	piece->setHasMoved(1); /// test for real move ?
	return true;
}
