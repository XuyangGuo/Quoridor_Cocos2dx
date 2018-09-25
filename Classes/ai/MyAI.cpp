#include "MyAI.h"
#include <cstring>
#include <queue>
#include <iostream>
#include "game/ChessState.h"
using namespace std;

MyAI::MyAI(ChessState * chessState, int difficulty)
{
	this->chessState = chessState;
	this->difficulty = difficulty;
}

Op MyAI::getAIOp()
{
	Op res = Op(EMPTY, pair<int, int> (9, 9));

	if (chessState->getRemain(1) > 0) {

		// 前几步可以按谱走
		if (chessState->getStep() < 2) {
			if (opening(res)) {
				return res;
			}
		}

		// 组合挡
		if (difficulty == 1 && doubleCount(res)) {
			//CCLOG("double count!!!");
			return res;
		}

		// 计算是否可以去除自己的最长路径。
		if (longPathForPlayer(res)) {
			//CCLOG("long path!!!");
			return res;
		}

		// 计算阻挡对方的最佳木板位置
		if (AiCount(res)) {
			//CCLOG("AI COUNT!");
			return res;
		}

		// 阻止对方延长自己的路径
		if (difficulty == 1 && strongSelfWall(res)) {
			//CCLOG("strong defend!");
			return res;
		}
	}

	// 木板下完了，那就只能任人宰割了。
	vector<int> playerWithGamer;
	if (pathForPlayerWithGamer(true, playerWithGamer) && playerWithGamer.size() > 1) {
		int nextStep = playerWithGamer[playerWithGamer.size() - 2];
		return Op(EMPTY, pair<int, int>(nextStep / 9, nextStep % 9));
	} else {
		playerWithGamer.clear();
		pathForPlayer(true, playerWithGamer);

		vector <pair<int, int> > resReachable;
		chessState->getReachable(1, resReachable);

		int nextP = playerWithGamer[playerWithGamer.size() - 2];
		for (pair<int, int> & reach : resReachable) {
			if (reach.first * 9 + reach.second == nextP) {
				return Op(EMPTY, pair<int, int>(nextP / 9, nextP % 9));
			}
		}
		return Op(EMPTY, resReachable[0]);  // 随便返回一个位置
	}
	
	//vector<vector<int> > res;
		//allPath(true, res);

		////print
		//for (int i = 0; i < res.size(); ++i) {
		//	
		//	string s = to_string(i) + ": ";
		//	for (int j = 0; j < res[i].size(); ++j) {
		//		char buffer[50];
		//		sprintf(buffer, "(%d,%d)<- ", res[i][j] / 9, res[i][j] % 9);
		//		s += string(buffer);
		//	}
		//	CCLOG(s.c_str());
		//}
		//CCLOG("");
}


// 把挡人写进去！！！
inline bool MyAI::opening(Op & res)
{	
	// 计算移动概率
	srand((int)time(0));
	int isBlock = ((rand() % 100) + 50) % 100;

	// 获取完全路径
	vector<vector<int> > allPathCountGamer;
	allPath(false, allPathCountGamer);

	// CCLOG("%d %d", isBlock, allPathCountGamer.size());
	if (isBlock < 30 + 4 * chessState->getRemain(1) && allPathCountGamer.size() <= 1) {
		// 计算出一个随机木板位置，并检测是否合理,尝试50次
		// CCLOG("try to set");

		vector<int> choice;
		for (int i = 0; i < 128; ++i) {
			choice.push_back(i);
		}

		// ai 最短路径
		vector<int> player;
		pathForPlayer(true, player);
		vector<int> gamerPath;
		pathForPlayer(false, gamerPath);

		while (!choice.empty()) {
			int cur_r = rand() % choice.size();
			int cur = choice[cur_r];
			choice[cur_r] = choice.back();
			choice.pop_back();

			Block h;
			if (cur < 64) {
				h = VERTICAL;
			} else {
				h = HORIZONTAL;
			}
			int x = (cur % 64) / 8;
			int y = cur % 8;
			if (!chessState->canPutPlank(x, y, h)) {
				continue;
			}


			// 计算该木板是否会阻挡到自己。
			chessState->block[x][y] = h;
			vector<int> test;
			pathForPlayer(true, test);
			vector<vector<int> > allPathsCountTemp;
			allPath(false, allPathsCountTemp);

			vector<int> gamerPathTemp;
			pathForPlayer(false, gamerPathTemp);

			chessState->block[x][y] = EMPTY;

			if (allPathsCountTemp.size() < 2 || gamerPathTemp.size() <= gamerPath.size()
				|| int(gamerPathTemp.size()) - int(gamerPath.size()) < int(test.size()) - int(player.size())) {
				continue;
			}

			res = Op(h, pair<int, int>(x, y));
			return true;
		}

		// CCLOG("Sign!!!");
	}

	return false;
}

