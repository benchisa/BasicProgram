#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"

class EvaluateContains
{
public:
	EvaluateContains(void);
	~EvaluateContains(void);

	static PKB *			pkb;
	static bool				getIsContainResult(TYPE, int, TYPE, int);
	static bool				getIsContainStarResult(TYPE, int, TYPE, int);
	static CONTAIN_LIST		getContainResult(TYPE, int, TYPE, int);
	static CONTAIN_LIST		getContainStarResult(TYPE, int, TYPE, int);


private:
	static void				computeContain();
	static void				computeContainStar();
	static DATA_LIST		getDataListTypeDescendantOf(TYPE typeName, TYPE typeName2, STATEMENT_NUM stmtNum);
	static bool				isStatementDescendantTypeDataOf(TYPE typeName, TYPE typeName2, int data, STATEMENT_NUM stmtNum);
	static DATA_LIST		getDataListTypeAncestorOf(TYPE typeName, TYPE typeName2, STATEMENT_NUM stmtNum);

};

