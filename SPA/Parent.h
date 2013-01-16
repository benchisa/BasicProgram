#include "Global_Data_Abstraction.h"
#include <map>
#include <utility>
#include <list>
#include <algorithm>

class Parent
{

public:
	Parent(void);
	~Parent(void);
	
	bool insertParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	bool isParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	//return a collection of pairs in three cases:
	//1. getParent(empty,s2)
	//2. getParent(s1,empty)
	//3. getParent(empty,empty) all combinations
	PARENT_LIST getParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);

	

private:
	multimap<int, int> *parentDictionary;
typedef pair<int,int> parentPair;
typedef multimap<int, int>::iterator parentItr;
};