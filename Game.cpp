#include "game.h"


Game::Game(std::string fen, unsigned long long keyArraySeed)
{
	m_stat.nodes = 0;
	m_stat.EnPassant = 0;
	m_stat.promotions = 0;
	m_nbrTour = 0;
	if (keyArraySeed == 0) {
		HashKey();
	}
	BoardStartup(fen);
	
}

Game::~Game()
{
}

void Game::BoardStartup(std::string fen)
{
	int ColIt = 0;
	int lignIt = 0;
	int PieceIt = 0;
	for (int it = 0; it < fen.size(); it++) {

		char currChar = fen[it];
		bool team;

		if (currChar >= 48 && currChar <= 56) {
			lignIt += (int)currChar - 48;
		}
		else {
			if (currChar >= 65 && currChar <= 90) {
				team = true;
				currChar = currChar + 32;
			}
			else team = false;
			if (currChar == '/') {
				ColIt++;
				lignIt = 0;
			}
			else {
				int PieceType;
				switch (currChar) {
				case ('k'):
					PieceType = 6;
					m_kingId[team] = PieceIt;
					break;
				case ('q'):
					PieceType = 5;
					break;
				case ('r'):
					PieceType = 4;
					break;
				case ('n'):
					PieceType = 3;
					break;
				case ('b'):
					PieceType = 2;
					break;
				case ('p'):
					PieceType = 1;
					break;
				}
				m_pieceList[PieceIt] = Piece(PieceType, PieceIt, team);
				m_board[ColIt][lignIt].placePiece(&m_pieceList[PieceIt], newPosition(ColIt, lignIt));
				PieceIt++;
				lignIt++;
			}
		}
	}
	m_kingInCheck[0] = checkCheckOnTile(m_pieceList[m_kingId[0]].getLocation(), 1);
	m_kingInCheck[1] = checkCheckOnTile(m_pieceList[m_kingId[1]].getLocation(), 0);
}

position Game::getCooFromPos(char col, char lig) //a7
{
	position coo;
	if (col < 'a' || col>'h' || lig < '1' || lig > '8') return newPosition(-1, -1);
	else {

		coo.x = 8 - (lig - '0');
		coo.y = (col - 'a');
	}
	return coo;
}

std::string Game::getPosFromCoo(position coo) {
	std::string pos = "a0";
	pos[0] = coo.y + 'a';
	pos[1] = +8 - coo.x + '0';
	return pos;

}


bool Game::checkMove(Move* move, int pieceType, bool pieceTeam)
{
	std::vector <Move> possibleMove = getPieceRange(move->from, pieceType, pieceTeam);
	for (auto cmpMove : possibleMove) {
		if (move->to.x == cmpMove.to.x && move->to.y == cmpMove.to.y) {
			*move = cmpMove;
			return true;
		}
	}
	return false;
}

