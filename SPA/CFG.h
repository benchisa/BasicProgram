#pragma once
#include "Global_Data_Abstraction.h"
#include <queue>
#include <stack>

class CFG
{
public:
	CFG(int size);
	~CFG(void);
	bool addEdge(PROG_LINE p1, PROG_LINE p2);
	bool isNext(PROG_LINE p1, PROG_LINE p2);
	NEXT_LIST getNext(PROG_LINE p1, PROG_LINE p2);
	list<PROG_LINE> findAllPaths(PROG_LINE p1, PROG_LINE p2);

private:
	// 2D array
	int **cfg;
	int size;
	list<PROG_LINE> paths;
	queue<PROG_LINE> q;
	bool isConnected(PROG_LINE p1, PROG_LINE p2);
	void BFS(PROG_LINE p1, PROG_LINE p2, int reverse);
};

