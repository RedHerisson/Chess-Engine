#include "game.h"


std::pair<position, position > Game::randomMoveCmd(bool team) {

	position CooStart, CooEnd;
	std::vector<std::pair<position, position>> legalPosList;
	do {
		std::vector<int> idList;
		std::vector<position> validPosPerPiece;
		std::vector<std::pair<position, position>> validMove;
		std::vector<position> showMoveList;
		legalPosList.empty();
		for (auto piece : PieceList) {
			if (piece.getTeam() == team && piece.getifAlive() == 1) {
				CooStart = piece.getLocation();
				validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
				for (auto Pos : validPosPerPiece) {
					validMove.push_back(std::make_pair(CooStart, Pos));
				}
			}

		}
		for (int i = 0; i < validMove.size(); i++) {
			if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
				legalPosList.push_back(validMove[i]);
			}
		}
		for (int i = 0; i < legalPosList.size(); i++) {
			showMoveList.push_back(legalPosList[i].second);
		}
		//DisplayBoardOverlay(showMoveList);

	} while (legalPosList.size() == 0);
	//std::cout << "nombre de coup possible : " << legalPosList.size() << std::endl;
	srand(time(NULL));
	int idChoosen = rand() % legalPosList.size();
	return legalPosList[idChoosen];
}

void Game::treeSearch(int depth,bool team) {

		depth--;
		int nbrCoup = 1;
		position CooStart, CooEnd;
		std::vector<std::pair<position, position>> legalPosList;
		std::vector<int> idList;
		std::vector<position> validPosPerPiece;
		std::vector<std::pair<position, position>> validMove;
		std::vector<position> showMoveList;
		legalPosList.empty();
		for (auto piece : PieceList) {
			if (piece.getTeam() == team && piece.getifAlive() == 1) {
				CooStart = piece.getLocation();
				validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
				for (auto Pos : validPosPerPiece) {
					
					validMove.push_back(std::make_pair(CooStart, Pos));
				}
			}

		}

		for (int i = 0; i < validMove.size(); i++) {
			if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
				legalPosList.push_back(validMove[i]);
					Game tempoGame = *this;
					//std::cout << "DEPTH :" << depth << "MOVE :" << legalPosList.back().first.first << ":" << legalPosList.back().first.second << "//" << legalPosList.back().second.first << ":" << legalPosList.back().second.second << std::endl;
					tempoGame.movePiece(team, legalPosList.back().first, legalPosList.back().second);
					std::vector<position> tempoAff;
					tempoAff.push_back(legalPosList.back().first); tempoAff.push_back(legalPosList.back().second);
					//DisplayBoardOverlay(tempoAff);
					if (depth != 0)  tempoGame.treeSearch(depth, !team);
					addStat(tempoGame.getStat());
					
					
					
					tempoGame.~Game();

					
			}
		}
		
		if (depth == 0) m_stat.nodes += legalPosList.size();
}

void Game::perf()
{
	bool currPlayer = 1;
	int depth = 5;
	for (int i = 5; i <= depth; i++) {
		treeSearch(i, currPlayer);
		std::cout << "DEPTH " << i << ":" << m_stat.nodes << std::endl;
		//std::cout << "DEPTH " << i << ":" << m_stat.Castles << std::endl;
		resetStat();
	}
}

float Game::minimax(int depth, bool team, int alpha, int beta) {
	depth--;
	incrTour();
	float CurrValue = 0, nodeValue = 0;
	m_nbrTour++;
	position CooStart, CooEnd;
	std::vector<std::pair<position, position>> legalPosList;
	std::vector<position> validPosPerPiece;
	std::vector<std::pair<position, position>> validMove;
	legalPosList.empty();
	if (depth == 0) {
		
		int v = eval(team);
		return v;
	}
	else {
		for (auto piece : PieceList) {
			if (piece.getTeam() == team && piece.getifAlive() == 1) {
				CooStart = piece.getLocation();
				validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
				for (auto Pos : validPosPerPiece) {
					validMove.push_back(std::make_pair(CooStart, Pos));
				}
			}

		}
		nodeValue = -1000000000.0;
		for (int i = 0; i < validMove.size(); i++) {
			if (depth != 1 || (m_board[validMove[i].second.first][validMove[i].second.second].isEmpty())) {
				if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
					legalPosList.push_back(validMove[i]);
						Game tempoGame = *this;
						tempoGame.movePiece(team, legalPosList.back().first, legalPosList.back().second);
						
						float tempoValue;
						tempoValue = -tempoGame.minimax(depth, !team, -beta, -alpha);
						//if (depth == 2) std::cout << "d2:" << getPosFromCoo(legalPosList.back().first) + getPosFromCoo(legalPosList.back().second) << " POINT: " << tempoValue << std::endl;
						if (tempoValue > nodeValue) {
							m_stat.leafMove = tempoGame.getStat().leafMove;
								m_stat.leafMove.push_back(legalPosList.back());
								nodeValue = tempoValue;
						}
					if (nodeValue >= alpha) {
						alpha = nodeValue;
						if (alpha >= beta) {
							//std::cout << team << "-->" << "alpha :" << alpha << " beta :" << beta << " valeur:" << nodeValue << std::endl;
							return nodeValue;
						}
					}

					//if (depth == 1) std::cout <<" || alpha :" << alpha << ", beta :" << beta << std::endl;

					tempoGame.~Game();
				}
			}
		}
	}
	return nodeValue;

}

std::pair<position, position > Game::iAPlayer(int maxDepth, bool team) {
	auto t_start = std::chrono::high_resolution_clock::now();
	int finalBestMoveId = 0;
	position CooStart, CooEnd;
	std::vector<std::pair<position, position>> legalPosList;
	std::vector<int> idList;
	std::vector<position> validPosPerPiece;
	std::vector<std::pair<position, position>> validMove;
	std::vector<std::pair < position, position > > bestMoveList;
	std::vector<std::vector<std::pair < position, position > >> bestMoveHistoric;
	std::vector<position> showMoveList;
	legalPosList.empty();
	for (auto piece : PieceList) {
		if (piece.getTeam() == team && piece.getifAlive() == 1) {
			CooStart = piece.getLocation();
			validPosPerPiece = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
			for (auto Pos : validPosPerPiece) {

				validMove.push_back(std::make_pair(CooStart, Pos));
			}
		}

	}
	float bestHeuristic = -10000000000;
	for (int i = 0; i < validMove.size(); i++) {
		if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, validMove[i].first, validMove[i].second)) {
			legalPosList.push_back(validMove[i]);
			Game tempoGame = *this;
			tempoGame.movePiece(team, legalPosList.back().first, legalPosList.back().second);
			float heuristicValue;
			int alpha = 9999999, beta = - 999999;
			heuristicValue = - tempoGame.minimax(maxDepth, !team,- alpha,- beta);
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
	/*auto t_end = std::chrono::high_resolution_clock::now();
	std::cout << "V1 time :" << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;*/

	return bestMoveList[finalBestMoveId];
}





void Game::resetStat() {
	m_stat.promotions = 0;
	m_stat.capture = 0;
	m_stat.Castles = 0;
	m_stat.checks = 0;
	m_stat.EnPassant = 0;
	m_stat.nodes = 0;
}
