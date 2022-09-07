#include "Game.h"

class IA {
private :
	int maxDepth;
	int m_piecePosPoint[7][8][8];
	std::unordered_map<int, Move> m_killerMoveCmpt[2];
	std::multiset<Move> m_killerMove[2];
	std::unordered_map<unsigned long long, TranspositionTable> m_TTList[20];

	Move m_bestNextMove;
	gamestat m_stat;
	Game* m_currentGame;

public :

	IA(Game* currentGame); 
	~IA();

	/// performance test 
	void treeSearch(int depth, int team);
	float minimax(int depth, bool team, int alpha, int beta);
	void perf();

	Move findMove(int maxDepth, bool team);

	void updateMoveValue(Move* move, int depth, bool team);

	void resetStat();
	void StatappendMove(std::vector<Move> ToAppendMove) { m_stat.leafMove.insert(m_stat.leafMove.end(), ToAppendMove.begin(), ToAppendMove.end()); };

	float eval(int team);
	void eval_pieceWeight(Piece piece, int materialPoint[2], int team);
	//float eval_kingProtection(int team); // dep
	float eval_kingCheck(int team);
	int eval_kingBestPos(int MaterialPoint[2], int team);
	float eval_piecePosition(Piece piece, int team);

};