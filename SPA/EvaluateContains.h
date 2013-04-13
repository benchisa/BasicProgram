#pragma once
#include "Global_Data_Abstraction.h"
#include "Helper.h"
#include "EvaluateParents.h"

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
	static DATA_LIST		getDataListTypeDescendantOf(TYPE, TYPE , STATEMENT_NUM );
	static DATA_LIST		getDataListTypeAncestorOf(TYPE, TYPE , STATEMENT_NUM );
	static bool				isStatementDescendantTypeDataOf(TYPE, TYPE, int, STATEMENT_NUM);
	static void				isStatementDescendantTypeOf(CONTAIN_LIST*, TYPE, TYPE, int, STATEMENT_NUM);
	static void				getTypeInsertAll(CONTAIN_LIST*,TYPE, TYPE, STATEMENT_NUM, STATEMENT_NUM);
	static void				getParentInsertAll(CONTAIN_LIST*,TYPE, int, STATEMENT_NUM);
	
	static int				findPrefixTreeMatch(TYPE, STATEMENT_NUM, int, int);
	static DATA_LIST		getPrefixTreeVarIndexes(string, int);
	static DATA_LIST		getPrefixTreeConstants(string, int);
	static DATA_LIST		getPrefixTreeOperators(string, TYPE type, int);
};

