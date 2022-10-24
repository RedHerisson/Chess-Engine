#include "Game.h"

void Game::movePiece(Move move, bool team, LastGameStat* LastGameStat) {

	if (m_enPassant.Available && m_enPassant.team == team) m_enPassant.Available = 0; // enPassant ne dure qu'un seul tour, on le retire après

	LastGameStat->MovePieceHasMoved = m_pieceList[m_board[move.from.x][move.from.y].getPieceId()].getIfHasMoved();

	if (move.capture) {
		LastGameStat->killedPieceHasMoved = m_pieceList[m_board[move.to.x][move.to.y].getPieceId()].getIfHasMoved();
		LastGameStat->lastKillId = m_board[move.to.x][move.to.y].getPieceId();
		m_pieceList[m_board[move.to.x][move.to.y].getPieceId()].kill(); // if the move is a capture, kill the piece taken
	}

	m_board[move.to.x][move.to.y].placePiece(&m_pieceList[m_board[move.from.x][move.from.y].getPieceId()], move.to); /// piece move
	m_board[move.from.x][move.from.y].setEmpty(); // clear start move

	///creation of a new enPassant possibility if a pawn move two tile forward
	if (m_pieceList[m_board[move.to.x][move.to.y].getPieceId()].getType() == 1 && abs(move.from.x - move.to.x) == 2) {
		enPassant newEnPassant;
		newEnPassant.team == team;
		newEnPassant.Available = true;
		newEnPassant.ligne = move.to.y;

	}

	/// if enPassant, then we kill the pawn taken
	if (move.takeEnPassant) {
		m_pieceList[m_board[(move.to.x + (!team) * 2 - 1)][move.to.y].getPieceId()].kill();
		m_board[(move.to.x + (!team) * 1)][move.to.y].setEmpty();
		LastGameStat->enPassantTakeHasMoved = m_pieceList[m_board[(move.to.x + (!team) * 2 - 1)][move.to.y].getPieceId()].getIfHasMoved();
		LastGameStat->enPassantTakeId = m_board[(move.to.x + (!team) * 2 - 1)][move.to.y].getPieceId();
	}

	/// if castling, we move the rook also
	if (move.Castling) {
		int x;
		if (team == BLACK) x = 0;
		else x = 7;
		if (move.from.y - move.to.y > 0) {/// short
			m_board[x][3].placePiece(&m_pieceList[m_board[x][0].getPieceId()], newPosition(x, 3));
			m_board[x][0].setEmpty();

		}
		else {
			m_board[x][5].placePiece(&m_pieceList[m_board[x][7].getPieceId()], newPosition(x, 5));
			m_board[x][7].setEmpty();
		}
		m_stat.Castles++;

	}

	// update king check stat;
	m_kingInCheck[0] = checkCheckOnTile(m_pieceList[m_kingId[0]].getLocation(), 1);
	m_kingInCheck[1] = checkCheckOnTile(m_pieceList[m_kingId[1]].getLocation(), 0);

	//std::cout << "team 0: " << m_kingInCheck[!PlayerId] << std::endl;


	/// PROMOTION PAWN TO QUEEN
	if (move.Promotion) {
		m_pieceList[m_board[move.to.x][move.to.y].getPieceId()].setType(5);
		//if (!realMove) PieceList[m_board[CooEnd.first][CooEnd.second].getPieceId()].setWeight(2000); needed in the futur
	}
}

