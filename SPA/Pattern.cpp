#include "Pattern.h"
#include <iostream>


Pattern::Pattern(PKB *pkb)
{
	pkb = p;
	ast = pkb->getRootAST();
	varList = pkb->getAllVar();
}


Pattern::~Pattern(void)
{
	delete ast;
	delete p;
	delete varList;
}


RELATION_LIST* Pattern::evaluatePattern(QUERYTABLE * qtable) {
	QUERYTABLE::iterator iter1;
	INDEX qVar;
	INDEX varType, varIndex, varIndex2;
	INDEX opr;
	AST *astNode, *astNode2, *subTree;
	ASTNODE_TYPE nodeType;
	bool flag;
	//traverse the tree
	for(iter1=qtable->begin(); iter1==qtable->end(); iter1++) {
			qVar = iter1->first;
			//check pattern type
			if(qVar == ASSIGNMENT || qVar == WHILE || qVar == IF) {
				//checks if variable is being assigned.
					iter1++;
					varType = iter1->first;
					//checks if variable is being assigned.
					if(varType != VARIABLE && varType != ANY) {
						varIndex = iter1->second;
						iter1++;
						opr = iter1->first;
						astNode = ast->getFirstDescendant();
						while(astNode->getRootData() != NULL) {
						if(astNode->getRootType() == qVar) { //points to the node with the pattern type specified in query
							subTree = astNode->getFirstDescendant();
							//check if variable to be assigned exists in AST
							if(subTree->getRootData() == varIndex) {
								subTree = astNode->getRightSibling();
								varIndex2 = iter1->second;
								varType = iter1->first;
								//if expression contains only variable with no operators
								if(varType == VARIABLE) {
									while(subTree->getRootData() != NULL) {
										if(subTree->getRootData() == varIndex2) {
											flag = true;
											break;
										}else {
											flag = false;
										}
										subTree = subTree->getFirstDescendant();

									}

								}else {
									//check if operators exists in AST and traverse the subtree
									while(subTree->getRootData() != NULL) {
										if(subTree->getRootType() == opr) {
											subTree = subTree->getFirstDescendant();
											if(subTree->getRootData() == varIndex2) {
												iter1++;
												varIndex2 = iter1->second;
												if(subTree->getRightSibling()->getRootData() == varIndex2) {
													subTree = subTree->getRightSibling();
													flag = true;
												}else {
													flag = false;
												}
											}else {
												flag = false;
											}
										}
										iter1++;
										opr = iter1->first;
										subTree = subTree->getFirstDescendant();
									}
								}
								if(flag == true) {
									list->push_back(pair<STATEMENT_NUM, VAR_INDEX>(astNode->getRootStatementNum(), astNode->getFirstDescendant()->getRootData()));
								}
							}
						}
					}
					astNode2 = astNode->getRightSibling();
					if(astNode2 != NULL) {
						astNode = astNode2;
					}else {
						astNode = astNode->getFirstDescendant();
					}
				}else if(varType == VARIABLE || varType == ANY) { //if variable is unknown
					varIndex = iter1->second;
					iter1++;
					opr = iter1->first;
					astNode = ast->getFirstDescendant();
					while(astNode->getRootData() != NULL) {
						//checks if expression is valid
						if(opr == VARIABLE || opr == ANY ) {
							nodeType = astNode->getRootType();
							if(nodeType == ASSIGNMENT || nodeType == WHILE || nodeType == IF) {
								if(astNode->getFirstDescendant()->getRootData() == varIndex) {
									list->push_back(pair<STATEMENT_NUM, VAR_INDEX>(0, varIndex));
								}
							}
							astNode2 = astNode->getRightSibling();
							if(astNode2->getRootData() != NULL) {
								astNode = astNode2;
							}else {
								astNode = astNode->getFirstDescendant();
							}
						}else {
									//check if operators exists in AST and traverse the subtree
									while(subTree != NULL) {
										if(subTree->getRootType() == opr) {
											subTree = subTree->getFirstDescendant();
											if(subTree->getRootData() == varIndex2) {
												iter1++;
												varIndex2 = iter1->second;
												if(subTree->getRightSibling()->getRootData() == varIndex2) {
													subTree = subTree->getRightSibling();
													flag = true;
												}else {
													flag = false;
												}
											}else {
												flag = false;
											}
										}
										iter1++;
										opr = iter1->first;
										subTree = subTree->getFirstDescendant();
									}	
								if(flag == true) {
									list->push_back(pair<STATEMENT_NUM, VAR_INDEX>(astNode->getRootStatementNum(), astNode->getFirstDescendant()->getRootData()));
								}
						}
					}

				}

		}
	}
	//delete astNode, astNode2, subTree;
	return list;
}

