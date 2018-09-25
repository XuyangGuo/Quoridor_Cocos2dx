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
	getReachable(p, players[p], v);
}

void ChessState::getReachable(int p, pair<int, int> pos, vector<pair<int, int>>& v)
{
	int i0 = pos.first, j0 = pos.second, i1 = players[1 - p].first, j1 = players[1 - p].second;
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
		if (i0 == i1 && j0 - 1 == j1) {
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
		remain[p]++;
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

vector<Direction> ChessState::getCanBuild(int i, int j)
{
	vector<Direction> v;
	// left
	//if (i < 8 && i > -1 && (j == 1 || j > 1 && j <= 8 && block[i][j - 2] != HORIZONTAL)  && (j == 8 || block[i][j] != HORIZONTAL) && block[i][j - 1] == EMPTY && canMoveToEnd(Op(HORIZONTAL, pair<int, int> (i, j - 1)))) {
	//	v.push_back(LEFT);
	//}
	//// right
	//if (i < 8 && i > -1 && (j == 6 || j < 6 && j >= -1 && block[i][j + 2] != HORIZONTAL) && (j == -1 || block[i][j] != HORIZONTAL && block[i][j + 1] == EMPTY) && canMoveToEnd(Op(HORIZONTAL, pair<int, int>(i, j + 1)))) {
	//	v.push_back(RIGHT);
	//}
	//// down
	//if (j < 8 && j > -1 && (i == 1 || i > 1 && i <= 8 && block[i - 2][j] != VERTICAL) && (i == 8 || block[i][j] != VERTICAL && block[i - 1][j] == EMPTY) && canMoveToEnd(Op(VERTICAL, pair<int, int>(i - 1, j)))) {
	//	v.push_back(DOWN);
	//}
	//// up
	//if (j < 8 && j > -1 && (i == 6 || i < 6 && i >= -1 && block[i + 2][j] != VERTICAL) && (i == -1 || block[i][j] != VERTICAL && block[i + 1][j] == EMPTY) && canMoveToEnd(Op(VERTICAL, pair<int, int>(i + 1, j)))) {
	//	v.push_back(UP);
	//}

	if (canPutPlank(i, j - 1, HORIZONTAL)) {
		v.push_back(LEFT);
	}
	if (canPutPlank(i, j + 1, HORIZONTAL)) {
		v.push_back(RIGHT);
	}
	if (canPutPlank(i - 1, j, VERTICAL)) {
		v.push_back(DOWN);
	}
	if (canPutPlank(i + 1, j, VERTICAL)) {
		v.push_back(UP);
	}
	return v;
}

inline bool ChessState::canPutPlank(int i, int j, Block b)
{
	if (b == HORIZONTAL) {
		if (i < 8 && i > -1 && (j == 0 || j > 0 && j < 8 && block[i][j - 1] != HORIZONTAL) && (j == 7 || block[i][j + 1] != HORIZONTAL) && block[i][j] == EMPTY && canMoveToEnd(Op(HORIZONTAL, pair<int, int>(i, j)))) {
			return true;
		}
	} else {
		if (j < 8 && j > -1 && (i == 0 || i > 0 && i < 8 && block[i - 1][j] != VERTICAL) && (i == 7 || block[i + 1][j] != VERTICAL) && block[i][j] == EMPTY && canMoveToEnd(Op(VERTICAL, pair<int, int>(i, j)))) {
			return true;
		}
	}
	return false;
}


bool ChessState::canMoveToEnd(Op op)
{
	CCAssert(block[op.i][op.j] == EMPTY, "block[op.i,op.j] != EMPTY");
	block[op.i][op.j] = op.block;
	auto res = DFS(0) && DFS(1);
	block[op.i][op.j] = EMPTY;
	return res;
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


bool ChessState::DFS(int p)
{
	stack<pair<int, int> > s;
	bool visited[9][9];
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			visited[i][j] = false;
		}
	}	

	s.push(this->players[p]);
	visited[players[p].first][players[p].second] = true;

	while (!s.empty()) {
		int i = s.top().first, j = s.top().second;
		s.pop();
		if (canUp(i, j) && !visited[i + 1][j]) {
			if (p == 0 && i == 7) {
				return true;
			}
			visited[i + 1][j] = true;
			s.push(pair<int, int>(i + 1, j));
		}
		if (canDown(i, j) && !visited[i - 1][j]) {
			if (p == 1 && i == 1) {
				return true;
			}
			visited[i - 1][j] = true;
			s.push(pair<int, int>(i - 1, j));
		}
		if (canLeft(i, j) && !visited[i][j - 1]) {
			visited[i][j - 1] = true;
			s.push(pair<int, int>(i, j - 1));
		}
		if (canRight(i, j) && !visited[i][j + 1]) {
			visited[i][j + 1] = true;
			s.push(pair<int, int>(i, j + 1));
		}
	}
	return false;
}


Op::Op(Block b, pair<int, int> p)
{
	this->block = b;
	this->i = p.first;
	this->j = p.second;
}
