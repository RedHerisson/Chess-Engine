#include "Game.h"

/// definition of the class IA, using to find best next move in chess

class IA {
private :
	int maxDepth;	/// the max depth the minMax algorithm can go to
	int m_piecePosPoint[7][8][8];	/// point map for the evalution of each piece position
	std::unordered_map<int, Move> m_killerMove[20];	/// killer move vector
	std::unordered_map<unsigned long long, TranspositionTable> m_TTList[20];	/// transpotition table
	// note -> set as 20 because of depth, increase if you want to try higher depth than 20

	Move m_bestNextMove;	/// last best move find
	gamestat m_stat;	/// stat for data analysis at debug
	Game* m_currentGame;	/// pointer to the current game

public :

	IA(Game* currentGame);	/// constuctor of the IA object
	~IA();

	/// in IA.cpp
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
