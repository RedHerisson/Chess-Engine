#include "game.h"


Game::Game()
	: m_antiRecursion(0)
{
	m_stat.nodes = 0;
	m_stat.EnPassant = 0; 
	m_stat.promotions = 0;
	m_nbrTour = 0;
}

Game::~Game()
{
}

Game::Game(const Game& gOld)
{
	m_stat.nodes = 0;
	m_antiRecursion = gOld.m_antiRecursion;
	m_enPassant = gOld.m_enPassant;
	m_nbrTour = gOld.m_nbrTour;
	m_kingInCheck[0] = gOld.m_kingInCheck[0];
	m_kingInCheck[1] = gOld.m_kingInCheck[1];
	std::copy(std::begin(gOld.kingId), std::end(gOld.kingId), std::begin(kingId));
	std::copy(std::begin(gOld.PieceList), std::end(gOld.PieceList), std::begin(PieceList));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_board[i][j] = gOld.m_board[i][j];
		}
	}
	
}

void Game::addStat(gamestat stat)
{
	m_stat.nodes += stat.nodes;
	m_stat.capture += stat.capture;
	m_stat.EnPassant += stat.EnPassant;
	m_stat.promotions += stat.promotions;
	m_stat.Castles += stat.Castles;
	m_stat.checks += stat.Castles;
}



void Game::BoardStartup()
{
	std::string boardDisposition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	int ColIt = 0;
	int lignIt = 0;
	int PieceIt = 0;
	for (int it = 0; it < boardDisposition.size(); it++) {
		
		char currChar = boardDisposition[it];
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
					kingId[team] = PieceIt;
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
				PieceList[PieceIt] = Piece(PieceType, PieceIt, team);
				m_board[ColIt][lignIt].placePiece(&PieceList[PieceIt],ColIt, lignIt);
				PieceIt++;
				lignIt++;
			}
		}
	}
	m_kingInCheck[0] = checkCheckOnTile(PieceList[kingId[0]].getLocation(), 1);
	m_kingInCheck[1] = checkCheckOnTile(PieceList[kingId[1]].getLocation(), 0);
}

std::pair<position, position >  Game::userMoveCmd(bool team) {
	do {
		std::string moveCmd;

		std::cin.clear();
		std::cin >> moveCmd; // a7a6
		if (moveCmd.size() == 4) {
			position CooStart = getCooFromPos(moveCmd[0], moveCmd[1]);
			position CooEnd = getCooFromPos(moveCmd[2], moveCmd[3]);
			if (!m_board[CooStart.first][CooStart.second].isEmpty()) {
				
				if (PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam() == team) {
					if (checkMove(CooStart, CooEnd, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam())) {
						
						if (!checkPrevision(PieceList[kingId[team]].getLocation(), !team, CooStart, CooEnd)) {
							
							return std::make_pair(CooStart, CooEnd);
						}
						else std::cout << "mouvement illegal, mise en echec de votre roi" << std::endl;
					}
					else std::cout << "deplacement impossible" << std::endl;

				}
				else std::cout << "le pion n'est pas a vous" << std::endl;

			}
			else {
				std::cout << "error deplacement impossible" << std::endl;
			}
		}
		else if (moveCmd.size() == 2) {

			position CooStart = getCooFromPos(moveCmd[0], moveCmd[1]);
			std::cout << m_board[CooStart.first][CooStart.second].isEmpty() << std::endl;
			if (!m_board[CooStart.first][CooStart.second].isEmpty()) {
				std::vector<position > validPos;
				validPos = ValidCooByType(CooStart, PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType(), PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getTeam());
				m_antiRecursion = 0; /// only for aff, reset flag after 
				DisplayBoardOverlay(validPos);
			}
			else {
				std::cout << "error deplacement impossible" << std::endl;
			}
		}
		else {
			std::cout << "commande de deplacement incorrect, format a1a2 acceptee ou a1 pour previsualiser" << std::endl;
		}
	} while (1);
	
}