void Game::UnmovePiece(Move lastMove, bool team, LastGameStat LastGameStat) {

	m_board[lastMove.from.x][lastMove.from.y].placePiece(&m_pieceList[m_board[lastMove.to.x][lastMove.to.y].getPieceId()], lastMove.from); // piece move
	m_board[lastMove.to.x][lastMove.to.y].setEmpty(); // clear start move
	m_pieceList[m_board[lastMove.from.x][lastMove.from.x].getPieceId()].setHasMoved(0);//LastGameStat.MovePieceHasMoved);

	if (lastMove.capture) {
		m_pieceList[LastGameStat.lastKillId].revive();
		m_board[lastMove.to.x][lastMove.to.y].placePiece(&m_pieceList[LastGameStat.lastKillId], lastMove.to); // piece move
		m_pieceList[m_board[lastMove.from.x][lastMove.from.x].getPieceId()].setHasMoved(LastGameStat.killedPieceHasMoved);

	}


	// remove last promotion if it happended
	if (lastMove.Promotion) {
		m_pieceList[m_board[lastMove.to.x][lastMove.to.y].getPieceId()].setType(1);
	}

	/// move rook back in place if castling has happended
	if (lastMove.Castling) {

		int x;
		if (team == BLACK) x = 0;
		else x = 7;
		if(m_pieceList[m_kingId[team]].getLocation().y == 2) {
			m_board[x][0].placePiece(&m_pieceList[m_board[x][3].getPieceId()], newPosition(x, 3));
			m_board[x][3].setEmpty();
			m_pieceList[m_board[lastMove.from.x][lastMove.from.x].getPieceId()].setHasMoved(0);

		}
		else {
			m_board[x][7].placePiece(&m_pieceList[m_board[x][5].getPieceId()], newPosition(x, 5));
			m_board[x][5].setEmpty();
			m_pieceList[m_board[lastMove.from.x][lastMove.from.x].getPieceId()].setHasMoved(0);

		}
	}

	/// if enPassant, put the pawn back in place
	if (lastMove.takeEnPassant) {
		m_pieceList[LastGameStat.enPassantTakeId].revive();
		m_board[m_pieceList[LastGameStat.enPassantTakeId].getLocation().x][m_pieceList[LastGameStat.enPassantTakeId].getLocation().y].placePiece(&m_pieceList[LastGameStat.enPassantTakeId], m_pieceList[LastGameStat.enPassantTakeId].getLocation()); // piece move
		m_board[m_pieceList[LastGameStat.enPassantTakeId].getLocation().x][m_pieceList[LastGameStat.enPassantTakeId].getLocation().y].setFull();
		m_pieceList[m_board[m_pieceList[LastGameStat.enPassantTakeId].getLocation().x][m_pieceList[LastGameStat.enPassantTakeId].getLocation().y].getPieceId()].setHasMoved(LastGameStat.enPassantTakeHasMoved);
		
	}


	
}