inline bool MyAI::longPathForPlayer(Op & res)
{
	vector<vector<int> > allPaths;
	allPath(true, allPaths);
	if (allPaths.size() == 2) {
		int count0 = allPaths[0].size();
		int count1 = allPaths[1].size();
		vector<int> & my_max = count0 > count1 ? allPaths[0] : allPaths[1];
		vector<int> & my_min = count0 > count1 ? allPaths[1] : allPaths[0];

		if (my_max.size() - my_min.size() > 2) {  // > 3
			for (int i = 1; i < my_max.size(); ++i) {
				vector<Op> walls;
				wallData(my_max[i], my_max[i - 1], walls);
				for (Op & wall : walls) {
					// !!!
					chessState->block[wall.i][wall.j] = wall.block;

					vector<vector<int> > allPathTemp;
					allPath(true, allPathTemp);
					chessState->block[wall.i][wall.j] = EMPTY;

					if (allPathTemp.size() == 1 && allPathTemp[0].size() == my_min.size()) {	
						res = wall;
						return true;
					}
				}
			}
		}
	}

	return false;
}

inline void MyAI::wallData(int ids0, int ids1, vector<Op> & res)
{
	if (ids1 < ids0) {
		int temp = ids1;
		ids1 = ids0;
		ids0 = temp;
	}

	int x = ids0 / 9;
	int y = ids0 % 9;
	if ((ids1 - ids0) == 9) {
		if (chessState->canPutPlank(x, y, HORIZONTAL)) {
			res.push_back(Op(HORIZONTAL, pair<int, int>(x, y)));
		}
		if (chessState->canPutPlank(x, y - 1, HORIZONTAL)) {
			res.push_back(Op(HORIZONTAL, pair<int, int>(x, y - 1)));
		}
	} else {
		CCAssert((ids1 - ids0) == 1, "(ids1 - ids0) != 1");
		if (chessState->canPutPlank(x, y, VERTICAL)) {
			res.push_back(Op(VERTICAL, pair<int, int>(x, y)));
		}
		if (chessState->canPutPlank(x - 1, y, VERTICAL)) {
			res.push_back(Op(VERTICAL, pair<int, int>(x - 1, y)));
		}
	}
}

inline bool MyAI::AiCount(Op & res)
{
	vector<int> player, rival;
	pathForPlayer(true, player);
	pathForPlayer(false, rival);

	if (player.size() <= rival.size()) {  // add by gil
		return false;
	}

	int maxGain = -1;
	Op bestWall(EMPTY, pair<int, int> (9, 9));

	// 查找阻挡对方的最佳木板位置
	for (int i = rival.size() - 1; i > 0; --i) {
		vector<Op> walls;
		wallData(rival[i], rival[i - 1], walls);

		for (Op & wall : walls) {
			chessState->block[wall.i][wall.j] = wall.block;

			vector<int> gamerPathTemp, playerPathTemp;
			pathForPlayer(false, gamerPathTemp);
			pathForPlayer(true, playerPathTemp);
			
			chessState->block[wall.i][wall.j] = EMPTY;
			int tempGain = int(gamerPathTemp.size()) - int(rival.size()) - int(playerPathTemp.size()) + int(player.size()) - 1;
			if (tempGain > maxGain) {
				maxGain = tempGain;
				bestWall = wall;
			}
		}
	}

	if (bestWall.block != EMPTY) {
		if (player.size() > rival.size() || maxGain > 2) {
			res = bestWall;
			return true;
		}
	}
	return false;
}