std::pair<position, position > Game::MoveSelection(bool team) {

	if (team) {
		if (PLAYER1_IS_IA) return randomMoveCmd(team);
		else return userMoveCmd(team);
	}
	else {

	}
	if(PLAYER0_IS_IA) return randomMoveCmd(team);
	else return userMoveCmd(team);
}



void Game::movePiece(int PlayerId, position CooStart, position CooEnd, bool realMove) {

	if (m_enPassant.Available && m_enPassant.team == PlayerId) m_enPassant.Available = 0; // enPassant ne dure qu'un seul tour, on le retire après

	if (realMove) m_antiRecursion = 0; /// if real movement reset antiRecursion flag
	if (!m_board[CooEnd.first][CooEnd.second].isEmpty()) PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].kill();

	if (m_enPassant.Available) {
		if (PieceList[m_board[CooStart.first][CooStart.second].getPieceId()].getType() == 1) {
			if (m_enPassant.ligne == CooEnd.second && (2 + (!PlayerId) * 3) == CooEnd.first) {
				if (!m_board[(3 + (!PlayerId) * 1)][m_enPassant.ligne].isEmpty()) PieceList[m_board[(3 + (!PlayerId) * 1)][m_enPassant.ligne].getPieceId()].kill();
				m_board[(3 + (!PlayerId) * 1)][m_enPassant.ligne].setEmpty();

			}
		}
	}

	m_board[CooEnd.first][CooEnd.second].placePiece(&PieceList[m_board[CooStart.first][CooStart.second].getPieceId()], CooEnd.first, CooEnd.second);
	m_board[CooStart.first][CooStart.second].setEmpty();
	if (PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].getType() == 1 && abs(CooStart.first - CooEnd.first) == 2) {
		enPassant newEnPassant;
		newEnPassant.team == PlayerId;
		newEnPassant.Available = true;
		newEnPassant.ligne = CooEnd.second; 

	}
	///
	if (PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].getType() == 6 && abs(CooStart.second - CooEnd.second) == 2) {
		bool castlingType;
		int x;
		if (PlayerId == 0) x = 0;
		else x = 7;
		if (CooStart.second - CooEnd.second > 0) {/// short
			m_board[x][3].placePiece(&PieceList[m_board[x][0].getPieceId()], x, 3);
			m_board[x][0].setEmpty();

		}
		else {
			m_board[x][5].placePiece(&PieceList[m_board[x][7].getPieceId()], x, 5);
			m_board[x][7].setEmpty();
		}
		m_stat.Castles++;

	}
	m_kingInCheck[0] = checkCheckOnTile(PieceList[kingId[0]].getLocation(),1);
	m_kingInCheck[1] = checkCheckOnTile(PieceList[kingId[1]].getLocation(), 0);

	//std::cout << "team 0: " << m_kingInCheck[!PlayerId] << std::endl;


	/// PROMOTION PAWN TO QUEEN
	if (CooEnd.first == (!PlayerId)*7) {
		if (PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].getType() == 1) {
			PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].setType(5);
			if(!realMove) PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].setWeight(2000);
		}
	}



}

position Game::getCooFromPos(char col, char lig)
{
	position Coo;
	if (col < 'a' || col>'h' || lig < '1' ||lig > '8') return newPosition(-1, -1);
	else {
		
		Coo.first = 8 - (lig - '0'); 
		Coo.second = (col - 'a'); 
	}
	return Coo;
}

std::string Game::getPosFromCoo(position coo) {
	std::string pos = "a0";
	pos[0] = coo.second + 'a';
	pos[1] = + 8 -  coo.first + '0';
	return pos;

}

position Game::newPosition(int a, int b)
{
	position newPos;
	newPos.first = a;
	newPos.second = b;
	return newPos;
}

