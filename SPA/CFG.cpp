#include "CFG.h"

// all prog_line need to -1 due to array position
CFG::CFG(int size)
{
	int i,j;
	this->size = size;
	//initiate a graph base on the size
	cfg = new int*[this->size];
	for(i = 0; i < this->size; i++)
		cfg[i] = new int[this->size];
	for(i = 0; i < this->size; i++)
	{
		for(j = 0; j < this->size; j++)
		{
			cfg[i][j] = 0;
		}
	}
}


CFG::~CFG(void)
{
	for(int i = 0; i < this->size; ++i)
		delete [] cfg[i];

	delete [] cfg;
}

bool CFG::isConnected(PROG_LINE p1, PROG_LINE p2)
{
	return (cfg[p1-1][p2-1] == 1);
}

bool CFG::addEdge(PROG_LINE p1, PROG_LINE p2)
{
	if(p1 > size || p2 > size)
		return false;

	// set both way link
	cfg[p1-1][p2-1] = 1;
	cfg[p2-1][p1-1] = 1;

	return true;
}

// can get all path from p1 to p2
// need to store in a list of list of pair?
void CFG::BFS(PROG_LINE p1, PROG_LINE p2)
{
	queue<int> q;
	bool *visited = new bool[size+1];
	for(int i = 0; i <= size; ++i)
		visited[i] = false;

	q.push(p1);
	if(p1 == p2) return;
	visited[p1] = true;
	while(!q.empty())
	{
		int tmpProg = q.back();
		q.pop();
		if(tmpProg == p2)
		{
			cout << "found?\n";
		}

		cout << tmpProg <<"\n";
		for(int j = 1; j<=size; ++j)
		{
			if(CFG::isConnected(tmpProg, j) &&!visited[j])
			{
				q.push(j);
				visited[j] = true;
			}
		}
	}

	delete [] visited;
}