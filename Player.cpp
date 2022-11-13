#include "Game.h"

Move Game::userMoveCmd(bool team) {
	do {
		std::string moveCmd;

		std::cin.clear();
		std::cin >> moveCmd; // a7a6
		if (moveCmd.size() == 4) {
			position CooStart = getCooFromPos(moveCmd[0], moveCmd[1]);
			position CooEnd = getCooFromPos(moveCmd[2], moveCmd[3]);
			Move userMove = newMove(CooStart, CooEnd, 0, 0, 0, 0);
			displayMoveInfo(userMove);
			if (!m_board[CooStart.x][CooStart.y].isEmpty()) {

				if (m_pieceList[m_board[CooStart.x][CooStart.y].getPieceId()].getTeam() == team) {
					if (checkMove(&userMove, m_pieceList[m_board[CooStart.x][CooStart.y].getPieceId()].getType(), m_pieceList[m_board[CooStart.x][CooStart.y].getPieceId()].getTeam())) {

						if (!checkPrevision(userMove, team)) {

							return userMove;
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
			std::cout << m_board[CooStart.x][CooStart.y].isEmpty() << std::endl;
			if (!m_board[CooStart.x][CooStart.y].isEmpty()) {
				std::vector<Move > validPos;
				validPos = getPieceRange(CooStart, m_pieceList[m_board[CooStart.x][CooStart.y].getPieceId()].getType(), m_pieceList[m_board[CooStart.x][CooStart.y].getPieceId()].getTeam());
				//m_antiRecursion = 0; /// only for aff, reset flag after 
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