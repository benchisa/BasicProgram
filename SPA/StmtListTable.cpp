#include "StmtListTable.h"


StmtListTable::StmtListTable(void)
{
}


StmtListTable::~StmtListTable(void)
{
}

SIZE StmtListTable::getSize(){
	return stmtTable.size();
}
void StmtListTable::insert(STATEMENT_NUM stmtNo,TYPE type){
	//cout << stmtNo << ", " << type << "\n";
	stmtTable[stmtNo] = type;
	stmtList.push_back(stmtNo);
	
}
DATA_LIST * StmtListTable::getAllStmtLists(){
	DATA_LIST * returnList = new DATA_LIST(stmtList);
	return returnList;
}

STMTENTRY StmtListTable::getStmtListEntry(STATEMENT_NUM stmtNo){
	itr = stmtTable.find(stmtNo);
	
	STMTENTRY thisEntry;

	if(itr!=stmtTable.end())
	{
		thisEntry.stmtNo = itr->first;
		thisEntry.type = itr->second;
	}

	return thisEntry;
}