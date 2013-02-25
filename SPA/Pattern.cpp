#include "Pattern.h"
#include "DesignExtractor.h"
#include <iostream>


Pattern::Pattern(PKB *pkb)
{
	p = pkb;
	//ast = pkb->getRootAST();
	//varList = pkb->getAllVar();
}


Pattern::~Pattern(void)
{
	//delete ast;
	//delete p;
	//delete varList;
}


RELATION_LIST* Pattern::evaluatePattern(QTREE * qtree, QUERYTABLE * qtable, QUERYPARAM * param) {
	QTREE *currentNode, *rootNode, * patternNode, * qVar, * paramNode, *varNode, *expr;
	TYPE pType, qType, paramType;
	int	 qVarIndex, varIndex;
	string subexp, varName;
	DATA_LIST * data;
	ASSIGNENTRY entry;

	currentNode = qtree->getFirstDescendant();
	patternNode = qtree->getRightSibling();
	//check if query contains Pattern
		if(patternNode->getType() == PATTERN) {
			qVar = patternNode->getFirstDescendant();
			pType = qtable->at(qVar->getData());
				varNode = qVar->getFirstDescendant();
				paramNode = varNode->getRightSibling();
				paramType = paramNode->getType();
				qType = varNode->getType();

				//checks if variable to be assigned exists in Vartable
				/*if(qType == QUERYVAR) {

				}else {*/

					varIndex = varNode->getData();
					//varName = p->getVarName(varIndex);

				//}
				//check if subtree exists in AST
				if(paramType == ANY) {
					//return all statements if the variable is found
				}else {
					//translate the expression to prefix
					expr = paramNode->getFirstDescendant();
					DesignExtractor::convertExprToPrefix(expr->getData());
				}

			//check if pattern is Assignment/While/If
			if(pType == ASSIGNMENT) {
				//check assignment table and return all statements
				for(int x = 1; x < p->getMaxStatementNum()+1; x++) {
					entry = p->getAssignEntry(x);
					if(entry.varIndex == varIndex && varIndex > 0) {
					list->push_back(pair<STATEMENT_NUM, VAR_INDEX>(data->at(x), varIndex));
					}
				}
				//data = p->getAllAssigns();
			}else if(pType == WHILE) {

				//check while table
				data = p->getAllWhiles();
			}else if(pType == IF) {

				//check if table
				data = p->getAllIfs();
			}
			//DATA_LIST::iterator itr;
			for(int i = 0; i < data->size(); i++) {
				entry = p->getAssignEntry(data->at(i));
				if(entry.varIndex == varIndex) {
					list->push_back(pair<STATEMENT_NUM, VAR_INDEX>(data->at(i), varIndex));
				}
			}
		}

	//}
	delete currentNode, rootNode, patternNode, qVar, paramNode, varNode;
	return list;
}

