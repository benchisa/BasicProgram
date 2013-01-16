#pragma once
#include "Global_Data_Abstraction.h"
#include <unordered_map>
class Follows
{
public:
	Follows(void);
	~Follows(void);
	
	bool insertFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	bool isFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);

	//take care of three cases: getFollows(2,empty), getFollows(empty,3),
	//                          getFollows(empty,empty) which returns all combinations
	FOLLOWS_LIST getFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);


private:
	typedef pair<int,int> followsPair;
unordered_map<int,int> *followsTable;
unordered_map<int,int>::iterator followsItr;

	
};
