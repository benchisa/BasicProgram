#include "DesignExtractor.h"


DesignExtractor::DesignExtractor(PKB* pkb)
{
	this->pkb = pkb;
}


DesignExtractor::~DesignExtractor(void)
{
}

bool DesignExtractor::isStatementTypeOf(TYPE typeName,STATEMENT_NUM stmtNum){
	//find the type of statement
	AST_LIST* currentAST = pkb->getASTBy(stmtNum);
	AST_LIST::iterator itr;
	for(itr = currentAST->begin();itr!=currentAST->end();itr++){
		if((*itr)->getRootType()==typeName) return true;
	}
	return false;
}