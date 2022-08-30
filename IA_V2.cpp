#include "game.h"

float Game::minimaxV2(int depth, bool team, int alpha, int beta, std::unordered_map<unsigned long long, TranspositionTable> TTList[10], std::unordered_map<int, std::pair<position, position> > killerMove[10], unsigned long long keyArray[12][8][8]) {
	depth--;
	incrTour();
	float CurrValue = 0, nodeValue = 0;
	
	if (depth == 0) {
		int v = evalV2(team);
		return v;
	}
	else {
		position CooStart, CooEnd;
		std::vector<std::pair<position, position>> legalPosList;
		std::vector<position> validPosPerPiece;
		std::vector<std::pair<position, position>> validMove;
		std::vector<std::pair<position, position>> tempoKillerMove;
		for (auto piece : PieceList) {
			if (piece.getTeam() == team && piece.getifAlive() == 1) {
				CooStart = piece.getLocation();
				validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
				for (auto Pos : validPosPerPiece) {
						bool isKillerMove = false;
						if (depth > KILLER_DEPTH) {
							std::unordered_map<int, std::pair<position, position> >::const_iterator CoorespondingMove = killerMove[depth].find(hashMove(std::make_pair(CooStart,Pos)));
							if (CoorespondingMove != killerMove[depth].end()) {
								isKillerMove = true;
								tempoKillerMove.push_back(std::make_pair(CooStart, Pos));

							}
						}
						if (!isKillerMove) {
							if (!m_board[Pos.first][Pos.second].isEmpty()) {
								validMove.insert(validMove.begin(), std::make_pair(CooStart, Pos));
							}
							else {
								validMove.push_back(std::make_pair(CooStart, Pos));
							}
						}
					
				}
			}

		}
		for (auto kmove : tempoKillerMove) validMove.insert(validMove.begin(), kmove);
		nodeValue = -10000.0 - depth;
		
		int legalMoveNbr = 0;
		for (int i = 0; i < validMove.size(); i++) {
			if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
				legalMoveNbr++;
				Game tempoGame = *this;
				tempoGame.movePiece(team, validMove[i].first, validMove[i].second);
				float tempoValue;
				bool findTT = false;
				unsigned long long hash = 0;
				if (depth > 1) {
					hash = tempoGame.HashFunction(keyArray);
					std::unordered_map<unsigned long long, TranspositionTable>::const_iterator CorrespondingTable = TTList[depth].find(hash);
					if (CorrespondingTable != TTList[depth].end()) {
						findTT = true;
						tempoValue = CorrespondingTable->second.score;
						tempoGame.StatappendMove(CorrespondingTable->second.leafMove);

					}
				}
				if (findTT == false) {
					tempoValue = -tempoGame.minimaxV2(depth, !team, -beta, -alpha, TTList, killerMove, keyArray);
					if (depth > 1) {
						TranspositionTable newTT;
						newTT.score = tempoValue;
						newTT.leafMove = tempoGame.getStat().leafMove;
						TTList[depth].insert({hash,newTT});
						
					}
				}
				m_stat.nodes += tempoGame.getStat().nodes;
				if (tempoValue > nodeValue) {
					m_stat.leafMove = tempoGame.getStat().leafMove;
					m_stat.leafMove.push_back(validMove[i]);
					nodeValue = tempoValue;
				}
				if (nodeValue >= alpha) {
					alpha = nodeValue;	
					if (nodeValue >= beta) {
						//std::cout << team << "-->" << "alpha :" << alpha << " beta :" << beta << " valeur:" << nodeValue << std::endl;
						if (depth > KILLER_DEPTH) {
							killerMove[depth].insert({ tempoGame.hashMove(validMove[i]), validMove[i] });
						}

						return nodeValue;
					}
				}
				
				
				

				//if (depth == 1) std::cout <<" || alpha :" << alpha << ", beta :" << beta << std::endl;
			}
		}
		m_stat.nodes += legalMoveNbr;
		
	}
	return nodeValue;

}

