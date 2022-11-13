#include "IA.h"

int main() {

<<<<<<< HEAD
	Game Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
=======
	Game Game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R");
>>>>>>> parent of 6376dfd (petit changement)

	IA engine(&Game);
	Game.DisplayBoard();

	bool Mat = false; /// false normalement
	Move move;
	bool currPlayer = BLACK;

	

	while (!Mat) {
		Game.incrTour();
<<<<<<< HEAD
		IA engine(&Game);
		
		if (currPlayer == BLACK) move = engine.findMove(7, currPlayer);
=======
		currPlayer = !currPlayer;
		if (currPlayer == 1) move = engine.findMove(6, currPlayer);
>>>>>>> parent of 6376dfd (petit changement)
		else move = Game.userMoveCmd(currPlayer);
		LastGameStat gameStat;
		Game.movePiece(move, currPlayer, &gameStat);
		
		Mat = Game.checkForMat(!currPlayer);

		//if (Game.getNbrTour() > 200) Mat = 1;

		Game.DisplayBoard();

		std::cout << "CURR BOARD EVAL :" << engine.eval(!currPlayer) << std::endl;
	}
}
