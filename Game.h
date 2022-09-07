#include "Tile.h"
#include "definition.h"

class Game {
private :
	unsigned long long m_keyArray[12][8][8];
	Piece m_pieceList[32];
	int m_kingId[2];
	bool m_kingInCheck[2];
	Tile m_board[8][8];
	enPassant m_enPassant;
	int m_nbrTour;
	gamestat m_stat;

public :
	Game(std::string fen, unsigned long long keyArraySeed = 0);
	~Game();

	void BoardStartup(std::string fen);
	bool checkMove(Move* move, int pieceType, bool pieceTeam);
	position getCooFromPos(char col, char lig);
	std::string getPosFromCoo(position coo);


	void movePiece(Move move, bool team, LastGameStat* lastState);
	void UnmovePiece(Move lastMove, bool team, LastGameStat lastState);

	Move userMoveCmd(bool team);

	bool isSameTeam(position from, position to);
	void incrTour() { m_nbrTour++; };

	

	bool checkCheckOnTile(position coo, bool opteam);
	bool checkPrevision(Move move, bool team);
	std::vector<Move> checkForCastling(bool team);
	bool checkForMat(bool team);

	Move newMove(position from, position to, bool capture, int promotion, bool castling, bool TakeenPassant);
	std::vector<Move> getPieceRange(position from, int pieceType, bool pieceTeam);
	std::vector<Move> checkPawnMove(position from, bool team);
	std::vector<Move> checkBishopMove(position from);
	std::vector<Move> checkKnightMove(position from);
	std::vector<Move> checkRookMove(position from);
	std::vector<Move> checkQueenMove(position from);
	std::vector<Move> checkKingMove(position from, bool team);

	position newPosition(int x, int y);

	void displayMoveInfo(Move move);

	bool getIfKingIsInCheck(bool team) { return m_kingInCheck[team]; };
	void getPieceList(Piece* pieceList) { pieceList = m_pieceList;};
	Piece getPieceFromPos(int x, int y) { return m_pieceList[m_board[x][y].getPieceId()]; };
	Piece getPieceFromID(int id) { return m_pieceList[id]; };
	bool getIfTileEmpty(int x, int y) { return m_board[x][y].isEmpty(); };
	int getKingId(bool team) { return m_kingId[team]; };
	int getNbrTour() { return m_nbrTour; };

	void DisplayBoard();
	void DisplayBoardOverlay(std::vector<Move> validPos);

	void HashKey();
	unsigned long long HashFunction();
	int hashMove(Move move);
	unsigned rand256();
	unsigned long long rand64bits();
	
};