std::pair<position, position > Game::iAPlayerV2(int maxDepth, bool team, unsigned long long keyArray[12][8][8], std::pair<position, position>* bestNextMove) {
	
	auto t_start = std::chrono::high_resolution_clock::now();
	int finalBestMoveId = 0;
	bool BestNextMoveFound = false;
	position CooStart, CooEnd;
	std::vector<std::pair<position, position>> legalPosList;
	std::vector<int> idList;
	std::vector<position> validPosPerPiece;
	std::vector<std::pair<position, position>> validMove;
	std::vector<std::pair < position, position > > bestMoveList;
	std::vector<std::vector<std::pair < position, position > >> bestMoveHistoric;
	std::vector<position> showMoveList;
	std::unordered_map<unsigned long long, TranspositionTable> TTList[20];
	std::unordered_map<int, std::pair<position, position> > killerMove[20];
	for (auto piece : PieceList) {
		if (piece.getTeam() == team && piece.getifAlive() == 1) {
			CooStart = piece.getLocation();
			validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
			for (auto Pos : validPosPerPiece) {
				if (BestNextMoveFound == false && bestNextMove->first.first == CooStart.first && bestNextMove->first.second == CooStart.second && bestNextMove->second.first == Pos.first && bestNextMove->second.second == Pos.second) {
					BestNextMoveFound = true;
				}
				else {
					if (!m_board[Pos.first][Pos.second].isEmpty()) {
						validMove.insert(validMove.begin(), std::make_pair(CooStart, Pos));
					}
					else {
						validMove.push_back(std::make_pair(CooStart, Pos));
					}
				}
			}
		}

	}
	float bestHeuristic = -1000000;
	
	if(BestNextMoveFound == true) validMove.insert(validMove.begin(), *bestNextMove);
	for (int i = 0; i < validMove.size(); i++) {
		if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
			legalPosList.push_back(validMove[i]);
			Game tempoGame = *this;
			tempoGame.movePiece(team, legalPosList.back().first, legalPosList.back().second);
			float heuristicValue;
			int alpha = 9999999, beta = -999999;
			heuristicValue = -tempoGame.minimaxV2(maxDepth, !team, -alpha, -beta, TTList,	killerMove, keyArray);
			m_stat.nodes += tempoGame.getStat().nodes;
			if (heuristicValue > bestHeuristic) {
				bestHeuristic = heuristicValue;
				bestMoveList.clear();
				bestMoveHistoric.clear();
				bestMoveList.push_back(validMove[i]);
				bestMoveHistoric.push_back(tempoGame.getStat().leafMove);
			}
			else if (heuristicValue == bestHeuristic) {
				bestMoveList.push_back(validMove[i]);
				bestMoveHistoric.push_back(tempoGame.getStat().leafMove);
			}
			if (bestHeuristic <= alpha) {
				alpha = bestHeuristic;
			}
			std::cout << "moveList :";
			for (auto move : tempoGame.getStat().leafMove) std::cout << getPosFromCoo(move.first) << getPosFromCoo(move.second) << ", ";
			std::cout << std::endl;
			std::cout << getPosFromCoo(validMove[i].first) << getPosFromCoo(validMove[i].second) << ":" << heuristicValue << std::endl;

		}

	}
	srand(time(NULL));
	finalBestMoveId = rand() % (bestMoveList.size());
	std::cout << std::endl;
	std::cout << std::endl << "	BEST MOVE :" << getPosFromCoo(bestMoveList[finalBestMoveId].first) << getPosFromCoo(bestMoveList[finalBestMoveId].second) << std::endl;
	std::cout << "moveList :";
	for (auto move : bestMoveHistoric[finalBestMoveId]) std::cout << getPosFromCoo(move.first) << getPosFromCoo(move.second) << ", ";
	std::cout << std::endl;
	std::cout << "nodes explored : " << m_stat.nodes;
	auto t_end = std::chrono::high_resolution_clock::now();
	m_stat.nodes = 0;
	std::cout << " V2 time :" << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;

	if(bestMoveHistoric[finalBestMoveId].size() >= 2) *bestNextMove = bestMoveHistoric[finalBestMoveId][bestMoveHistoric[finalBestMoveId].size() - 2];

	return bestMoveList[finalBestMoveId];
}