std::vector<Move> Game::getPieceRange(position from, int pieceType, bool pieceTeam) {
	std::vector <Move> possibleMove;
	//std::cout << pieceType << std::endl;
	if (pieceType == 1) possibleMove = checkPawnMove(from, pieceTeam);
	if (pieceType == 2) possibleMove = checkBishopMove(from);
	if (pieceType == 3) possibleMove = checkKnightMove(from);
	if (pieceType == 4) possibleMove = checkRookMove(from);
	if (pieceType == 5) possibleMove = checkQueenMove(from);
	if (pieceType == 6) possibleMove = checkKingMove(from, pieceTeam);
	return possibleMove;
}
/// <summary>
/// generation of the list of move for a specific pawn
/// </summary>
/// <param name="from"> 
/// current pawn tile
/// </param>
/// <param name="team">
/// team of the pawn selected
/// </param>
/// <returns></returns>
std::vector<Move> Game::checkPawnMove(position from, bool team)
{
	std::vector<Move> moveMap;
	
	if (team == WHITE) { 
		if (from.x == 6) { /// two tile move possible at start position
			if (m_board[from.x - 2][from.y].isEmpty() && m_board[from.x - 1][from.y].isEmpty()) { /// only if both tile are empty
				moveMap.push_back(newMove(from, newPosition(from.x - 2, from.y), 0, 0,0,0));
			}
		}
		// pawn forward move, cannot take another piece
		if (from.x - 1 >= 0) if (m_board[from.x - 1][from.y].isEmpty()) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y), 0, 0, 0, 0));
		
		// pawn take move, always take piece
		if (from.x - 1 >= 0 && from.y + 1 <= 7)if ((!isSameTeam(from, newPosition(from.x - 1, from.y + 1))) && !m_board[from.x - 1][from.y + 1].isEmpty()) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y + 1), 1, 0, 0, 0));
		if (from.x - 1 >= 0 && from.y - 1 >= 0)if (!isSameTeam(from, newPosition(from.x - 1, from.y - 1)) && !m_board[from.x - 1][from.y - 1].isEmpty()) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y - 1), 1, 0, 0, 0));                      
		
		// if an enPassant is disponible, add it to the vector
		if (m_enPassant.Available) { 
			if (from.y == m_enPassant.ligne + 1 || from.y == m_enPassant.ligne - 1) {
				if (from.x == 4) {
					moveMap.push_back(newMove(from, newPosition(5, m_enPassant.ligne), 0, 0, 0, 1)); /// en passant to 1 
				}

			}
		}
	}
	else { // same for the other team (should be factorised in the futur)
		if (from.x == 1) {
			if (m_board[from.x + 2][from.y].isEmpty() && m_board[from.x + 1][from.y].isEmpty()) {
				moveMap.push_back(newMove(from, newPosition(from.x + 2, from.y), 0, 0, 0, 0));
			}
		}
		if (from.x + 1 <= 7) if (m_board[from.x + 1][from.y].isEmpty())  moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y), 0, 0, 0, 0));

		if (from.x + 1 <= 7 && from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x + 1, from.y + 1)) && !m_board[from.x + 1][from.y + 1].isEmpty()) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y + 1), 1, 0, 0, 0));
		if (from.x + 1 <= 7 && from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x + 1, from.y - 1)) && !m_board[from.x + 1][from.y - 1].isEmpty()) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y - 1), 1, 0, 0, 0));
		if (m_enPassant.Available) {
			if (from.y == m_enPassant.ligne + 1 || from.y == m_enPassant.ligne - 1) {
				if (from.x == 3) {
					moveMap.push_back(newMove(from, newPosition(2, m_enPassant.ligne), 0, 0, 0, 1)); /// en passant to 1 

				}

			}
		}
	}



	return moveMap;
}
/// <summary>
/// generation of the list of move for a specific bishop
/// </summary>
/// <param name="from"></param>
/// <returns></returns>
std::vector<Move> Game::checkBishopMove(position from)
{

	// the code should be factorised, by using directional vector instead of case

	std::vector<Move> moveMap;
	int x = from.x;
	int y = from.y;
	bool pieceFound = false;
	while (x != 0 && y != 0 && !pieceFound) {
		x--;
		y--;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(x, y))) moveMap.push_back(newMove(from, newPosition(x, y), pieceFound, 0, 0, 0));

	}

	x = from.x;
	y = from.y;
	pieceFound = false;
	while (x != 7 && y != 0 && !pieceFound) {
		x++;
		y--;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(x, y))) moveMap.push_back(newMove(from, newPosition(x, y), pieceFound, 0, 0, 0));
	}
	x = from.x;
	y = from.y;
	pieceFound = false;
	while (x != 0 && y != 7 && !pieceFound) {
		x--;
		y++;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(x, y))) moveMap.push_back(newMove(from, newPosition(x, y), pieceFound, 0, 0, 0));
	}

	x = from.x;
	y = from.y;
	pieceFound = false;
	while (x != 7 && y != 7 && !pieceFound) {
		x++;
		y++;
		if (!m_board[x][y].isEmpty())  pieceFound = true;
		if (!isSameTeam(from, newPosition(x, y))) moveMap.push_back(newMove(from, newPosition(x, y), pieceFound, 0, 0, 0));

	}

	return moveMap;
}

