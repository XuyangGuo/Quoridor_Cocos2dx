#include "ChessState.h"

ChessState::ChessState(int r) {
	remain[0] = r;
	remain[1] = r;
	stepDouble = 0;

	for (int i = 0; i < 1; ++i) {
		while (!path[i].empty()) {
			path[i].pop();
		}
	}

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			block[i][j] = EMPTY;
		}
	}

	players[0].first = 0;
	players[0].second = 4;
	players[1].first = 8;
	players[1].second = 4;
}

void ChessState::getReachable(int p, vector<pair<int, int> > & v)
{
	int i0 = players[p].first, j0 = players[p].second, i1 = players[1 - p].first, j1 = players[1 - p].second;
	if (canDown(i0, j0)) {
		if (i0 - 1 == i1 && j0 == j1) {
			if (canDown(i1, j1)) {
				v.push_back(pair<int, int>(i1 - 1, j1));
			} else {
				if (canLeft(i1, j1)) {
					v.push_back(pair<int, int>(i1, j1 - 1));
				}
				if (canRight(i1, j1)) {
					v.push_back(pair<int, int>(i1, j1 + 1));
				}
			}
		} else {
			v.push_back(pair<int, int>(i0 - 1, j0));
		}
	}

	if (canUp(i0, j0)) {
		if (i0 + 1 == i1 && j0 == j1) {
			if (canUp(i1, j1)) {
				v.push_back(pair<int, int>(i1 + 1, j1));
			} else {
				if (canLeft(i1, j1)) {
					v.push_back(pair<int, int>(i1, j1 - 1));
				}
				if (canRight(i1, j1)) {
					v.push_back(pair<int, int>(i1, j1 + 1));
				}
			}
		} else {
			v.push_back(pair<int, int>(i0 + 1, j0));
		}
	}

	if (canLeft(i0, j0)) {
		if (i0== i1 && j0 - 1 == j1) {
			if (canLeft(i1, j1)) {
				v.push_back(pair<int, int>(i1, j1 - 1));
			} else {
				if (canUp(i1, j1)) {
					v.push_back(pair<int, int>(i1 + 1, j1));
				}
				if (canDown(i1, j1)) {
					v.push_back(pair<int, int>(i1 - 1, j1));
				}
			}
		} else {
			v.push_back(pair<int, int>(i0, j0 - 1));
		}
	}

	if (canRight(i0, j0)) {
		if (i0 == i1 && j0 + 1 == j1) {
			if (canRight(i1, j1)) {
				v.push_back(pair<int, int>(i1, j1 + 1));
			} else {
				if (canUp(i1, j1)) {
					v.push_back(pair<int, int>(i1 + 1, j1));
				}
				if (canDown(i1, j1)) {
					v.push_back(pair<int, int>(i1 - 1, j1));
				}
			}
		} else {
			v.push_back(pair<int, int>(i0, j0 + 1));
		}
	}
}

int ChessState::getRemain(int p) 
{
	return remain[p];
}

int ChessState::setStep(Op op, int p)
{
	if (op.block == EMPTY) {
		auto temp = players[p];
		players[p] = pair<int, int>(op.i, op.j);

		op.i = temp.first;
		op.j = temp.second;
		
	} else {
		block[op.i][op.j] = op.block;
		remain[p]--;
	}

	stepDouble++;
	path[p].push(op);
	
	return stepDouble / 2;
}

Op ChessState::setRegret(int p)
{
	Op op = path[p].top();
	path[p].pop();

	if (op.block == EMPTY) {
		players[p].first = op.i;
		players[p].second = op.j;
	} else {
		block[op.i][op.j] = EMPTY;
	}
	stepDouble--;
	return op;
}

int ChessState::getStep()
{
	return stepDouble / 2;
}

bool ChessState::canRegret()
{
	return !path[0].empty();
}


inline bool ChessState::canUp(int i0, int j0)
{
	return i0 < 8 && (j0 == 8 || block[i0][j0] != HORIZONTAL) && (j0 == 0 || block[i0][j0 - 1] != HORIZONTAL);
}

inline bool ChessState::canDown(int i0, int j0)
{
	return i0 > 0 && (j0 == 8 || block[i0 - 1][j0] != HORIZONTAL) && (j0 == 0 || block[i0 - 1][j0 - 1] != HORIZONTAL);
}

inline bool ChessState::canLeft(int i0, int j0)
{
	return j0 > 0 && (i0 == 8 || block[i0][j0 - 1] != VERTICAL) && (i0 == 0 || block[i0 - 1][j0 - 1] != VERTICAL);
}

inline bool ChessState::canRight(int i0, int j0)
{
	return j0 < 8 && (i0 == 8 || block[i0][j0] != VERTICAL) && (i0 == 0 || block[i0 - 1][j0] != VERTICAL);
}

Op::Op(Block b, pair<int, int> p)
{
	this->block = b;
	this->i = p.first;
	this->j = p.second;
}
