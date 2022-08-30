#include "piece.h"

Piece::Piece()
	:m_type(0), m_alive(false), m_id(-1), m_team(-1), m_hasMoved(0)
{

}

Piece::Piece(int type, int id, bool team)
	: m_type(type),m_id(id), m_alive(true),m_team(team), m_hasMoved(0)
{
	switch (type) {
	case (6):
		m_weight = 1; // roi
		break;
	case (5):
		m_weight = 10000; // reine
		break;
	case (4):
		m_weight = 525; /// tour
		break;
	case (3):
		m_weight = 350; /// knight
		break;
	case (2):
		m_weight = 350; /// bishop
		break;
	case (1):
		m_weight = 100; /// pawn
		break;
	}
}

Piece::~Piece()
{
}