bool Game::checkCheckOnTile(position tile, bool opteam) {

		//knight check detection ( factorisation possible with directional  vector
		if (tile.x + 1 <= 7 && tile.y + 2 <= 7) if (!m_board[tile.x + 1][tile.y + 2].isEmpty()) {
			if (m_pieceList[m_board[tile.x + 1][tile.y + 2].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + 1][tile.y + 2].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x + 2 <= 7 && tile.y + 1 <= 7) if (!m_board[tile.x + 2][tile.y + 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x + 2][tile.y + 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + 2][tile.y + 1].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x - 1 >= 0 && tile.y + 2 <= 7) if (!m_board[tile.x - 1][tile.y + 2].isEmpty()) {
			if (m_pieceList[m_board[tile.x - 1][tile.y + 2].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x - 1][tile.y + 2].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x - 2 >= 0 && tile.y + 1 <= 7) if (!m_board[tile.x - 2][tile.y + 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x - 2][tile.y + 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x - 2][tile.y + 1].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x + 1 <= 7 && tile.y - 2 >= 0) if (!m_board[tile.x + 1][tile.y - 2].isEmpty()) {
			if (m_pieceList[m_board[tile.x + 1][tile.y - 2].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + 1][tile.y - 2].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x + 2 <= 7 && tile.y - 1 >= 0) if (!m_board[tile.x + 2][tile.y - 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x + 2][tile.y - 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + 2][tile.y - 1].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x - 1 >= 0 && tile.y - 2 >= 0) if (!m_board[tile.x - 1][tile.y - 2].isEmpty()) {
			if (m_pieceList[m_board[tile.x - 1][tile.y - 2].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x - 1][tile.y - 2].getPieceId()].getType() == 3) {
				return true;
			}
		}
		if (tile.x - 2 >= 0 && tile.y - 1 >= 0) if (!m_board[tile.x - 2][tile.y - 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x - 2][tile.y - 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x - 2][tile.y - 1].getPieceId()].getType() == 3) {
				return true;
			}
		}
		// pawn check detection
		int deltaPawnTake;
		if (!opteam == 1) deltaPawnTake = -1;
		else deltaPawnTake = 1;
		if (tile.x + deltaPawnTake <= 7 && tile.x + deltaPawnTake >= 0 && tile.y + 1 <= 7) if (!m_board[tile.x + deltaPawnTake][tile.y + 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x + deltaPawnTake][tile.y + 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + deltaPawnTake][tile.y + 1].getPieceId()].getType() == 1) {
				return true;
			}
		}
		if (tile.x + deltaPawnTake >= 0 && tile.x + deltaPawnTake <= 7 && tile.y - 1 >= 0) if (!m_board[tile.x + deltaPawnTake][tile.y - 1].isEmpty()) {
			if (m_pieceList[m_board[tile.x + deltaPawnTake][tile.y - 1].getPieceId()].getTeam() == opteam && m_pieceList[m_board[tile.x + deltaPawnTake][tile.y - 1].getPieceId()].getType() == 1) {
				return true;
			}
		}

		/// sliding piece check detection 

		//std::vector<position> AllTileExplored;
		int directionVect[3] = { -1, 0, 1 };
		for (auto dX : directionVect) {
			for (auto dY : directionVect) {
				if (dX != 0 || dY != 0) {
					int x = tile.x;
					int y = tile.y;
					bool pieceFound = false;
					// check from to other king
					if (x + dX >= 0 && x + dX <= 7 && y + dY <= 7 && y + dY >= 0) {
						if (!m_board[x + dX][y + dY].isEmpty() && m_pieceList[m_board[x + dX][y + dY].getPieceId()].getTeam() == opteam && m_pieceList[m_board[x + dX][y + dY].getPieceId()].getType() == 6) return true;
					}
					while ((x + dX >= 0 && x + dX <= 7 && y + dY <= 7 && y + dY >= 0) && !pieceFound) {
						x += dX;
						y += dY;
						//AllTileExplored.push_back(newPosition(x, y));

						if (!m_board[x][y].isEmpty()) {
							pieceFound = true;
							if (m_pieceList[m_board[x][y].getPieceId()].getTeam() == opteam) {
								//std::cout << m_pieceList[m_board[x][y].getPieceId()].getType() << std::endl;

								if (dX == 0 || dY == 0) {
									if (m_pieceList[m_board[x][y].getPieceId()].getType() == 4 || m_pieceList[m_board[x][y].getPieceId()].getType() == 5) return true;
								}
								else if (m_pieceList[m_board[x][y].getPieceId()].getType() == 2 || m_pieceList[m_board[x][y].getPieceId()].getType() == 5) return true;
							}
						}
					}
				}
			}
		}

		return false;
}

bool Game::checkPrevision(Move move, bool team) /// check 
{
	bool returnValue;
	LastGameStat currGameStat;
	movePiece(move, team, &currGameStat);
	returnValue = getIfKingIsInCheck(team);
	UnmovePiece(move, team, currGameStat);
	return returnValue;

}