std::vector<Move> Game::checkKnightMove(position from)
{
	std::vector<Move> moveMap;
	if (from.x + 1 <= 7 && from.y + 2 <= 7) if (!isSameTeam(from, newPosition(from.x + 1, from.y + 2))) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y + 2), !m_board[from.x + 1][from.y + 2].isEmpty(), 0, 0, 0));
	if (from.x + 2 <= 7 && from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x + 2, from.y + 1))) moveMap.push_back(newMove(from, newPosition(from.x + 2, from.y + 1), !m_board[from.x + 2][from.y + 1].isEmpty(), 0, 0, 0));

	if (from.x - 1 >= 0 && from.y + 2 <= 7) if (!isSameTeam(from, newPosition(from.x - 1, from.y + 2))) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y + 2), !m_board[from.x - 1][from.y + 2].isEmpty(), 0, 0, 0));
	if (from.x - 2 >= 0 && from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x - 2, from.y + 1))) moveMap.push_back(newMove(from, newPosition(from.x - 2, from.y + 1), !m_board[from.x - 2][from.y + 1].isEmpty(), 0, 0, 0));

	if (from.x + 1 <= 7 && from.y - 2 >= 0) if (!isSameTeam(from, newPosition(from.x + 1, from.y - 2))) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y - 2), !m_board[from.x + 1][from.y - 2].isEmpty(), 0, 0, 0));
	if (from.x + 2 <= 7 && from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x + 2, from.y - 1))) moveMap.push_back(newMove(from, newPosition(from.x + 2, from.y - 1), !m_board[from.x + 2][from.y - 1].isEmpty(), 0, 0, 0));

	if (from.x - 1 >= 0 && from.y - 2 >= 0) if (!isSameTeam(from, newPosition(from.x - 1, from.y - 2))) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y - 2), !m_board[from.x - 1][from.y - 2].isEmpty(), 0, 0, 0));
	if (from.x - 2 >= 0 && from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x - 2, from.y - 1))) moveMap.push_back(newMove(from, newPosition(from.x - 2, from.y - 1), !m_board[from.x - 2][from.y - 1].isEmpty(), 0, 0, 0));

	return moveMap;

}

std::vector<Move> Game::checkRookMove(position from)
{
	std::vector<Move> moveMap;
	int xmin = from.x;
	bool pieceFound = false;
	while (xmin != 0 && !pieceFound) {
		xmin--;
		if (!m_board[xmin][from.y].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(xmin, from.y))) moveMap.push_back(newMove(from, newPosition(xmin, from.y), pieceFound, 0, 0, 0));

	}
	int xmax = from.x;
	pieceFound = false;
	while (xmax != 7 && !pieceFound) {
		xmax++;
		if (!m_board[xmax][from.y].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(xmax, from.y)))moveMap.push_back(newMove(from, newPosition(xmax, from.y), pieceFound, 0, 0, 0));

	}
	int ymin = from.y;
	pieceFound = false;
	while (ymin != 0 && !pieceFound) {
		ymin--;
		if (!m_board[from.x][ymin].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(from.x, ymin))) moveMap.push_back(newMove(from, newPosition(from.x, ymin), pieceFound, 0, 0, 0));
	}
	int ymax = from.y;
	pieceFound = false;
	while (ymax != 7 && !pieceFound) {
		ymax++;
		if (!m_board[from.x][ymax].isEmpty()) pieceFound = true;
		if (!isSameTeam(from, newPosition(from.x, ymax)))  moveMap.push_back(newMove(from, newPosition(from.x, ymax), pieceFound, 0, 0, 0));

	}
	return moveMap;
}

std::vector<Move> Game::checkQueenMove(position from)
{
	std::vector<Move> moveMap1, moveMap2;
	moveMap1 = checkBishopMove(from);
	moveMap2 = checkRookMove(from);
	moveMap1.insert(moveMap1.end(), moveMap2.begin(), moveMap2.end());
	return moveMap1;
}

