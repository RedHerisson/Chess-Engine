#include "piece.h"

class Tile {

	private :
		bool m_empty;
		int m_pieceId;

	public :	
		Tile();
		~Tile();
		Tile(const Tile& TileOld);
		bool isEmpty() { return m_empty; };
		void setEmpty() { m_empty = true; }
		int getPieceId() { return m_pieceId; };
		void placePiece(Piece* piece, int x, int y);

};