bool Game::checkMove(position cooStart, position cooEnd, int pieceType, bool pieceTeam)
{
	std::vector <position > possibleMove = ValidCooByType(cooStart, pieceType, pieceTeam);
	for (auto CooOk : possibleMove) {
		if (cooEnd.first == CooOk.first && cooEnd.second == CooOk.second) {
			return true;
		}
	}
	return false;
}



bool Game::isSameTeam(position cooStart, position cooEnd)
{
	if (!m_board[cooEnd.first][cooEnd.second].isEmpty()) {
		if (PieceList[m_board[cooEnd.first][cooEnd.second].getPieceId()].getTeam() == PieceList[m_board[cooStart.first][cooStart.second].getPieceId()].getTeam()) return true;
	}
	return false;
}

bool Game::checkCheckOnTile(position coo, bool opteam)
{
	//knight check detection
	if (coo.first + 1 <= 7 && coo.second + 2 <= 7) if (!m_board[coo.first + 1][coo.second + 2].isEmpty()) {
		if (PieceList[m_board[coo.first + 1][coo.second + 2].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + 1][coo.second + 2].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first + 2 <= 7 && coo.second + 1 <= 7) if (!m_board[coo.first + 2][coo.second + 1].isEmpty()) {
		if (PieceList[m_board[coo.first + 2][coo.second + 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + 2][coo.second + 1].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first - 1 >= 0 && coo.second + 2 <= 7) if (!m_board[coo.first - 1][coo.second + 2].isEmpty()) {
		if (PieceList[m_board[coo.first - 1][coo.second + 2].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first - 1][coo.second + 2].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first - 2 >= 0 && coo.second + 1 <= 7) if (!m_board[coo.first - 2][coo.second + 1].isEmpty()) {
		if (PieceList[m_board[coo.first - 2][coo.second + 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first - 2][coo.second + 1].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first + 1 <= 7 && coo.second - 2 >= 0) if (!m_board[coo.first + 1][coo.second - 2].isEmpty()) {
		if (PieceList[m_board[coo.first + 1][coo.second - 2].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + 1][coo.second - 2].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first + 2 <= 7 && coo.second - 1 >= 0) if (!m_board[coo.first + 2][coo.second - 1].isEmpty()) {
		if (PieceList[m_board[coo.first + 2][coo.second - 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + 2][coo.second - 1].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first - 1 >= 0 && coo.second - 2 >= 0) if (!m_board[coo.first - 1][coo.second - 2].isEmpty()) {
		if (PieceList[m_board[coo.first - 1][coo.second - 2].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first - 1][coo.second - 2].getPieceId()].getType() == 3) {
			return true;
		}
	}
	if (coo.first - 2 >= 0 && coo.second - 1 >= 0) if (!m_board[coo.first - 2][coo.second - 1].isEmpty()) {
		if (PieceList[m_board[coo.first - 2][coo.second - 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first - 2][coo.second - 1].getPieceId()].getType() == 3) {
			return true;
		}
	}
	// pawn check detection
	int deltaPawnTake;
	if (!opteam == 1) deltaPawnTake = -1;
	else deltaPawnTake = 1;
	if (coo.first + deltaPawnTake <= 7 && coo.first + deltaPawnTake >=0 && coo.second + 1 <= 7) if (!m_board[coo.first + deltaPawnTake][coo.second + 1].isEmpty()) {
		if (PieceList[m_board[coo.first + deltaPawnTake][coo.second + 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + deltaPawnTake][coo.second + 1].getPieceId()].getType() == 1) {
			return true;
		}
	}
	if (coo.first + deltaPawnTake >= 0 && coo.first + deltaPawnTake <= 7 && coo.second - 1 >= 0) if(!m_board[coo.first + deltaPawnTake][coo.second - 1].isEmpty()) {
		if (PieceList[m_board[coo.first + deltaPawnTake][coo.second - 1].getPieceId()].getTeam() == opteam && PieceList[m_board[coo.first + deltaPawnTake][coo.second - 1].getPieceId()].getType() == 1) {
			return true;
		}
	}
	
	//std::vector<position> AllTileExplored;
	int directionVect[3] = { -1, 0, 1 };
	for (auto dX : directionVect) {
		for (auto dY : directionVect) {
			if (dX != 0 || dY != 0) {
				int x = coo.first;
				int y = coo.second;
				bool pieceFound = false;
				// king check
				if (x + dX >= 0 && x + dX <= 7 && y + dY <= 7 && y + dY >= 0) {
					if (!m_board[x + dX][y + dY].isEmpty() && PieceList[m_board[x + dX][y + dY].getPieceId()].getTeam() == opteam && PieceList[m_board[x + dX][y + dY].getPieceId()].getType() == 6) return true;
				}
				while ((x + dX >= 0 && x + dX <= 7 && y + dY <= 7 && y + dY >= 0) && !pieceFound) {
					x += dX;
					y += dY;
					//AllTileExplored.push_back(newPosition(x, y));

					if (!m_board[x][y].isEmpty()) {
						pieceFound = true;
						if (PieceList[m_board[x][y].getPieceId()].getTeam() == opteam) {
							//std::cout << PieceList[m_board[x][y].getPieceId()].getType() << std::endl;

							if (dX == 0 || dY == 0) {
								if(PieceList[m_board[x][y].getPieceId()].getType() == 4 || PieceList[m_board[x][y].getPieceId()].getType() == 5) return true;
							} 
							else if (PieceList[m_board[x][y].getPieceId()].getType() == 2 || PieceList[m_board[x][y].getPieceId()].getType() == 5) return true;
						}
					}
				}
			}
			
			
			
		}
	}
	
	return false;






	/*
	for (auto piece : PieceList) {
		
		if (piece.getTeam() == opteam && piece.getifAlive() == 1) {
			if (checkMove(piece.getLocation(),coo, piece.getType(), opteam)) {

				return true;
			}
		}
	}
	*/
	return false;
}

bool Game::checkPrevision(position coo, bool Opteam, position cooStart, position cooEnd) /// check 
{
	int tempoPieceId;
	bool returnValue;
	Game tGame = *this;
	tGame.movePiece(!Opteam, cooStart, cooEnd,0);
	
	returnValue = tGame.getIfKingIsInCheck(!Opteam);
	tGame.~Game();
	return returnValue;

}

bool Game::checkForMat(bool team)
{

	bool findValid = false;
	for (auto piece : PieceList) {
		if (piece.getifAlive() && piece.getTeam() == team) {
			std::vector<position> validCooList = ValidCooByType(piece.getLocation(), piece.getType(), team);
			for (auto coo : validCooList) {
				if (!checkPrevision(piece.getLocation(), !team, piece.getLocation(), coo)) {
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
	for (auto piece : PieceList) {
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

std::vector<position> Game::checkForCastling(bool team) {
	std::vector < position> rookCoo;
	int x;
	if (team == 0) x = 0;
	else x = 7;
	
	if (!m_board[x][4].isEmpty()) {
		
		if (!PieceList[m_board[x][4].getPieceId()].getIfHasMoved()) {
			if (!m_kingInCheck[team]) {
				/// test for short Castling
				
				if(m_board[x][5].isEmpty() && m_board[x][6].isEmpty()) {
					
					if (!checkCheckOnTile(newPosition(x, 5), !team) &&  !checkCheckOnTile(newPosition(x, 6), !team)) {
						if (!m_board[x][7].isEmpty()) {
							if (!PieceList[m_board[x][7].getPieceId()].getIfHasMoved()) {
								rookCoo.push_back(newPosition(x, 6));
							}
						}	
					}
				}
				/// test for Long Castling
				if (m_board[x][3].isEmpty() && m_board[x][2].isEmpty() && m_board[x][1].isEmpty()) {
					if (!checkCheckOnTile(newPosition(x, 3), !team) && !checkCheckOnTile(newPosition(x, 2), !team)) {
						if (!m_board[x][0].isEmpty()) {
							if (!PieceList[m_board[x][0].getPieceId()].getIfHasMoved()) {
								rookCoo.push_back(newPosition(x, 2));
							}
						}
					}
				}
			}	
		}
	}
	


	return rookCoo;

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
					int currPieceType = PieceList[m_board[i][j].getPieceId()].getType();
					int AsciiShift = 0;
					if (!PieceList[m_board[i][j].getPieceId()].getTeam()) AsciiShift = 32;
					if (currPieceType == 1) std::cout << char('P' + AsciiShift);
					else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
					else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
					else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
					else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
					else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
				}
				std::cout << " ";
			}
			std::cout << "| " << 8- (i) <<  std::endl;
		
	}
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "  A   B   C   D   E   F   G   H  " << std::endl << std::endl;
	std::cout << "liste des pieces hors terrain : ";
	for (auto pieceDead : PieceList) {
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

void Game::DisplayBoardOverlay(std::vector<position> validPos) {
	for (int i = 0; i < 8; i++) {
		std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
		for (int j = 0; j < 8; j++) {
			std::cout << "| ";
			if (m_board[i][j].isEmpty()) {
				std::cout << " ";
			}
			else {
				int currPieceType = PieceList[m_board[i][j].getPieceId()].getType();
				int AsciiShift = 0;
				if (!PieceList[m_board[i][j].getPieceId()].getTeam()) AsciiShift = 32;
				if (currPieceType == 1) std::cout << char('P' + AsciiShift);
				else if (currPieceType == 2) std::cout << char('B' + AsciiShift);
				else if (currPieceType == 3) std::cout << char('N' + AsciiShift);
				else if (currPieceType == 4) std::cout << char('R' + AsciiShift);
				else if (currPieceType == 5) std::cout << char('Q' + AsciiShift);
				else if (currPieceType == 6) std::cout << char('K' + AsciiShift);
			}
			bool findValidPos = false;
			for (auto coo : validPos) {
				if (coo.first == i && coo.second == j) {
					std::cout << "O";
					findValidPos = true;
					break;
				}
			}
			if(!findValidPos) std::cout << " ";
		}
		std::cout << "| " << 8 - (i) << std::endl;

	}
	std::cout << "+---+---+---+---+---+---+---+---+" << std::endl;
	std::cout << "  A   B   C   D   E   F   G   H  " << std::endl << std::endl;
	std::cout << "liste des pieces hors terrain : ";
	for (auto pieceDead : PieceList) {
		int currPieceType = pieceDead.getType();
		if (!pieceDead.getifAlive() && currPieceType!=-1) {
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

void Game::HashKey(unsigned long long keyArray[12][8][8]) {
	srand(5465465);
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				keyArray[i][j][k] = rand64bits();
			}
		}
	}
}

int Game::hashMove(std::pair<position, position> move)
{
	return move.first.first + move.first.second * 10 + move.second.first * 100 + move.second.second * 1000;
}

unsigned long long Game::HashFunction(unsigned long long keyArray[12][8][8]) {
	unsigned long long hash = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (!m_board[i][j].isEmpty()) {
				int pieceId = PieceList[m_board[i][j].getPieceId()].getType()-1;
				if (PieceList[m_board[i][j].getPieceId()].getTeam()) pieceId += 6;
				hash ^= keyArray[pieceId][i][j];
			}
		}
	}
	return hash;
}

unsigned Game::rand256()
{
	static unsigned const limit = RAND_MAX - RAND_MAX % 256;
	unsigned result = rand();
	while (result >= limit) {
		result = rand();
	}
	return result % 256;
}

unsigned long long Game::rand64bits()
{
	unsigned long long results = 0ULL;
	for (int count = 8; count > 0; --count) {
		results = 256U * results + rand256();
	}
	return results;
}
