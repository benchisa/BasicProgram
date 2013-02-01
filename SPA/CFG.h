#pragma once
#include "Global_Data_Abstraction.h"
#include <queue>
#include <stack>

class CFG
{
public:
	CFG(int size);
	~CFG(void);
	bool isConnected(PROG_LINE p1, PROG_LINE p2);
	bool addEdge(PROG_LINE p1, PROG_LINE p2);
	list<int> findAll(PROG_LINE p1, PROG_LINE p2);

private:
	// 2D array
	int **cfg;
	int size;
	list<int> paths;
	queue<PROG_LINE> q;
	void BFS(PROG_LINE p1, PROG_LINE p2, int reverse);
};