inline bool MyAI::strongSelfWall(Op & res)
{
	vector<int> player, gamer;
	pathForPlayer(true, player);
	pathForPlayer(false, gamer);

	for (int i = player.size() - 1; i > 0; --i) {
		vector<Op> walls;
		wallData(player[i], player[i - 1], walls);
		for (Op & wall : walls) {
			chessState->block[wall.i][wall.j] = wall.block;
			vector<int> pathTemp;
			pathForPlayer(true, pathTemp);
			chessState->block[wall.i][wall.j] = EMPTY;

			if ((player.size() + 4 <= gamer.size() && pathTemp.size() > player.size() + 2) || 
				pathTemp.size() > player.size() + 5) {  // pathTemp.size() > player.size() + 1
				int offsetXs[3] = { 0, -1, 1 };
				int offsetYs[3] = { 0, -1, 1 };
				Block offsetHs[2] = { HORIZONTAL, VERTICAL };
				for (int offsetX : offsetXs) {
					for (int offsetY : offsetYs) {
						for (Block offsetH : offsetHs) {
							if (chessState->canPutPlank(wall.i + offsetX, wall.j + offsetY, offsetH)) {
								chessState->block[wall.i + offsetX][wall.j + offsetY] = offsetH;
								vector<int> guardPath;
								pathForPlayer(true, guardPath);
								bool strongSucceed = !chessState->canPutPlank(wall.i, wall.j, wall.block);
								chessState->block[wall.i + offsetX][wall.j + offsetY] = EMPTY;

								if (guardPath.size() == player.size() && strongSucceed) {
									res = Op(offsetH, pair <int, int> (wall.i + offsetX, wall.j + offsetY));
									return true;
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

// 连续找两步板子
inline bool MyAI::doubleCount(Op & res)
{
	if (chessState->getRemain(1) < 2) {
		return false;
	}

	vector<int> gamer0, player0;
	pathForPlayer(false, gamer0);
	pathForPlayer(true, player0);

	int gain2 = 0, gain1 = 0;
	Op cur = Op(EMPTY, pair<int, int>(9, 9));

	for (int m = 0; m < 128; ++m) {
		int i1 = (m % 64) / 8, j1 = m % 8;
		Block h1 = m < 64 ? VERTICAL : HORIZONTAL;
		if (chessState->canPutPlank(i1, j1, h1)) {
			chessState->block[i1][j1] = h1;  // !!!
			
			vector<int> gamer1, player1;
			pathForPlayer(false, gamer1);
			pathForPlayer(true, player1);

			if (gamer1.size() > 2) {
				pair<int, int> curPos = chessState->players[0];
				int nextPos = gamer1[gamer1.size() - 2]; 
				
				int tempGain1 = int(gamer1.size()) - int(gamer0.size()) - 1 - (int(player1.size()) - int(player0.size()));

				chessState->players[0] = pair<int, int>(nextPos / 9, nextPos % 9); // !!! 沿最短路走一步

				for (int i = gamer1.size() - 2; i > 0; --i) {
					vector<Op> walls;
					wallData(gamer1[i], gamer1[i - 1], walls);
					for (Op & wall : walls) {
						chessState->block[wall.i][wall.j] = wall.block; // !!!

						vector<int> gamer2, player2;
						pathForPlayer(false, gamer2);
						pathForPlayer(true, player2);

						int tempGain2 = int(gamer2.size()) - int(gamer0.size()) - 1 - (int(player2.size()) - int(player0.size()));


						if (tempGain2 > gain2 || tempGain2 == gain2 && tempGain1 > gain1) {
							gain2 = tempGain2;
							gain1 = tempGain1;
							cur = Op(h1, pair<int, int>(i1, j1));
						}
						chessState->block[wall.i][wall.j] = EMPTY;  // !!!
					}
				}
				
				chessState->players[0] = curPos;  // !!!
			}

			chessState->block[i1][j1] = EMPTY;  // !!!
		}

	}

	if (gain2 > 4 || gain2 > 2 && gain1 > 1) {
		res = cur;
		return true;
	}
	return false;
}

// BFS Shortest
void MyAI::pathForPlayer(bool play, vector<int>& res)
{
	int player, des;

	if (play) {  // is AI
		pair<int, int> posPlayer = chessState->players[1];
		player = posPlayer.first * 9 + posPlayer.second;
	} else {
		pair<int, int> posPlayer = chessState->players[0];
		player = posPlayer.first * 9 + posPlayer.second;
	}

	bool canVis[81];
	int parents[81];
	memset(canVis, 1, sizeof(canVis));

	queue<int> q;
	q.push(player);
	canVis[player] = false;
	parents[player] = -1;

	while (true) {
		if (q.size() == 0) {
			return;
		}
		int cur = q.front();
		q.pop();
		if (canLeft(cur) && canVis[cur - 1]) {
			q.push(cur - 1);
			canVis[cur - 1] = false;
			parents[cur - 1] = cur;
		}
		if (canRight(cur) && canVis[cur + 1]) {
			q.push(cur + 1);
			canVis[cur + 1] = false;
			parents[cur + 1] = cur;
		}
		if (canUp(cur) && canVis[cur + 9]) {
			canVis[cur + 9] = false;
			parents[cur + 9] = cur;
			if (!play && (cur > 62)) {
				des = cur + 9;
				break;
			} 
			q.push(cur + 9);
		}
		if (canDown(cur) && canVis[cur - 9]) {
			canVis[cur - 9] = false;
			parents[cur - 9] = cur;
			if (play && (cur < 18)) {
				des = cur - 9;
				break;
			}
			q.push(cur - 9);
		}
	}

	while (des != -1) {
		res.push_back(des);
		des = parents[des];
	}
}

inline bool MyAI::pathForPlayerWithGamer(bool play, vector<int>& res)
{
	int player, endX, des;

	if (play) {  // is AI
		pair<int, int> posPlayer = chessState->players[1];
		player = posPlayer.first * 9 + posPlayer.second;
		endX = 0;
	} else {
		pair<int, int> posPlayer = chessState->players[0];
		player = posPlayer.first * 9 + posPlayer.second;
		endX = 8;
	}

	bool canVis[81], continueFind = true;
	int parents[81];
	memset(canVis, 1, sizeof(canVis));

	queue<int> q;
	q.push(player);
	canVis[player] = false;
	parents[player] = -1;

	while (continueFind) {
		if (q.size() == 0) {
			return false;
		}
		int cur = q.front();
		q.pop();

		vector<pair<int, int> > rest;
		chessState->getReachable(int(play), pair<int, int>(cur / 9, cur % 9), rest);
		for (auto & p : rest) {
			des = p.first * 9 + p.second;
			if (canVis[des]) {
				parents[des] = cur;
				canVis[des] = false;
				if (p.first == endX) {
					continueFind = false;
					break;
				}
				q.push(des);
			}
		}
	}

	while (des != -1) {
		res.push_back(des);
		des = parents[des];
	}
	return true;
}


// 全路径
inline void MyAI::allPath(bool play, vector<vector<int>>& res)
{
	int player, destX;

	if (play) {  // is AI
		pair<int, int> posPlayer = chessState->players[1];
		player = posPlayer.first * 9 + posPlayer.second;
		destX = 0;
	} else {
		pair<int, int> posPlayer = chessState->players[0];
		player = posPlayer.first * 9 + posPlayer.second;
		destX = 8;
	}

	map<int, int> mv, ms;
	mv[player] = -1;
	ms[player] = -1;
	allPathForPlayer(mv, ms, destX, res);
}

inline void MyAI::allPathForPlayer(map<int, int> & visited, map<int, int> & scanQueue, int destX, vector<vector<int>>& res)
{
	while (!scanQueue.empty()) {
		// 0.0 检查队列是否包含终点目标，是的话输出路径
		for (map<int, int>::iterator it = scanQueue.begin(); it != scanQueue.end(); ++it) {
			if (destX == it->first / 9) {
				res.push_back(vector<int>());
				vector<int> & path = res.back();

				int node = it->first;
				path.push_back(node);
				while ((node = visited[node]) != -1) {
					path.push_back(node);
				}
				return;
			}
		}

		// 0.1 对队列进行扩展扫描，将没有阻隔的扩展节点组合成集合。
		vector<map<int, int>> nodeSets;
		for (map<int, int>::iterator it = scanQueue.begin(); it != scanQueue.end(); ++it) { // *it==scanNode 
			map<int, int> nodeScopes;
			if (canUp(it->first) && visited.find(it->first + 9) == visited.end()) {
				nodeScopes[it->first + 9] = it->first;
			}
			if (canDown(it->first) && visited.find(it->first - 9) == visited.end()) {
				nodeScopes[it->first - 9] = it->first;
			}
			if (canLeft(it->first) && visited.find(it->first - 1) == visited.end()) {
				nodeScopes[it->first - 1] = it->first;
			}
			if (canRight(it->first) && visited.find(it->first + 1) == visited.end()) {
				nodeScopes[it->first + 1] = it->first;
			}

			if (nodeScopes.size() == 2) {
				map<int, int>::iterator it = nodeScopes.begin();
				int d1 = it->first;
				it++;
				int d2 = it->first;
				int absValue = abs(d1 - d2);
				if (absValue == 18 || absValue == 2) {
					map<int, int> tempS1, tempS2;
					tempS1[it->first] = it->second;
					it--;
					tempS2[it->first] = it->second;
					myUnion(nodeSets, tempS1);
					myUnion(nodeSets, tempS2);
				} else {
					myUnion(nodeSets, nodeScopes);
				}
			} else if (nodeScopes.size() > 0) {
				myUnion(nodeSets, nodeScopes);
			}
		}

		// 0.2 对所有节点的集合数组进行判定，假如出现分支，则对每个分支进行迭代处理，然后退出。否则更新搜索队列。
		if (nodeSets.size() > 1) {
			for (auto & nodeSet : nodeSets) {
				// 0.2.1 计算出该分组的父节点以及已经被查询过的领结点，作为新的logs
				map<int, int> nodes = nodeSet;
				map<int, int> pathSet = nodeSet;
				while (!nodes.empty()) {
					pair<int, int> node = *nodes.begin();
					nodes.erase(nodes.begin());
					
					map<int, int>::iterator finder;
					if (canUp(node.first) && (finder = visited.find(node.first + 9)) != visited.end()) {
						pathSet.insert(*finder);
					}
					if (canDown(node.first) && (finder = visited.find(node.first - 9)) != visited.end()) {
						pathSet.insert(*finder);
					}
					if (canLeft(node.first) && (finder = visited.find(node.first - 1)) != visited.end()) {
						pathSet.insert(*finder);
					}
					if (canRight(node.first) && (finder = visited.find(node.first + 1)) != visited.end()) {
						pathSet.insert(*finder);
					}

					pathSet.insert(node);  // 是否可以删除
					if (node.second != -1) {
						nodes.insert(*visited.find(node.second));
					}
				}

				// 0.2.2 获取分支路径
				allPathForPlayer(pathSet, nodeSet, destX, res);
			}
			return;
		} else if (nodeSets.size() > 0) {
			for (pair<const int, int> & n : nodeSets[0]) {
				visited.insert(n);
			}
			scanQueue = nodeSets[0];
		} else {
			return;
		}
	}
}

inline bool MyAI::canUp(int cur)
{
	return chessState->canUp(cur / 9, cur % 9);
}

inline bool MyAI::canDown(int cur)
{
	return chessState->canDown(cur / 9, cur % 9);
}

inline bool MyAI::canLeft(int cur)
{
	return chessState->canLeft(cur / 9, cur % 9);
}

inline bool MyAI::canRight(int cur)
{
	return chessState->canRight(cur / 9, cur % 9);
}

inline void MyAI::myUnion(vector<map<int, int> > & allSets, map<int, int> & newSet)
{
	bool hasUnion = false;

	for (int i = 0; i < allSets.size(); ++i) {
		map<int, int> temp;
		if (baseUnion(allSets[i], newSet, temp)) {  // not disjoint
			allSets[i] = temp;
			hasUnion = true;

			int j = i + 1;
			while (j < allSets.size()) {
				map<int, int> temp2;
				if (baseUnion(allSets[i], allSets[j], temp2)) {
					allSets[i] = temp2;
					allSets[j] = allSets.back();
					allSets.pop_back();
				} else {
					j++;
				}
			}
			break;
		}
	}

	if (!hasUnion) {
		allSets.push_back(newSet);
	}
}

// true joint
inline bool MyAI::baseUnion(map<int, int> & s1, map<int, int> & s2, map<int, int> & res)
{
	res = s2;
	for (map<int, int>::iterator it = s1.begin(); it != s1.end(); ++it) {
		res[it->first] = it->second;
	}
	return res.size() < s1.size() + s2.size();
}
