#include "WhileTable.h"


WhileTable::WhileTable(void)
{
}


WhileTable::~WhileTable(void)
{
}

SIZE WhileTable::getSize(){

	return whileTable.size();
}
void WhileTable::insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex){
	DATA_LIST whileEntry;
	whileEntry.push_back(stmtNo);
	whileEntry.push_back(ctrVarIndex);

	whileTable.push_back(whileEntry);
	whileStmtList.push_back(stmtNo);
}
INDEX WhileTable::getCtrVar(STATEMENT_NUM stmtNo){
	
	
	for(itr = whileTable.begin();itr!=whileTable.end();itr++){
		if(itr->at(0)==stmtNo)
			return itr->at(1);
	}

	return NULL;
}
DATA_LIST * WhileTable::getWhiles(INDEX ctrVarIndex){
	DATA_LIST * returnList = new DATA_LIST();

	for(itr = whileTable.begin();itr!=whileTable.end();itr++){
		if(itr->at(1)==ctrVarIndex){
			returnList->push_back(itr->at(0));
		}
	}
	return returnList;
}
DATA_LIST * WhileTable::getAllWhiles(){
	DATA_LIST * returnList = new DATA_LIST(whileStmtList);
	return returnList;
}