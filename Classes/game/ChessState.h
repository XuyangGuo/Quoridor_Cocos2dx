#pragma once
#include <utility>
#include <stack>
#include <vector>
#include <cocos2d.h>
USING_NS_CC;
using namespace std;

enum Block {
	EMPTY,
	HORIZONTAL,
	VERTICAL,
};

enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NO_DIR
};

struct Op {
	Op(Block b, pair<int ,int> p);
	Block block;
	int i, j;
};

class ChessState {
public:
	friend class MyAI;

	ChessState(int r);
	void getReachable(int player, vector<pair<int, int> > & v);
	void getReachable(int player, pair<int, int> pos, vector<pair<int, int> > & v);

	int getRemain(int p);
	int setStep(Op op, int p);
	Op setRegret(int p);
	int getStep();
	bool canRegret();
	
	vector<Direction> getCanBuild(int i, int j);

private:

	// left up is [7, 0] ;  left down is [0, 0]
	Block block[8][8];
	pair<int, int> players[2];
	int remain[2];
	int stepDouble;

	stack<Op> path[2];

	inline bool canPutPlank(int i, int j, Block b);
	inline bool canUp(int i0, int j0);
	inline bool canDown(int i0, int j0);
	inline bool canLeft(int i0, int j0);
	inline bool canRight(int i0, int j0);

	bool canMoveToEnd(Op op);
	bool DFS(int p);
};