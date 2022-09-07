#include "IA.h"


float IA::eval(int team) {
	
	int evalPoint = 0;
	int MaterialPoint[2] = { 0, 0 };
	int nbrPiece[2];
	nbrPiece[0] = 0;
	nbrPiece[1] = 0;
	for (int it = 31; it >= 0; it--) {
		if (m_currentGame->getPieceFromID(it).getifAlive()) {
			//if (piece.getType() == 5) evalPoint += 1000;
			eval_pieceWeight(m_currentGame->getPieceFromID(it), MaterialPoint, team);
			if (m_currentGame->getPieceFromID(it).getType() != 6) {

				if (m_currentGame->getPieceFromID(it).getTeam() == team) evalPoint += (int)eval_piecePosition(m_currentGame->getPieceFromID(it), team);
				else evalPoint -= (int)eval_piecePosition(m_currentGame->getPieceFromID(it), !team);
			}
		}
	}
	evalPoint += eval_kingBestPos(MaterialPoint, team);
	evalPoint -= eval_kingBestPos(MaterialPoint, !team);
	evalPoint += (150 * (MaterialPoint[0] + MaterialPoint[1])) / (97 * ((abs(MaterialPoint[0] + MaterialPoint[1]) / 970) + 1));

	//evalPoint += eval_kingProtection(team);
	evalPoint += eval_kingCheck(team);
	return evalPoint;

}

void IA::eval_pieceWeight(Piece piece, int materialPoint[2], int team) {
	float evalPoint = 0;
	if (piece.getTeam() == team) {
		//if (piece.getType() == 2) return 400;
		materialPoint[team] += piece.getWeight();
	}
	else materialPoint[!team] -= piece.getWeight();
}
/*
float IA::eval_kingProtection(int team) {
	int evalPoint = 0;
	std::vector<position > kingMove;
	kingMove = checkKingMove(PieceList[kingId[team]].getLocation(), team);
	for (auto move : kingMove) {
		if (!m_board[move.first][move.second].isEmpty()) {
			if (PieceList[m_board[move.first][move.second].getPieceId()].getType() == 1) evalPoint += 1;
		}
	}

	return evalPoint * 0.6 * m_nbrTour * 20;
} // dep*/

float IA::eval_kingCheck(int team) {

	int evalPoint = 0;

	if (m_currentGame->getIfKingIsInCheck(team)) {
		std::vector<Move> kingMoveList = m_currentGame->checkKingMove(m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation(), team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases dispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (m_currentGame->checkCheckOnTile(KingMove.to, !team)) tileOnCheck++;
		}
		evalPoint -= 400 / (pow(10, -0.113636 * tileOnCheck) + 1) - 200;
		evalPoint -= 404.4 / (kingMoveNbr + 3);
	}

	if (m_currentGame->getIfKingIsInCheck(!team)) {

		std::vector<Move> kingMoveList = m_currentGame->checkKingMove(m_currentGame->getPieceFromID(m_currentGame->getKingId(!team)).getLocation(), !team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases indispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (m_currentGame->checkCheckOnTile(KingMove.to, team)) tileOnCheck++;
		}

		evalPoint += 400 / (pow(10, -0.113636 * tileOnCheck) + 1) - 200;
		evalPoint += 404.4 / (kingMoveNbr + 3);


	}

	return evalPoint;
}

float IA::eval_piecePosition(Piece piece, int team) {
	float evalPoint = 0.0;
	if (team == 1)  evalPoint = m_piecePosPoint[piece.getType() - 1][piece.getLocation().x][piece.getLocation().y];
	else evalPoint = m_piecePosPoint[piece.getType() - 1][7 - piece.getLocation().x][piece.getLocation().y];
	return evalPoint;
}

int IA::eval_kingBestPos(int MaterialPoint[2], int team) {
	// 1/3 des points, fin de partie

	if (team == 1) {
		if (abs(MaterialPoint[team]) <= 1400) {
			
			return m_piecePosPoint[6][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().x][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().y];
		}
		else return m_piecePosPoint[5][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().x][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().y];
	}
	else {
		if (abs(MaterialPoint[team]) <= 1400) {
			return m_piecePosPoint[6][7 - m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().x][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().y];
		}
		else return m_piecePosPoint[5][7 - m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().x][m_currentGame->getPieceFromID(m_currentGame->getKingId(team)).getLocation().y];

	}
}