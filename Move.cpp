#include "game.h"

std::vector<position> Game::ValidCooByType(position cooStart, int pieceType, bool pieceTeam) {
	std::vector <position > possibleMove;
	if (pieceType == 1) possibleMove = checkPawnMove(cooStart, pieceTeam);
	if (pieceType == 2) possibleMove = checkBishopMove(cooStart);
	if (pieceType == 3) possibleMove = checkKnightMove(cooStart);
	if (pieceType == 4) possibleMove = checkRookMove(cooStart);
	if (pieceType == 5) possibleMove = checkQueenMove(cooStart);
	if (pieceType == 6) {
		if (m_antiRecursion) possibleMove = checkKingMoveAR(cooStart, pieceTeam);
		else possibleMove = checkKingMove(cooStart, pieceTeam);
	}
	return possibleMove;
}

std::vector<position> Game::checkPawnMove(position coo, bool team)
{
	std::vector<position> ValidCoo;
	if (team == true) {
		if (coo.first == 6) {
			if (m_board[coo.first - 2][coo.second].isEmpty() && m_board[coo.first - 1][coo.second].isEmpty()) {
				ValidCoo.push_back(newPosition(coo.first - 2, coo.second));
			}
		}
		if(coo.first -1 >= 0) if (m_board[coo.first - 1][coo.second].isEmpty()) ValidCoo.push_back(newPosition(coo.first - 1, coo.second));
		if (coo.first - 1 >= 0 && coo.second + 1 <= 7)if ((!isSameTeam(coo, newPosition(coo.first - 1, coo.second + 1))) && !m_board[coo.first - 1][coo.second + 1].isEmpty()) ValidCoo.push_back(newPosition(coo.first - 1, coo.second + 1));
		if (coo.first - 1 >= 0 && coo.second - 1 >= 0)if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second - 1)) && !m_board[coo.first - 1][coo.second - 1].isEmpty()) ValidCoo.push_back(newPosition(coo.first - 1, coo.second - 1));
		if (m_enPassant.Available) {
			if (coo.second == m_enPassant.ligne + 1 || coo.second == m_enPassant.ligne - 1) {
				if (coo.first == 4) {
					ValidCoo.push_back(newPosition(5, m_enPassant.ligne));
				}

			}
		}
	}
	else {
		if (coo.first == 1) {
			if (m_board[coo.first + 2][coo.second].isEmpty() && m_board[coo.first + 1][coo.second].isEmpty()) {
				ValidCoo.push_back(newPosition(coo.first + 2, coo.second));
			}
		}
		if (coo.first +1 <= 7) if (m_board[coo.first + 1][coo.second].isEmpty())  ValidCoo.push_back(newPosition(coo.first + 1, coo.second));
		if (coo.first + 1 <= 7 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second + 1)) && !m_board[coo.first + 1][coo.second + 1].isEmpty()) ValidCoo.push_back(newPosition(coo.first + 1, coo.second + 1));
		if (coo.first + 1 <= 7 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second - 1)) && !m_board[coo.first + 1][coo.second - 1].isEmpty()) ValidCoo.push_back(newPosition(coo.first + 1, coo.second - 1));
		if (m_enPassant.Available) {
			if (coo.second == m_enPassant.ligne + 1 || coo.second == m_enPassant.ligne - 1) {
				if (coo.first == 3) {
					ValidCoo.push_back(newPosition(2, m_enPassant.ligne));
				}

			}
		}
	}



	return ValidCoo;
}

std::vector<position> Game::checkBishopMove(position coo)
{
	std::vector<position> ValidCoo;
	int x = coo.first;
	int y = coo.second;
	bool pieceFound = false;
	while (x != 0 && y != 0 && !pieceFound) {
		x--;
		y--;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
			if (!isSameTeam(coo, newPosition(x, y))) ValidCoo.push_back(newPosition(x, y));
	}

	x = coo.first;
	y = coo.second;
	pieceFound = false;
	while (x != 7 && y != 0 && !pieceFound) {
		x++;
		y--;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(x, y))) ValidCoo.push_back(newPosition(x, y));
	}
	x = coo.first;
	y = coo.second;
	pieceFound = false;
	while (x != 0 && y != 7 && !pieceFound) {
		x--;
		y++;
		if (!m_board[x][y].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(x, y))) ValidCoo.push_back(newPosition(x, y));
	}

	x = coo.first;
	y = coo.second;
	pieceFound = false;
	while (x != 7 && y != 7 && !pieceFound) {
		x++;
		y++;
		if (!m_board[x][y].isEmpty())  pieceFound = true;
		if (!isSameTeam(coo, newPosition(x, y))) ValidCoo.push_back(newPosition(x, y));

	}

	return ValidCoo;
}

