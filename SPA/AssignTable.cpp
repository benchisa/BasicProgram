#include "AssignTable.h"


AssignTable::AssignTable(void)
{

}


AssignTable::~AssignTable(void)
{
}

SIZE AssignTable::getSize(){
	return assignTable.size();
}
void AssignTable::insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex,string prefixTree){
	ASSIGNENTRY newEntry;
	newEntry.varIndex = ctrVarIndex;
	newEntry.prefixTree = prefixTree;

	assignTable.insert(ASSIGNROW(stmtNo,newEntry));
	assignStmtList.push_back(stmtNo);
	
}
ASSIGNENTRY AssignTable::getAssignEntry(STATEMENT_NUM stmtNo){
	itr = assignTable.find(stmtNo);
	
	if(itr!=assignTable.end())
		return itr->second;
}
DATA_LIST * AssignTable::getAllAssigns(){
	DATA_LIST * returnList = new DATA_LIST(assignStmtList);
	return returnList;
}