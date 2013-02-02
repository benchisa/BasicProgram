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

bool CFG::addEdge(PROG_LINE p1, PROG_LINE p2)
{
	if(p1 > size || p2 > size)
		return false;

	if(p1 <= 0 && p2 <= 0)
		return false;

	cfg[p1-1][p2-1] = 1;
	//cfg[p2-1][p1-1] = 1;

	return true;
}

list<int> CFG::findAll(PROG_LINE p1, PROG_LINE p2)
{
	if((p1 != 0 && p2 == 0) || (p1 == 0 && p2 == 0) || (p1 != 0 && p2 != 0))
		BFS(p1, p2, 0);
	else
		BFS(p1, p2, 1);
	return this->paths;
}

bool CFG::isNext(PROG_LINE p1, PROG_LINE p2)
{
	if(p1 > size || p2 > size || p1 <= 0 || p2 <= 0) return false;
	return isConnected(p1, p2);
}

NEXT_LIST CFG::getNext(PROG_LINE p1, PROG_LINE p2){
	NEXT_LIST tmp;
	
	// Next(n1, n2)
	if(p1 == NULL && p2 == NULL){
		for(int i = 1; i <= size; i++)
		{
			for(int j = 1; j <= size; j++)
			{
				if(isConnected(i, j)){
					pair<PROG_LINE, PROG_LINE> tPair;
					tPair.first = i;
					tPair.second = j;
					tmp.push_back(tPair);
				}
			}
		}
	}
	// Next(1, n1)
	else if (p1 != NULL && p2 == NULL){
		for(int i = 1; i <= size; i++)
		{
			if(isConnected(p1, i)){
				pair<PROG_LINE, PROG_LINE> tPair;
				tPair.first = p1;
				tPair.second = i;
				tmp.push_back(tPair);
			}
		}
	}
	// Next(n1, 2)
	else if (p1 == NULL && p2 != NULL){
		for(int i = 1; i <= size; i++)
		{
			if(isConnected(i, p2)){
				pair<PROG_LINE, PROG_LINE> tPair;
				tPair.first = i;
				tPair.second = p2;
				tmp.push_back(tPair);
			}
		}
	}
	// this case shld never happen.. but for sake of error checking
	else if(p1 != NULL && p2 != NULL){
		pair<PROG_LINE, PROG_LINE> tPair;
		tPair.first = p1;
		tPair.second = p2;
		tmp.push_back(tPair);
	}
	return tmp;
}

bool CFG::isConnected(PROG_LINE p1, PROG_LINE p2)
{
	return (cfg[p1-1][p2-1] == 1);
}

//return the PROG_LINEs that connects between p1 and p2
//return the PROG_LINEs that connects from p1 to any other possibilities
//return the PROG_LINEs that connects to p2 from any other possibilities
void CFG::BFS(PROG_LINE p1, PROG_LINE p2, int reverse)
{
	// save time strategy
	if(p1 > size || p2 > size) return; // invalid case
	if(p1 <= 0 && p2 <= 0) return; // invalid case
	

	if(paths.size() != 0)
		paths.clear();

	bool *visited = new bool[size+1];
	for(int i = 0; i <= size; ++i)
		visited[i] = false;

	PROG_LINE prev, last;
	if(!reverse){
		q.push(p1);
		visited[p1] = true;
	}
	else{
		q.push(p2);
		visited[p2] = true;
	}

	while(!q.empty())
	{
		int tmpProg = q.front();
		
		if(!q.empty()) 
			q.pop();

		for(int j = 1; j<=size; j++)
		{
			if(reverse){
				if(isConnected(j, tmpProg) && (j==p2||!visited[j]))
				{
					//cout << "(" << j << "," << tmpProg << ")\n";
					paths.push_back(j);
					q.push(j);
					visited[j] = true;
				}
			}
			else
			{
				if(isConnected(tmpProg, j) && (j==p1 || !visited[j]) )
				{
					//cout << "(" << tmpProg << "," << j << ")\n";
					paths.push_back(j);
					q.push(j);
					visited[j] = true;
				}
			}
		}
	}

	paths.sort();
	paths.unique();
	delete [] visited;
}