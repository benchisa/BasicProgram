#pragma once
#include "Global_Data_Abstraction.h"
#include <stack>
#include "PKB.h"
#include "CFG.h"
#include "EvaluateNext.h"

// Static Functions to Help

class Helper
{
public:
	Helper(void);
	~Helper(void);

	// to set PKB
	static PKB *pkb;
	
	static PREFIXEXPR	convertExprToPrefix(EXPRESSION expr);
	static bool			isStatementTypeOf(TYPE typeName, STATEMENT_NUM stmtNum);
	static DATA_LIST *	getStmtListOf(TYPE nodeType);
	static TYPE			getStatementType(STATEMENT_NUM stmtNum);
	static DATA_LIST *	getAllConstants();
	static DATA_LIST *	getAllCallStmts();
	static bool			isStatement(TYPE typeName);
	static bool			isExprElement(TYPE typeName);

	static void			computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &listLst, TYPE type1, TYPE type2);
	static void			iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmpLst, int v1);

private:
	// Helper for conversion
	static bool isHigherPrecedence(stack<pair<TYPE, TOKEN>>& operators, TOKEN);
	static vector<pair<TYPE, TOKEN>> tokenize(EXPRESSION);
	static OPERATOR operatorToString(TYPE type);
	static EXPRESSION formExpression(stack<pair<TYPE, TOKEN>>& operators,stack<OPERAND>& operands);
};