std::vector<position> Game::checkKnightMove(position coo)
{
	std::vector<position> ValidCoo;
	if (coo.first + 1 <= 7 && coo.second + 2 <= 7 ) if( !isSameTeam(coo, newPosition(coo.first + 1, coo.second + 2))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second + 2));
	if (coo.first + 2 <= 7 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 2, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first + 2, coo.second + 1));

	if (coo.first - 1 >= 0 && coo.second + 2 <= 7) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second + 2))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second + 2));
	if (coo.first - 2 >= 0 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first - 2, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first - 2, coo.second + 1));

	if (coo.first + 1 <= 7 && coo.second - 2 >= 0) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second - 2))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second - 2));
	if (coo.first + 2 <= 7 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first + 2, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first + 2, coo.second - 1));

	if (coo.first - 1 >= 0 && coo.second - 2 >= 0) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second - 2))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second - 2));
	if (coo.first - 2 >= 0 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first - 2, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first - 2, coo.second - 1));

	return ValidCoo;

}

std::vector<position> Game::checkRookMove(position coo)
{
	std::vector<position> ValidCoo;
	int xmin = coo.first;
	bool pieceFound = false;
	while (xmin != 0 && !pieceFound) {
		xmin--;
		if (!m_board[xmin][coo.second].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(xmin, coo.second))) ValidCoo.push_back(newPosition(xmin, coo.second));

	}
	int xmax = coo.first;
	pieceFound = false;
	while (xmax != 7 && !pieceFound) {
		xmax++;
		if (!m_board[xmax][coo.second].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(xmax, coo.second)))ValidCoo.push_back(newPosition(xmax, coo.second));

	}
	int ymin = coo.second;
	pieceFound = false;
	while (ymin != 0 && !pieceFound) {
		ymin--;
		if (!m_board[coo.first][ymin].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(coo.first, ymin))) ValidCoo.push_back(newPosition(coo.first, ymin));
	}
	int ymax = coo.second;
	pieceFound = false;
	while (ymax != 7 && !pieceFound) {
		ymax++;
		if (!m_board[coo.first][ymax].isEmpty()) pieceFound = true;
		if (!isSameTeam(coo, newPosition(coo.first, ymax)))ValidCoo.push_back(newPosition(coo.first, ymax));

	}
	return ValidCoo;
}

std::vector<position> Game::checkQueenMove(position coo)
{
	std::vector<position> validCoo1, validCoo2;
	validCoo1 = checkBishopMove(coo);
	validCoo2 = checkRookMove(coo);
	validCoo1.insert(validCoo1.end(), validCoo2.begin(), validCoo2.end());
	return validCoo1;
}

std::vector<position> Game::checkKingMove(position coo, bool team)
{
	m_antiRecursion = !m_antiRecursion;
	std::vector<position> ValidCoo;
	if (coo.first - 1 >= 0 && coo.second - 1 >= 0) if ( !isSameTeam(coo, newPosition(coo.first - 1, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second - 1));
	if (coo.first + 1 <= 7 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second + 1));
	if (coo.first - 1 >= 0 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second + 1));
	if (coo.first + 1 <= 7 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second - 1));

	if (coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first, coo.second + 1));
	if (coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first, coo.second - 1));

	if (coo.first + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second));
	if (coo.first - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second)))ValidCoo.push_back(newPosition(coo.first - 1, coo.second));
	std::vector<position> castlingCoo = checkForCastling(team);
	for (int i = 0; i <castlingCoo.size(); i++) ValidCoo.push_back(castlingCoo[i]);
	
	return ValidCoo;
}

std::vector<position> Game::checkKingMoveAR(position coo, int team)
{
	std::vector<position> ValidCoo;
	if (coo.first - 1 >= 0 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second - 1));
	if (coo.first + 1 <= 7 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second + 1));
	if (coo.first - 1 >= 0 && coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second + 1))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second + 1));
	if (coo.first + 1 <= 7 && coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second - 1));

	if (coo.second + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first, coo.second + 1)) ) ValidCoo.push_back(newPosition(coo.first, coo.second + 1));
	if (coo.second - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first, coo.second - 1))) ValidCoo.push_back(newPosition(coo.first, coo.second - 1));

	if (coo.first + 1 <= 7) if (!isSameTeam(coo, newPosition(coo.first + 1, coo.second))) ValidCoo.push_back(newPosition(coo.first + 1, coo.second));
	if (coo.first - 1 >= 0) if (!isSameTeam(coo, newPosition(coo.first - 1, coo.second))) ValidCoo.push_back(newPosition(coo.first - 1, coo.second));
	
	
	return ValidCoo;
}
