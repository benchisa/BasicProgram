#pragma once
#include "Global_Data_Abstraction.h"
#include <queue>

class CFG
{
public:
	CFG(int size);
	~CFG(void);
	bool isConnected(PROG_LINE p1, PROG_LINE p2);
	bool addEdge(PROG_LINE p1, PROG_LINE p2);
	void BFS(PROG_LINE p1, PROG_LINE p2);
private:
	// 2D array
	int **cfg;
	int size;
};

