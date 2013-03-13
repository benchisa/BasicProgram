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

	//cout << "Inserting Next(" << p1 << ", " << p2 << ")\n";
	cfg[p1-1][p2-1] = 1;

	return true;
}



list<PROG_LINE> CFG::getAllProgLines(PROG_LINE p1, PROG_LINE p2)
{
	//cout << "p1,p2: (" << p1 << "," << p2 << ")\n";
	if((p1 != NULL && p2 == NULL) || (p1 == NULL && p2 == NULL) || (p1 != NULL && p2 != NULL))
		computeProgLines(p1, p2, 0);
	else
		computeProgLines(p1, p2, 1);

	// debug
	list<PROG_LINE>::iterator itr = pLines.begin();
	while(itr!=pLines.end()){
		itr++;
	}

	return this->pLines;
}

NEXT_LIST CFG::getAllPaths(PROG_LINE p1, PROG_LINE p2){
	if((p1 != NULL && p2 == NULL) || (p1 == NULL && p2 == NULL) || (p1 != NULL && p2 != NULL))
		computeProgLines(p1, p2, 0);
	else
		computeProgLines(p1, p2, 1);

	
	return paths;
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
					tmp.push_back(make_pair(i,j));
				}
			}
		}
	}
	// Next(1, n1)
	else if (p1 != NULL && p2 == NULL){
		for(int i = 1; i <= size; i++)
		{
			if(isConnected(p1, i)){
				tmp.push_back(make_pair(p1,i));
			}
		}
	}
	// Next(n1, 2)
	else if (p1 == NULL && p2 != NULL){
		for(int i = 1; i <= size; i++)
		{
			if(isConnected(i, p2)){
				tmp.push_back(make_pair(i, p2));
			}
		}
	}
	// this case shld never happen.. but for sake of error checking
	else if(p1 != NULL && p2 != NULL){
		tmp.push_back(make_pair(p1,p2));
	}
	//cout << "GETNEXT IN CFG: " << tmp.size() << "\n";
	return tmp;
}

bool CFG::isConnected(PROG_LINE p1, PROG_LINE p2)
{
	return (cfg[p1-1][p2-1] == 1);
}

//return the PROG_LINEs that connects between p1 and p2
//return the PROG_LINEs that connects from p1 to any other possibilities
//return the PROG_LINEs that connects to p2 from any other possibilities
void CFG::computeProgLines(PROG_LINE p1, PROG_LINE p2, int reverse)
{
	// save time strategy
	if(p1 > size || p2 > size) return; // invalid case
	if(p1 <= 0 && p2 <= 0) return; // invalid case
	
	if(pLines.size() != 0)
		pLines.clear();

	if(paths.size() != 0)
		paths.clear();

	
	queue<PROG_LINE> q;
	//cout << "compute: " << p1 << ", " << p2 << "\n";

	int same = 0;
	bool **visited = new bool*[size+1];
	for(int i = 0; i <= size; ++i){
		visited[i] = new bool[size+1];
	}

	for(int i = 0; i <= size; ++i)
	{
		for(int j = 0; j <= size; ++j)
		{
			visited[i][j] = false;
		}
	}
	//cout << p1 << " ";
	if(!reverse){
		q.push(p1);
		//visited[p1] = true;
	}
	else{
		q.push(p2);
		//visited[p2] = true;
	}

	while(!q.empty())
	{
		int tmpProg = q.front();
		
		if(!q.empty()) 
			q.pop();

		// reach destination
		if(p1 != NULL && p2 != NULL && tmpProg == p2 && p1 != p2)
			break;

		for(int j = 1; j<=size; j++)
		{
			if(reverse){
				if(isConnected(j, tmpProg) && (j==p2||!visited[j][tmpProg]))
				{
					//cout << "(" << j << "," << tmpProg << ")\n";
					
					pLines.push_back(j);
					paths.push_back(make_pair(j, tmpProg));

					q.push(j);
					visited[j][tmpProg] = true;
				}
			}
			else
			{
				if(isConnected(tmpProg, j) && (j==p1 || !visited[tmpProg][j]) )
				{
					
					//cout << "(" << tmpProg << "," << j << ")\n";
					
					//cout << j << " ";
					pLines.push_back(j);
					paths.push_back(make_pair(tmpProg, j));
					
					q.push(j);
					visited[tmpProg][j] = true;

					if(p1 != NULL && p2 != NULL && j == p2 && p1 != p2)
						break;
				}
			}
			same = 0;
		}
	}
	pLines.sort();
	pLines.unique();
	//paths.sort();
	paths.unique();
	// free memory
	delete [] visited;
}
