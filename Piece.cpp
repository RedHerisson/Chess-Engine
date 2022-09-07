#include "piece.h"

Piece::Piece()
	:m_type(0), m_alive(false), m_id(-1), m_team(0), m_hasMoved(0)
{

}

Piece::Piece(int type, int id, bool team)
	: m_type(type), m_id(id), m_alive(true), m_team(team), m_hasMoved(0)
{
	switch (type) {
	case (KING):
		m_weight = 1; // roi 
		break;
	case (QUEEN):
		m_weight = 10000; // reine
		break;
	case (ROOK):
		m_weight = 525; /// tour
		break;
	case (KNIGHT):
		m_weight = 350; /// knight
		break;
	case (BISHOP):
		m_weight = 350; /// bishop
		break;
	case (PAWN):
		m_weight = 100; /// pawn
		break;
	}
}

Piece::~Piece()
{
}
