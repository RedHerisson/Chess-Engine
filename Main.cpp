#include "IA.h"

int main() {

	Game Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

	
	Game.DisplayBoard();

	bool Mat = false; /// false normalement
	Move move;
	bool currPlayer = WHITE;

	

	while (!Mat) {
		Game.incrTour();
		IA engine(&Game);
		
		if (currPlayer == BLACK) move = engine.findMove(7, currPlayer);
		else move = Game.userMoveCmd(currPlayer);
		LastGameStat gameStat;
		Game.DisplayBoard();
		Game.movePiece(move, currPlayer, &gameStat);
		
		Mat = Game.checkForMat(!currPlayer);

		//if (Game.getNbrTour() > 200) Mat = 1;

		Game.DisplayBoard();

		std::cout << "CURR BOARD EVAL :" << engine.eval(!currPlayer) << std::endl;
		currPlayer = !currPlayer;
	}
}
