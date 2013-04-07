#pragma once
#include "Global_Data_Abstraction.h"

class WhileTable
{
public:
	WhileTable(void);
	~WhileTable(void);

	typedef RAWDATA WHILETABLE;

	SIZEX getSize();
	void insert(STATEMENT_NUM stmtNo,INDEX ctrVarIndex);
	INDEX getCtrVar(STATEMENT_NUM stmtNo);
	DATA_LIST * getWhiles(INDEX ctrVarIndex);
	DATA_LIST * getAllWhiles();

private:
	WHILETABLE whileTable;
	DATA_LIST whileStmtList;
	WHILETABLE::iterator itr;
};

