#include "CFG.h"
#include <stack>

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
			//cout << "[" << i << "][" << j << "]\n";
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

	cfg[p1-1][p2-1] = 1;
	//cfg[p2-1][p1-1] = 1;

	return true;
}

list<int> CFG::findAll(PROG_LINE p1, PROG_LINE p2)
{
	BFS(p1, p2, 0);
	return this->paths;
}

list<int> CFG::findAllReverse(PROG_LINE p1, PROG_LINE p2)
{
	BFS(p1, p2, 1);
	return this->paths;
}

// can get all path from p1 to p2
// need to store in a list of list of pair?
void CFG::BFS(PROG_LINE p1, PROG_LINE p2, int reverse)
{
	if(paths.size() != 0)
		paths.clear();

	bool *visited = new bool[size+1];
	for(int i = 0; i <= size; ++i)
		visited[i] = false;

	q.push(p1);
	
	PROG_LINE prev, last;
	if(p1 == p2){
		prev = p2;
		p2 = p2 - 1;	
	}
	visited[p1] = true;
	

	while(!q.empty())
	{
		int tmpProg = q.front();
		if(!q.empty()) 
			q.pop();

		if(tmpProg == p2){
			if(p2!=0 && CFG::isConnected(p2, prev)){
				paths.push_back(prev);
			}
			if(!q.empty()) q.pop();
			if(q.empty()) break;
			tmpProg = q.front();
		}
		
		findPaths(tmpProg, visited, reverse);
	}
	
	paths.sort();
	paths.unique();
	delete [] visited;
}

void CFG::findPaths(PROG_LINE tmpProg, bool *visited, int reverse){
	for(int j = 1; j<=size; j++)
		{
			if(reverse){
				if(CFG::isConnected(j, tmpProg) &&!visited[j])
				{
					//cout << "(" << tmpProg << "," << j << ")\n";
					paths.push_back(j);
					//cout << "push: " << j << "\n";
					q.push(j);
				}
			}
			else
			{
				if(CFG::isConnected(tmpProg, j) &&!visited[j])
				{
					//cout << "(" << tmpProg << "," << j << ")\n";
					paths.push_back(j);
					//cout << "push: " << j << "\n";
					q.push(j);
					visited[j] = true;
				}
			}
		}
}