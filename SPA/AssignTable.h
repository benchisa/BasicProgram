#pragma once
#include "Global_Data_Abstraction.h"
class AssignTable
{
public:
	AssignTable(void);
	~AssignTable(void);
	
	typedef pair<int,ASSIGNENTRY> ASSIGNROW;
	typedef hash_map<int,ASSIGNENTRY> ASSIGNTABLE;
	
	SIZEX getSize();
	void insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex,string prefixTree);
	ASSIGNENTRY getAssignEntry(STATEMENT_NUM stmtNo);
	DATA_LIST * getAllAssigns();

private:
	ASSIGNTABLE assignTable;
	ASSIGNTABLE::iterator itr;
	DATA_LIST  assignStmtList;
};