float Game::evalV2(bool team) {
	int piecePosPoint[7][8][8] = {
	{
		{0,  0,  0,  0,  0,  0,  0,  0},
		{40, 40, 40, 40, 40, 40, 40, 40},
		{10, 10, 20, 30, 30, 20, 10, 10},
		{5,  5, 10, 25, 25, 10,  5,  5 },
		{0,  0,  0, 20, 20,  0,  0,  0 },
		{5, -5,-10,  0,  0,-10, -5,  5 },
		{15, 15, 10,-20,-20, 10, 15,  15 },
		{0,  0,  0,  0,  0,  0,  0,  0 }, /// pawn point Board
	},
	{
		{-20,-10,-10,-10,-10,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5, 10, 10,  5,  0,-10},
		{-10,  5,  5, 10, 10,  5,  5,-10},
		{-10,  0, 10, 10, 10, 10,  0,-10},
		{-10, 10, 10, 10, 10, 10, 10,-10},
		{-10,  5,  0,  0,  0,  0,  5,-10},
		{-20,-10,-10,-10,-10,-10,-10,-20}, /// bishop point Board
	},
	{
		{ -50,-40,-30,-30,-30,-30,-40,-50},
		{-40,-20,  0,  0,  0,  0,-20,-40},
		{-30,  0, 10, 15, 15, 10,  0,-30},
		{-30,  5, 15, 20, 20, 15,  5,-30},
		{-30,  0, 15, 20, 20, 15,  0,-30},
		{-30,  5, 10, 15, 15, 10,  5,-30},
		{-40,-20,  0,  5,  5,  0,-20,-40},
		{-50,-40,-30,-30,-30,-30,-40,-50}, /// knight point Board
	},
	{
		{0,  0,  0,  0,  0,  0,  0,  0},
		{5, 10, 10, 10, 10, 10, 10,  5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{-5,  0,  0,  0,  0,  0,  0, -5},
		{0,  0,  0,  5,  5,  0,  0,  0}, /// rook point Board
	},
	{
		{ -20,-10,-10, -5, -5,-10,-10,-20},
		{-10,  0,  0,  0,  0,  0,  0,-10},
		{-10,  0,  5,  5,  5,  5,  0,-10},
		{-5,  0,  5,  5,  5,  5,  0, -5},
		{  0,  0,  5,  5,  5,  5,  0, -5},
		{-10,  5,  5,  5,  5,  5,  0,-10},
		{-10,  0,  5,  0,  0,  0,  0,-10},
		{-20,-10,-10, -5, -5,-10,-10,-20}, /// queen point Board
	},
	{
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-30,-40,-40,-50,-50,-40,-40,-30},
		{-20,-30,-30,-40,-40,-30,-30,-20},
		{-10,-20,-20,-20,-20,-20,-20,-10},
		{ 20, 20,  0,  0,  0,  0, 20, 20},
		{ 20, 30, 10,  0,  0, 10, 30, 20}, /// king
	},
	{
		{-50,-40,-30,-20,-20,-30,-40,-50},
		{-30,-20,-10,  0,  0,-10,-20,-30},
		{-30,-10, 20, 30, 30, 20,-10,-30},
		{-30,-10, 30, 40, 40, 30,-10,-30},
		{-30,-10, 30, 40, 40, 30,-10,-30},
		{-30,-10, 20, 30, 30, 20,-10,-30},
		{-30,-30,  0,  0,  0,  0,-30,-30},
		{-50,-30,-30,-30,-30,-30,-30,-50}, // king end
	}
	};
	int evalPoint = 0;
	int MaterialPoint[2] = { 0, 0 };
	int nbrPiece[2];
	nbrPiece[0] = 0;
	nbrPiece[1] = 0;
	for (auto piece : PieceList) {
		if (piece.getifAlive()) {
			//if (piece.getType() == 5) evalPoint += 1000;
			eval_pieceWeightV2(team, piece, MaterialPoint);
			if (piece.getType() != 6) {
				
				if (piece.getTeam() == team) evalPoint += (int)eval_piecePositionV2(team, piece, piecePosPoint);
				else evalPoint -= (int)eval_piecePositionV2(!team, piece, piecePosPoint);
			}
		}
	}
	evalPoint += eval_kingBestPosV2(team, MaterialPoint, piecePosPoint);
	evalPoint -= eval_kingBestPosV2(!team, MaterialPoint, piecePosPoint);
	evalPoint += (150 * (MaterialPoint[0] + MaterialPoint[1])) / (97 * ((abs(MaterialPoint[0] + MaterialPoint[1]) / 970) + 1));

	//evalPoint += eval_kingProtection(team);
	evalPoint += eval_kingCheckV2(team);
	return evalPoint;
	
}

