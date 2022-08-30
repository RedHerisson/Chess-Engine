#include "game.h"



float Game::eval(bool team) {

	int evalPoint = 0;
	float MaterialPoint = 0;
	int nbrPiece[2];
	nbrPiece[0] = 0;
	nbrPiece[1] = 0;
	int boardPoint[8][8] = {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,1,2,2,1,0,0},
		{0,0,1,2,2,1,0,0},
		{0,0,1,1,1,1,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0}
	};
	for (auto piece : PieceList) {
		MaterialPoint += eval_pieceWeight(team, piece);
		//evalPoint += eval_centerPosition(team, piece, boardPoint);
		if (piece.getifAlive()) {
			if (piece.getTeam() == 0) nbrPiece[0]++;
			else nbrPiece[1]++;
		}
	}
	MaterialPoint = (150 * MaterialPoint) / (97 * ((abs(MaterialPoint) / 970) + 1));
	
	evalPoint += MaterialPoint;
	if (nbrPiece[team] <= 3) evalPoint -= eval_lateGameMat(!team);
	if (nbrPiece[!team] <= 3) evalPoint += eval_lateGameMat(team);
	//evalPoint += eval_kingProtection(team);
	evalPoint += eval_kingCheck(team);
	return evalPoint;
}

float Game::eval_pieceWeight(bool team, Piece piece) {
	float evalPoint = 0;
	if (piece.getifAlive()) {
		if (piece.getTeam() == team) {
			//if (piece.getType() == 2) return 400;
			return piece.getWeight();
		}
		else return ( - piece.getWeight());
	}
	return 0;
}

float Game::eval_centerPosition(bool team, Piece piece, int boardPoint[8][8]) {
	
	//std::cout << boardPoint[piece.getLocation().first][piece.getLocation().second] * piece.getWeight() << std::endl;
	return ((boardPoint[piece.getLocation().first][piece.getLocation().second] * piece.getWeight()*0.05)/m_nbrTour);

	
}
float Game::eval_kingProtection(bool team) {
	int evalPoint = 0;
	std::vector<position > kingMove;
	kingMove = checkKingMove(PieceList[kingId[team]].getLocation(), team);
	for (auto move : kingMove) {
		if (!m_board[move.first][move.second].isEmpty()) {
			if (PieceList[m_board[move.first][move.second].getPieceId()].getType() == 1) evalPoint += 1;
		}
	}

	return evalPoint *0.6* m_nbrTour * 20;
} // dep

float Game::eval_kingCheck(bool team) {

	int evalPoint = 0;
	
	if (m_kingInCheck[team]) {
		std::vector<position> kingMoveList = checkKingMove(PieceList[kingId[team]].getLocation(), team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases dispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (checkCheckOnTile(KingMove, !team)) tileOnCheck++;
		}
		evalPoint -= 200 / (pow(10, -0.113636 * tileOnCheck) + 1) - 100;
		evalPoint -= 404.4 / (kingMoveNbr + 3);
	}

	if (m_kingInCheck[!team]) {
		
		std::vector<position> kingMoveList = checkKingMove(PieceList[kingId[!team]].getLocation(), !team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases indispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (checkCheckOnTile(KingMove, team)) tileOnCheck++;
		}
		
		evalPoint += 200 / (pow(10, -0.113636 * tileOnCheck) + 1) - 100;
		evalPoint += 404.4 / (kingMoveNbr + 3);


	}
	
	return evalPoint;
}

float Game::eval_casltingBonus(bool team) {
	//if (m_stat.Castles == 1) return 75;
	return 0.0;
}
float Game::eval_pawnPromotionBonus(bool team) {
	return 0.0;
}

float eval_terrainOccupation(bool team) {
	return 0.0;
}

float Game::eval_lateGameMat(bool team) {
	float evalPoint = 0.0;
	int boardPoint[8][8] = {
		{5,4,3,3,3,3,4,5},
		{4,2,1,1,1,1,2,4},
		{3,1,0,0,0,0,1,3},
		{3,1,0,0,0,0,1,3},
		{3,1,0,0,0,0,1,3},
		{3,1,0,0,0,0,1,3},
		{4,2,1,1,1,1,2,4},
		{5,4,3,3,3,3,4,5}
	};
	evalPoint = 10* boardPoint[PieceList[kingId[!team]].getLocation().first][PieceList[kingId[!team]].getLocation().second];
	return evalPoint;
}
