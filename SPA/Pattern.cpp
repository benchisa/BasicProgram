#include "Pattern.h"
#include <iostream>


Pattern::Pattern(PKB *pkb)
{
	p = pkb;
	ast = pkb->getRootAST();
	varList = pkb->getAllVar();
}


Pattern::~Pattern(void)
{
	delete ast;
	delete p;
	delete varList;
}


RELATION_LIST* Pattern::evaluatePattern(QTREE * qtree, QUERYTABLE * qtable, QUERYPARAM * param) {
	QTREE *currentNode, *rootNode, * patternNode, * qVar, * paramNode, *varNode;
	TYPE pType, qType, paramType;
	int	 qVarIndex, varIndex;
	string subexp, varName;

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
				if(qType == QUERYVAR) {

				}else {

					varIndex = varNode->getData();
					varName = p->getVarName(varIndex);

				}
				//check if subtree exists in AST
				if(paramType == ANY) {
					//return all statements if the variable is found
				}else {
					//translate the expression to prefix
				}

			//check if pattern is Assignment/While/If
			if(pType == ASSIGNMENT) {

				//check assignment table and return all statements

			}else if(pType == WHILE) {

				//check while table

			}else if(pType == IF) {

				//check if table

			}
		}

	//}
	delete currentNode, rootNode, patternNode, qVar, paramNode, varNode;
	return list;
}

