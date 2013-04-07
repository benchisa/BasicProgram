#include "StmtListTable.h"


StmtListTable::StmtListTable(void)
{
}


StmtListTable::~StmtListTable(void)
{
}

SIZEX StmtListTable::getSize(){
	return stmtTable.size();
}
void StmtListTable::insert(STATEMENT_NUM stmtNo,STMTENTRY entry){
	//cout << stmtNo << ", " << type << "\n";
	stmtTable[stmtNo] = entry;
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
		thisEntry.ownerNo = itr->second.ownerNo;
		thisEntry.type = itr->second.type;
	}

	return thisEntry;
}