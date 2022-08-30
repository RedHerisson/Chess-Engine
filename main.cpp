#include "game.h"
#include "main.h"

int main() {
	bool currPlayer = 0;
	Game Game;
	Game.BoardStartup();
	Game.DisplayBoard();

	bool Mat = false; /// false normalement
	std::pair<position, position> move;
	std::pair<position, position> bestNextMove = std::make_pair(Game.newPosition(0,0), Game.newPosition(0, 0));
	//Game.perf();
	unsigned long long keyArray[12][8][8];
	Game.HashKey(keyArray);
	Game.resetStat();

	Mat = false;
	while (!Mat) {
		Game.incrTour();
		currPlayer = !currPlayer;
		if (currPlayer == 1) move = Game.iAPlayerV2(7, currPlayer, keyArray, &bestNextMove);
		else move = Game.userMoveCmd(currPlayer);

		Game.movePiece(currPlayer, move.first, move.second,1);
		Mat = Game.checkForMat(!currPlayer);
		//Mat = true; /// false normalement
		if (Game.getNbrTour() > 200) Mat = 1;

		Game.DisplayBoard();
		std::cout << "CURR BOARD EVAL :" << Game.evalV2(!currPlayer) << std::endl;
	}
}

