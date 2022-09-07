#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <queue>
#include <set>

typedef struct enPassant {
	int ligne;
	bool Available;
	bool team;
};

typedef struct position {
	int x;
	int y;
};


typedef struct Move {
	position from;
	position to;
	bool capture;
	bool Castling;
	bool takeEnPassant;
	int Promotion; // 0 none / 1 Queen / 2 bishop / 3 knight
	int orderScore;
}; 



typedef struct LastGameStat {
	bool MovePieceHasMoved;/// add after the move, for unMove only
	int lastKillId;
	bool killedPieceHasMoved;
	int enPassantTakeId;
	bool enPassantTakeHasMoved;
	bool m_kingInCheck[2];
	
};

typedef struct TranspositionTable {
	int score;
	std::vector< Move> leafMove;
};

typedef struct gamestat {
	int nodes;
	int capture;
	int EnPassant;
	int promotions;
	int Castles;
	int checks;
	std::vector<Move> leafMove;
};

inline bool operator==(const position& lhs, const position& rhs)
{
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

inline bool operator<(const Move& lhs, const Move& rhs)
{
	return lhs.orderScore > rhs.orderScore;
}

inline bool operator==(const Move& lhs, const Move& rhs)
{
	return(lhs.from == rhs.from && lhs.to == rhs.to && lhs.capture == rhs.capture && lhs.Promotion == rhs.Promotion && lhs.takeEnPassant == rhs.takeEnPassant);

}


