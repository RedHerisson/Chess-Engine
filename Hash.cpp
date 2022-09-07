#include "game.h"

void Game::HashKey() {
	srand(5465465);
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				m_keyArray[i][j][k] = rand64bits();
			}
		}
	}
}

int Game::hashMove(Move move)
{
	return move.from.x + move.from.y * 10 + move.to.x * 100 + move.to.y * 1000 + move.capture * 10000 + move.Promotion * 20000 + move.takeEnPassant * 40000;
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