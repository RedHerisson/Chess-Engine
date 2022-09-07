#include "IA.h"

void IA::treeSearch(int depth, int team) {

	depth--;
	int nbrCoup = 1;
	position CooStart, CooEnd;
	std::vector<Move> legalPosList;
	std::vector<int> idList;
	std::vector<Move> validPosPerPiece;
	std::vector<Move> validMove;
	std::vector<position> showMoveList;
	legalPosList.empty();

	for (int it = 31; it >= 0; it--) {
		if (m_currentGame->getPieceFromID(it).getTeam() == team && m_currentGame->getPieceFromID(it).getifAlive() == 1) {
			validPosPerPiece = m_currentGame->getPieceRange(m_currentGame->getPieceFromID(it).getLocation(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getType(), m_currentGame->getPieceFromPos(m_currentGame->getPieceFromID(it).getLocation().x, m_currentGame->getPieceFromID(it).getLocation().y).getTeam());
			for (auto Pos : validPosPerPiece) {

				validMove.push_back(Pos);
			}
		}

	}

	for (int i = 0; i < validMove.size(); i++) {
		if (!m_currentGame->checkPrevision(validMove[i], team)) {
			legalPosList.push_back(validMove[i]);
			LastGameStat gameStat;
			m_currentGame->movePiece(validMove[i], team, &gameStat);
			//std::cout << "DEPTH :" << depth << "MOVE :" << legalPosList.back().first.first << ":" << legalPosList.back().first.second << "//" << legalPosList.back().second.first << ":" << legalPosList.back().second.second << std::endl;

			if (depth != 0)  treeSearch(depth, !team);

			m_currentGame->UnmovePiece(validMove[i], team, gameStat);


		}
	}
	if (depth == 0) m_stat.nodes += legalPosList.size();
}

void IA::perf()
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