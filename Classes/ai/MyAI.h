#pragma once
#include <vector>
#include <set>
#include <map>
#include "game/ChessState.h"
using namespace std;

class MyNode {
public:
	MyNode(int data, int parent) : data(data), parent(parent) {}
	int data;
	int parent;
};

// true AI, false myself

class MyAI {
public:
	MyAI(ChessState * chessState, int difficulty);
	 Op getAIOp();

private:
	ChessState * chessState;
	int difficulty;  // 0 easy  1 hard

	// opening
	inline bool opening(Op & res);

	// long path to remove
	inline bool longPathForPlayer(Op & res);

	// get planks that could split ids0 and ids1
	inline void wallData(int ids0, int ids1, vector<Op> & res);

	// the best place to set plank
	inline bool AiCount(Op & res);

	// defend ai's shortest path
	inline bool strongSelfWall(Op & res);

	// double scan
	inline bool doubleCount(Op & res);


	// shortest path
	inline void pathForPlayer(bool play, vector<int> & res);
	inline bool pathForPlayerWithGamer(bool play, vector<int> & res);

	// all shortest path
	inline void allPath(bool play, vector<vector<int>>& res);
	inline void allPathForPlayer(map<int, int> & visited, map<int, int> & scanQueue, int destX, vector<vector<int>>& res);

	// base function
	inline bool canUp(int cur);
	inline bool canDown(int cur);
	inline bool canLeft(int cur);
	inline bool canRight(int cur);

	inline void myUnion(vector<map<int, int> > & allSets, map<int, int> & newSet);
	inline bool baseUnion(map<int, int> & s1, map<int, int> & s2, map<int, int> & res);
};
