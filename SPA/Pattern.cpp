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


RELATION_LIST Pattern::evaluatePattern(QTREE * qtree, QUERYTABLE * qtable, QUERYPARAM * param) {
	QTREE *currentNode, *rootNode, * patternNode, * qVar, * paramNode, *varNode, *expr;
	TYPE pType, qType, paramType;
	VAR_INDEX qVarIndex, varIndex;
	string subexp, varName;
	DATA_LIST * data;
	ASSIGNENTRY entry;
	PREFIXEXPR ex;
	bool subFlag;
	pair<int, int> result;
	RELATION_LIST::iterator itr;

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

				varIndex = varNode->getData();
				//if variable name is unknown 
				if(varIndex < 1) {
					varIndex = 0;
				}
				//translate the expression to prefix
				ex = DesignExtractor::convertExprToPrefix(param->find(paramNode->getData())->second);
					if(ex != "_") {
						unsigned firstPos = ex.find("\"");
						while(firstPos != -1) {
							ex = ex.erase(firstPos, 1);
							firstPos = ex.find("\"");
						}
						//look for underscore in the expression.
						firstPos = ex.find("_");
						if(firstPos != -1) {
							subFlag = true;
							while(firstPos != -1) {
								ex = ex.erase(firstPos, 1);
								firstPos = ex.find("_");
							}
						}else {
							subFlag = false;
						}
					}

			//check if pattern is Assignment/While/If
			if(pType == ASSIGNMENT) {
				//check assignment table and return all statements
				for(int x = 1; x != p->getMaxStatementNum()+1; x++) {
					entry = p->getAssignEntry(x);
					//If QueryVar is unknown.
					//if(varIndex == 0) {
						//compare prefixTree between AssignTable and QueryTree
						if(ex != "_") {
							if(subFlag == true) {
								int r = (entry.prefixTree).find(ex);
								if(r > -1) {
									rlist.push_back(make_pair(x, varIndex));
								}
							}else {
								if(entry.prefixTree == ex) {
									rlist.push_back(make_pair(x, varIndex));
								}
							}
						}else {
							//data = p->getAllAssigns();
							INDEX rVar = entry.varIndex;
							if(rVar == varIndex) {
									rlist.push_back(make_pair(x, varIndex));
							}
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
		}

	//}
	delete currentNode, rootNode, patternNode, qVar, paramNode, varNode;
	return rlist;
}

