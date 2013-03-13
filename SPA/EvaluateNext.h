#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "CFG.h"

class EvaluateNext
{
public:
	EvaluateNext(void);
	~EvaluateNext(void);

	static PKB* EvaluateNext::pkb;
	static CFG* EvaluateNext::cfg;

	static void			createCFG();
	static bool			isNextStarResult(PROG_LINE, PROG_LINE);
	static bool			isNextResult(PROG_LINE, PROG_LINE);
	static NEXT_LIST	getNextStarResult(PROG_LINE, PROG_LINE);
	static NEXT_LIST	getNextResult(PROG_LINE, PROG_LINE);

private:
	static vector<PROG_LINE> cfgWhileKeepers;
	static AST * buildCFG(AST *);
};

