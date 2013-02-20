#include "IfTable.h"


IfTable::IfTable(void)
{
}


IfTable::~IfTable(void)
{
}

SIZE IfTable::getSize(){

	return ifTable.size();
}
void IfTable::insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex){
	DATA_LIST whileEntry;
	whileEntry.push_back(stmtNo);
	whileEntry.push_back(ctrVarIndex);

	ifTable.push_back(whileEntry);
	ifStmtList.push_back(stmtNo);
}
INDEX IfTable::getCtrVar(STATEMENT_NUM stmtNo){
	
	
	for(itr = ifTable.begin();itr!=ifTable.end();itr++){
		if(itr->at(0)==stmtNo)
			return itr->at(1);
	}

	return NULL;
}
DATA_LIST * IfTable::getIfs(INDEX ctrVarIndex){
	DATA_LIST * returnList = new DATA_LIST();

	for(itr = ifTable.begin();itr!=ifTable.end();itr++){
		if(itr->at(1)==ctrVarIndex){
			returnList->push_back(itr->at(0));
		}
	}
	return returnList;
}
DATA_LIST * IfTable::getAllIfs(){
	DATA_LIST * returnList = new DATA_LIST(ifStmtList);
	return returnList;
}