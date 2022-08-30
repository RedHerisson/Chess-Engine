#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <unordered_map>
#include "player.h"
#include "tile.h"

#define NUMTHREADS 8



typedef struct enPassant {
	int ligne;
	bool Available;
	bool team;
};

class Game;

typedef struct gamestat {
	int nodes;
	int capture;
	int EnPassant;
	int promotions;
	int Castles;
	int checks;
	std::vector< std::pair < position, position > > leafMove;
	
};

typedef struct TranspositionTable {
	int score;
	std::vector< std::pair < position, position > > leafMove;
};

class Game {
private:
	Piece PieceList[32];
	bool m_antiRecursion;
	int kingId[2];
	bool m_kingInCheck[2];
	Tile m_board[8][8];
	enPassant m_enPassant;
	int m_nbrTour;
	gamestat m_stat;


public:
	Game();
	~Game();
	Game(const Game& g1);
	void BoardStartup();
	std::pair<position, position > userMoveCmd(bool team);
	std::pair<position, position > randomMoveCmd(bool team);
	std::pair<position, position> MoveSelection(bool team);
	void treeSearch(int depth, bool team);
	float minimax(int depth, bool team, int alpha, int beta);
	std::pair<position, position > iAPlayer(int maxDepth, bool team);
	void perf();
	void resetStat();
	void StatappendMove(std::vector< std::pair < position, position > > ToAppendMove) { m_stat.leafMove.insert(m_stat.leafMove.end(), ToAppendMove.begin(), ToAppendMove.end()); };

	void movePiece(int PlayerId, position CooStart, position CooEnd, bool realMove = 0);



	position getCooFromPos(char col, char lig);
	std::string getPosFromCoo(position coo);
	position newPosition(int a, int b);

	bool checkMove(position cooStart, position cooEnd, int pieceType, bool pieceTeam);
	bool isSameTeam(position cooStart, position CooEnd);
	std::vector<position> ValidCooByType(position cooStart, int pieceType, bool pieceTeam);
	std::vector<position> checkPawnMove(position coo, bool team);
	std::vector<position> checkBishopMove(position coo);
	std::vector<position> checkKnightMove(position coo);
	std::vector<position> checkRookMove(position coo);
	std::vector<position> checkQueenMove(position coo);
	std::vector<position> checkKingMove(position coo, bool team);
	std::vector<position> checkKingMoveAR(position coo, int team);

	bool checkCheckOnTile(position coo, bool opteam);
	bool checkPrevision(position coo, bool Opteam, position cooStart, position cooEnd);
	std::vector<position> checkForCastling(bool team);

	bool checkForMat(bool team);

	void DisplayBoard();
	void DisplayBoardOverlay(std::vector<position> validPos);

	void incrTour() { m_nbrTour++; };

	Piece* getPieceList() { return PieceList; }
	enPassant getEnPassantList() { return m_enPassant; };
	gamestat getStat() { return m_stat; };
	int getNbrTour() { return m_nbrTour; };
	bool getIfKingIsInCheck(bool team) { return m_kingInCheck[team]; };
	void addStat(gamestat stat);

	float eval(bool team);
	float eval_pieceWeight(bool team, Piece piece);
	float eval_centerPosition(bool team, Piece piece, int boardPoint[8][8]);
	float eval_kingProtection(bool team); // dep
	float eval_kingCheck(bool team);
	float eval_casltingBonus(bool team);
	float eval_pawnPromotionBonus(bool team);
	float eval_lateGameMat(bool team);

	float minimaxV2(int depth, bool team, int alpha, int beta, std::unordered_map<unsigned long long, TranspositionTable> TTList[10], std::unordered_map<int, std::pair<position, position> > killerMove[10], unsigned long long keyArray[12][8][8]);
	std::pair<position, position > iAPlayerV2(int maxDepth, bool team, unsigned long long keyArray[12][8][8], std::pair<position, position>* bestNextMove);
	float evalV2(bool team);
	void eval_pieceWeightV2(bool team, Piece piece, int materialPoint[2]);
	float eval_centerPositionV2(bool team, Piece piece, int boardPoint[8][8]);
	float eval_kingProtectionV2(bool team); // dep
	float eval_kingCheckV2(bool team);
	int eval_kingBestPosV2(bool team, int MaterialPoint[2], int PiecePosPoint[7][8][8]);
	float eval_piecePositionV2(bool team, Piece piece, int PiecePosPoint[7][8][8]);

	void HashKey(unsigned long long keyArray[12][8][8]);
	int hashMove(std::pair<position, position> move);
	unsigned rand256();
	unsigned long long rand64bits();
	unsigned long long HashFunction(unsigned long long keyArray[12][8][8]);


};
#define PLAYER0_IS_IA 0
#define PLAYER1_IS_IA 0
#define KILLER_DEPTH 2