std::vector<Move> Game::checkKingMove(position from, bool team)
{
	std::vector<Move> moveMap;
	if (from.x - 1 >= 0 && from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x - 1, from.y - 1))) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y - 1), !m_board[from.x - 1][from.y - 1].isEmpty(), 0, 0, 0));
	
	if (from.x + 1 <= 7 && from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x + 1, from.y + 1))) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y + 1), !m_board[from.x + 1][from.y + 1].isEmpty(), 0, 0, 0));
	if (from.x - 1 >= 0 && from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x - 1, from.y + 1))) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y + 1), !m_board[from.x - 1][from.y + 1].isEmpty(), 0, 0, 0));
	if (from.x + 1 <= 7 && from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x + 1, from.y - 1))) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y - 1), !m_board[from.x + 1][from.y - 1].isEmpty(), 0, 0, 0));

	if (from.y + 1 <= 7) if (!isSameTeam(from, newPosition(from.x, from.y + 1))) moveMap.push_back(newMove(from, newPosition(from.x, from.y + 1), !m_board[from.x][from.y + 1].isEmpty(), 0, 0, 0));
	if (from.y - 1 >= 0) if (!isSameTeam(from, newPosition(from.x, from.y - 1))) moveMap.push_back(newMove(from, newPosition(from.x, from.y - 1), !m_board[from.x][from.y - 1].isEmpty(), 0, 0, 0));

	if (from.x + 1 <= 7) if (!isSameTeam(from, newPosition(from.x + 1, from.y))) moveMap.push_back(newMove(from, newPosition(from.x + 1, from.y), !m_board[from.x + 1][from.y].isEmpty(), 0, 0, 0));
	if (from.x - 1 >= 0) if (!isSameTeam(from, newPosition(from.x - 1, from.y))) moveMap.push_back(newMove(from, newPosition(from.x - 1, from.y), !m_board[from.x - 1][from.y].isEmpty(), 0, 0, 0));
	std::vector<Move> castlingfrom = checkForCastling(team);
	for (int i = 0; i < castlingfrom.size(); i++) moveMap.push_back(castlingfrom[i]);

	return moveMap;
}
bool Game::isSameTeam(position from, position to)
{
	if (!m_board[to.x][to.y].isEmpty()) {
		if (m_pieceList[m_board[to.x][to.y].getPieceId()].getTeam() == m_pieceList[m_board[from.x][from.y].getPieceId()].getTeam()) return true;
	}
	return false;
}
/// <summary>
/// funtion that return rook possiblity for a selected team
/// </summary>
/// <param name="team"> team king tested </param>
/// <returns></returns>
std::vector<Move> Game::checkForCastling(bool team) {
	std::vector < Move> rookTile;
	int x;
	if (team == 0) x = 0;
	else x = 7;

	if (!m_board[x][4].isEmpty()) {

		if (!m_pieceList[m_board[x][4].getPieceId()].getIfHasMoved()) {
			if (!m_kingInCheck[team]) {
				/// test for short Castling

				if (m_board[x][5].isEmpty() && m_board[x][6].isEmpty()) {

					if (!checkCheckOnTile(newPosition(x, 5), !team) && !checkCheckOnTile(newPosition(x, 6), !team)) {
						if (!m_board[x][7].isEmpty()) {
							if (!m_pieceList[m_board[x][7].getPieceId()].getIfHasMoved()) {
								rookTile.push_back(newMove(newPosition(x,4), newPosition(x, 6),0,0,0,0));
							}
						}
					}
				}
				/// test for Long Castling
				if (m_board[x][3].isEmpty() && m_board[x][2].isEmpty() && m_board[x][1].isEmpty()) {
					if (!checkCheckOnTile(newPosition(x, 3), !team) && !checkCheckOnTile(newPosition(x, 2), !team)) {
						if (!m_board[x][0].isEmpty()) {
							if (!m_pieceList[m_board[x][0].getPieceId()].getIfHasMoved()) {
								rookTile.push_back(newMove(newPosition(x, 4), newPosition(x, 2), 0, 0, 0, 0));
							}
						}
					}
				}
			}
		}
	}



	return rookTile;

}

Move Game::newMove(position from, position to, bool capture, int promotion, bool castling, bool TakeEnPassant)
{
	Move newMove;
	newMove.from = from;
	newMove.to = to;
	newMove.capture = capture;
	newMove.Castling = castling;
	newMove.takeEnPassant = TakeEnPassant;
	newMove.Promotion = promotion;
	return newMove;

}

position Game::newPosition(int x, int y)
{
	position newPos;
	newPos.x = x;
	newPos.y = y;
	return newPos;
}

void Game::displayMoveInfo(Move move) {
	std::cout << "	MOVE INFO " << std::endl;
	std::cout << "CAPTURE :" << move.capture << std::endl;
	std::cout << "CASTLING :" << move.Castling << std::endl;
	std::cout << "ENPASSANT :" << move.takeEnPassant << std::endl;
	std::cout << "PROMOTION :" << move.Promotion << std::endl;
	std::cout << "POSITION :" << getPosFromCoo(move.from) << getPosFromCoo(move.to) << std::endl;

}
