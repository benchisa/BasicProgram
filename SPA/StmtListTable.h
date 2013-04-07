#pragma once
#include "Global_Data_Abstraction.h"
class StmtListTable
{
public:
	StmtListTable(void);
	~StmtListTable(void);

	typedef hash_map<int,STMTENTRY> STMTLISTTABLE;
	
	SIZEX getSize();
	void insert(STATEMENT_NUM stmtNo,STMTENTRY entry);
	STMTENTRY getStmtListEntry(STATEMENT_NUM stmtNo);
	DATA_LIST * getAllStmtLists();

private:
	STMTLISTTABLE stmtTable;
	STMTLISTTABLE::iterator itr;
	DATA_LIST stmtList;
};