bool Game::checkForMat(bool team)
{

	bool findValid = false;
	for (auto piece : m_pieceList) {
		if (piece.getifAlive() && piece.getTeam() == team) {
			std::vector<Move> pieceRange = getPieceRange(piece.getLocation(), piece.getType(), team);
			for (auto PseudoValidMove : pieceRange) {
				if (!checkPrevision(PseudoValidMove, team)) {
					findValid = true;
					break;
				}
			}
			if (findValid) break;
		}
	}
	if (!findValid) {
		//std::cout << "CHECKMATE" << std::endl;
		return true;
	}

	int nbrPieceT0 = 0;
	int nbrPieceT1 = 0;
	for (auto piece : m_pieceList) {
		if (piece.getifAlive() == 1) {
			if (piece.getTeam() == team) nbrPieceT0++;
			else nbrPieceT1++;
		}
	}
	if (nbrPieceT0 == 1) {
		if (nbrPieceT1 == 1) {
			std::cout << "DRAW" << std::endl;
			return true;
		}
	}
	return false;


}

void Game::DisplayBoard() {

	for (int i = 0; i < 8; i++) {
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
		for (int j = 0; j < 8; j++) {
			std::cout << "| ";
			if (m_board[i][j].isEmpty()) {
				std::cout << " ";
			}
			else {
				int currPieceType = m_pieceList[m_board[i][j].getPieceId()].getType();
				int AsciiShift = 0;
				if (!m_pieceList[m_board[i][j].getPieceId()].getTeam()) AsciiShift = 32;
				if (currPieceType == 1) std::cout << char('P' + AsciiShift);
				else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
				else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
				else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
				else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
				else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
			}
			std::cout << " ";
		}
		std::cout << "| " << 8 - (i) << std::endl;

	}
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "  A   B   C   D   E   F   G   H  " << std::endl << std::endl;
	std::cout << "liste des pieces hors terrain : ";
	for (auto pieceDead : m_pieceList) {
		int currPieceType = pieceDead.getType();
		if (!pieceDead.getifAlive() && currPieceType != -1) {
			int AsciiShift = 0;
			if (!pieceDead.getTeam()) AsciiShift = 32;
			if (currPieceType == 1) std::cout << char('P' + AsciiShift);
			else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
			else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
			else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
			else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
			else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

void Game::DisplayBoardOverlay(std::vector<Move> validMove) {
	for (int i = 0; i < 8; i++) {
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
		for (int j = 0; j < 8; j++) {
			std::cout << "| ";
			if (m_board[i][j].isEmpty()) {
				std::cout << " ";
			}
			else {
				int currPieceType = m_pieceList[m_board[i][j].getPieceId()].getType();
				int AsciiShift = 0;
				if (!m_pieceList[m_board[i][j].getPieceId()].getTeam()) AsciiShift = 32;
				if (currPieceType == 1) std::cout << char('P' + AsciiShift);
				else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
				else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
				else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
				else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
				else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
			}
			bool findValidPos = false;
			for (auto move : validMove) {
				if (move.to.x == i && move.to.y == j) {
					std::cout << "O";
					findValidPos = true;
					break;
				}
			}
			if (!findValidPos) std::cout << " ";
		}
		std::cout << "| " << 8 - (i) << std::endl;

	}
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "  A   B   C   D   E   F   G   H  " << std::endl << std::endl;
	std::cout << "liste des pieces hors terrain : ";
	for (auto pieceDead : m_pieceList) {
		int currPieceType = pieceDead.getType();
		if (!pieceDead.getifAlive() && currPieceType != -1) {
			int AsciiShift = 0;
			if (!pieceDead.getTeam()) AsciiShift = 32;
			if (currPieceType == 1) std::cout << char('P' + AsciiShift);
			else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
			else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
			else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
			else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
			else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

unsigned long long Game::HashFunction() {
	unsigned long long hash = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!m_board[i][j].isEmpty()) {
				int pieceId = m_pieceList[m_board[i][j].getPieceId()].getType() - 1;
				if (m_pieceList[m_board[i][j].getPieceId()].getTeam()) pieceId += 6;
				hash ^= m_keyArray[pieceId][i][j];
			}
		}
	}
	return hash;
}
