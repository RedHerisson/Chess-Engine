
#include "piece.h"

class Tile {

private:
	bool m_empty;
	int m_pieceId;

public:
	Tile();
	Tile(bool outOfBoard);
	~Tile();
	Tile(const Tile& TileOld);
	bool isEmpty() { return m_empty; };
	void setEmpty() { m_empty = true; }
	void setFull() { m_empty = false; }
	int getPieceId() { return m_pieceId; };
	bool placePiece(Piece* piece, position pos);

};
