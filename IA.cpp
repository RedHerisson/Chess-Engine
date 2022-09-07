#include "IA.h"


IA::IA(Game* currentGame) {
	m_currentGame = currentGame;
	
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
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				m_piecePosPoint[i][j][k] = piecePosPoint[i][j][k];
			}
		}
	}
	resetStat();
}

IA::~IA()
{
}


float IA::minimax(int depth, bool team, int alpha, int beta) {
	depth--;
	m_currentGame->incrTour();
	float CurrValue = 0, nodeValue = 0;

	if (depth == 0) {
		int v = eval(team);
		return v;
	}
	else {
		std::vector<Move> validPosPerPiece;
		std::multiset<Move> validMove;

		for (int it = 0; it < 32 ; it++) {
			if (m_currentGame->getPieceFromID(it).getTeam() == team && m_currentGame->getPieceFromID(it).getifAlive() == 1) {
				
				validPosPerPiece = m_currentGame->getPieceRange(m_currentGame->getPieceFromID(it).getLocation(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getType(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getTeam() );

				for (int j = 0; j < validPosPerPiece.size(); j++) {
					updateMoveValue(&validPosPerPiece[j], depth, team);
					validMove.insert(validPosPerPiece[j]);
				}
			}

		}
		nodeValue = -10000.0 - depth;
		int legalMoveNbr = 0;
		for (auto move : validMove) {
			if (!m_currentGame->checkPrevision(move, team)) {
				legalMoveNbr++;
				LastGameStat gameStat;
				m_currentGame->movePiece(move, team, &gameStat);
				float tempoValue;
				bool findTT = false;
				unsigned long long hash = 0;
				if (depth > 1) {
					hash = m_currentGame->HashFunction();
					std::unordered_map<unsigned long long, TranspositionTable>::const_iterator CorrespondingTable = m_TTList[depth].find(hash);
					if (CorrespondingTable != m_TTList[depth].end()) {
						findTT = true;
						tempoValue = CorrespondingTable->second.score;
						//StatappendMove(CorrespondingTable->second.leafMove);

					}
				}
				if (findTT == false) {
					tempoValue = - minimax(depth, !team, -beta, -alpha);
					
					if (depth > 1) {
						TranspositionTable newTT;
						newTT.score = tempoValue;
						//newTT.leafMove = m_stat.leafMove;
						m_TTList[depth].insert({ hash,newTT });

					}
				}
				m_currentGame->UnmovePiece(move, team, gameStat);

				if (tempoValue > nodeValue) {
					//m_stat.leafMove.push_back(move);
					nodeValue = tempoValue;
				}
				if (nodeValue >= alpha) {
					alpha = nodeValue;
					if (nodeValue >= beta) {

						if (depth >0) {
							int moveHash = m_currentGame->hashMove(move);
							std::unordered_map<int, Move>::const_iterator index = m_killerMoveCmpt[team].find(moveHash);
							if (index == m_killerMoveCmpt[team].end()) { // if move not in the killer move list
								move.orderScore = depth * depth;
								m_killerMoveCmpt[team].insert({ moveHash,move});
							}
							else {
								move.orderScore = depth * depth + index->second.orderScore; // else increase the counter
								
							}
							
							if (m_killerMove->size() >= 5) {	// if the set is not empty


								std::cout << "LOWEST ELEMENT :" << m_killerMove[team].rbegin()->orderScore << std::endl;
								if (move.orderScore > m_killerMove->rbegin()->orderScore) {
									m_killerMove[team].erase(*m_killerMove->rbegin());
									m_killerMove[team].insert(move);
								}
								m_killerMoveCmpt[team].insert({ moveHash,move });
							}
							else m_killerMove[team].insert(move);
							
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

Move IA::findMove(int maxDepth, bool team) {

	auto t_start = std::chrono::high_resolution_clock::now();
	int finalBestMoveId = 0;
	bool BestNextMoveFound = false;
	position CooStart, CooEnd;
	std::vector<int> idList;
	std::multiset<Move> validMove;
	std::vector<Move> bestMoveList;
	std::vector<std::vector<Move>> bestMoveHistoric;
	std::vector<position> showMoveList;

	std::vector<Move> validPosPerPiece;
	


	for (int it = 0; it < 32; it++) {
		if (m_currentGame->getPieceFromID(it).getTeam() == team && m_currentGame->getPieceFromID(it).getifAlive() == 1) {
			
			validPosPerPiece = m_currentGame->getPieceRange(m_currentGame->getPieceFromID(it).getLocation(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getType(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getTeam());
			for (int j = 0; j < validPosPerPiece.size(); j++) {
				/*
				if (BestNextMoveFound == false && m_bestNextMove.from.x == validPosPerPiece[j].from.x && m_bestNextMove.from.y == m_currentGame->getPieceFromID(it).getLocation().x && m_bestNextMove.to.x == validPosPerPiece[j].to.x && m_bestNextMove.to.y == validPosPerPiece[j].from.y) {
					BestNextMoveFound = true;
				}*/
				
				updateMoveValue(&validPosPerPiece[j], maxDepth, team);
				validMove.insert(validPosPerPiece[j]);
			}
		}

	}
	float bestHeuristic = -1000000;

	for (auto move : validMove) {
		std::cout << "test" << std::endl;
		if (!m_currentGame->checkPrevision(move, team)) {
			LastGameStat gameStat;
			m_currentGame->movePiece(move,team, &gameStat);
			float heuristicValue;
			int alpha = 9999999, beta = -999999;
			heuristicValue = - minimax(maxDepth, !team, -alpha, -beta);
	
			if (heuristicValue > bestHeuristic) {
				bestHeuristic = heuristicValue;
				bestMoveList.clear();
				bestMoveHistoric.clear();
				bestMoveList.push_back(move);
				bestMoveHistoric.push_back(m_stat.leafMove);
			}
			else if (heuristicValue == bestHeuristic) {
				bestMoveList.push_back(move);
				bestMoveHistoric.push_back(m_stat.leafMove);
			}
			if (bestHeuristic <= alpha) {
				alpha = bestHeuristic;
			}
			std::cout << "moveList :";
			//for (auto move : m_stat.leafMove) std::cout << m_currentGame->getPosFromCoo(move.from) << m_currentGame->getPosFromCoo(move.to) << ", ";
			std::cout << std::endl;
			std::cout << m_currentGame->getPosFromCoo(move.from) << m_currentGame->getPosFromCoo(move.to) << ":" << heuristicValue  << " |  Move Value : "<< move.orderScore << std::endl;
			m_currentGame->UnmovePiece(move, team, gameStat);
			m_stat.leafMove.clear();
		}
		

	}
	srand(time(NULL));
	finalBestMoveId = rand() % (bestMoveList.size());
	std::cout << std::endl;
	std::cout << std::endl << "	BEST MOVE :" << m_currentGame->getPosFromCoo(bestMoveList[finalBestMoveId].from) << m_currentGame->getPosFromCoo(bestMoveList[finalBestMoveId].to) << std::endl;
	std::cout << "moveList :";
	for (auto move : bestMoveHistoric[finalBestMoveId]) std::cout << m_currentGame->getPosFromCoo(move.from) << m_currentGame->getPosFromCoo(move.to) << ", ";
	std::cout << std::endl;
	std::cout << "nodes explored : " << m_stat.nodes;
	auto t_end = std::chrono::high_resolution_clock::now();
	m_stat.nodes = 0;
	std::cout << " V2 time :" << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;
	if (bestMoveHistoric[finalBestMoveId].size() >= 2) m_bestNextMove = bestMoveHistoric[finalBestMoveId][bestMoveHistoric[finalBestMoveId].size() - 2];

	return bestMoveList[finalBestMoveId];
}

void IA::updateMoveValue(Move* move, int depth, bool team)
{
	move->orderScore = 0;
	// killer move ordering
	if (depth > KILLER_DEPTH) {
		int it = 0;
		std::cout << m_killerMove->size() << std::endl;
		for (auto kMove : m_killerMove[team]) {
			
			std::cout << m_currentGame->getPosFromCoo(kMove.from) << m_currentGame->getPosFromCoo(kMove.to) <<": " << kMove.orderScore <<  std::endl;
			if (kMove == *move)
				move->orderScore += 100000+(5-it);
		}
	}

	//capture ordering
	if (move->capture) {
		move->orderScore += 10000 + m_currentGame->getPieceFromPos(move->to.x, move->to.y).getWeight() * 10;
	}

	//piece Weight ordering
	move->orderScore += m_currentGame->getPieceFromPos(move->from.x, move->from.y).getWeight();

	//move->orderScore += m_piecePosPoint[m_currentGame->getPieceFromPos(move->from.x, move->from.y).getId()-1][move->to.x][move->to.y] - m_piecePosPoint[m_currentGame->getPieceFromPos(move->from.x, move->from.y).getId()-1][move->from.x][move->from.y];

	if (move->Promotion) {
		move->orderScore += 10000;
	}
}




void IA::resetStat() {
	m_stat.promotions = 0;
	m_stat.capture = 0;
	m_stat.Castles = 0;
	m_stat.checks = 0;
	m_stat.EnPassant = 0;
	m_stat.nodes = 0;
	m_stat.leafMove.clear();
}