void Game::eval_pieceWeightV2(bool team, Piece piece, int materialPoint[2]) {
	float evalPoint = 0;
	if (piece.getTeam() == team) {
		//if (piece.getType() == 2) return 400;
		materialPoint[team] += piece.getWeight();
	}
	else materialPoint[!team] -= piece.getWeight();
}

float Game::eval_centerPositionV2(bool team, Piece piece, int boardPoint[8][8]) {

	//std::cout << boardPoint[piece.getLocation().first][piece.getLocation().second] * piece.getWeight() << std::endl;
	return ((boardPoint[piece.getLocation().first][piece.getLocation().second] * piece.getWeight() * 0.05) / m_nbrTour);


}
float Game::eval_kingProtectionV2(bool team) {
	int evalPoint = 0;
	std::vector<position > kingMove;
	kingMove = checkKingMove(PieceList[kingId[team]].getLocation(), team);
	for (auto move : kingMove) {
		if (!m_board[move.first][move.second].isEmpty()) {
			if (PieceList[m_board[move.first][move.second].getPieceId()].getType() == 1) evalPoint += 1;
		}
	}

	return evalPoint * 0.6 * m_nbrTour * 20;
} // dep

float Game::eval_kingCheckV2(bool team) {

	int evalPoint = 0;

	if (m_kingInCheck[team]) {
		std::vector<position> kingMoveList = checkKingMove(PieceList[kingId[team]].getLocation(), team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases dispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (checkCheckOnTile(KingMove, !team)) tileOnCheck++;
		}
		evalPoint -= 400 / (pow(10, -0.113636 * tileOnCheck) + 1) - 200;
		evalPoint -= 404.4 / (kingMoveNbr + 3);
	}

	if (m_kingInCheck[!team]) {

		std::vector<position> kingMoveList = checkKingMove(PieceList[kingId[!team]].getLocation(), !team);
		int kingMoveNbr = kingMoveList.size(); /// nbr de cases indispo
		int tileOnCheck = 0;
		for (auto KingMove : kingMoveList) {
			if (checkCheckOnTile(KingMove, team)) tileOnCheck++;
		}

		evalPoint += 400 / (pow(10, -0.113636 * tileOnCheck) + 1) - 200;
		evalPoint += 404.4 / (kingMoveNbr + 3);


	}

	return evalPoint;
}

float Game::eval_piecePositionV2(bool team, Piece piece, int piecePosPoint[7][8][8]) {
	float evalPoint = 0.0;
	if(team == 1)  evalPoint =  piecePosPoint[piece.getType() - 1][piece.getLocation().first][piece.getLocation().second];
	else evalPoint =  piecePosPoint[piece.getType() - 1][7 - piece.getLocation().first][piece.getLocation().second];
	return evalPoint;
}

int Game::eval_kingBestPosV2(bool team, int MaterialPoint[2], int piecePosPoint[7][8][8])
{
	 // 1/3 des points, fin de partie

	if (team == 1) {
		if (abs(MaterialPoint[team]) <= 1400) {
			return piecePosPoint[6][PieceList[kingId[team]].getLocation().first][PieceList[kingId[team]].getLocation().second];
		}
		else return piecePosPoint[5][PieceList[kingId[team]].getLocation().first][PieceList[kingId[team]].getLocation().second];	
	}
	else {
		if (abs(MaterialPoint[team]) <= 1400) {
			return piecePosPoint[6][7 - PieceList[kingId[team]].getLocation().first][PieceList[kingId[team]].getLocation().second];
		}
		else return piecePosPoint[5][7 - PieceList[kingId[team]].getLocation().first][PieceList[kingId[team]].getLocation().second];

	}
}