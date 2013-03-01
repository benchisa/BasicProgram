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


RELATION_LIST Pattern::evaluatePattern(QTREE * patternNode, QUERYTABLE * qtable, QUERYPARAM * param) {
	QTREE * qVar, * paramNode, *varNode, *expr, *patternTree;
	TYPE pType, qType,varType, paramType;
	VAR_INDEX varIndex,varTableIndex;
	DATA_LIST * data;
	PREFIXEXPR ex;
	bool subFlag;
	pair<int, int> result;
	RELATION_LIST::iterator itr;
	INDEX ctrl;

	//check if query contains Pattern
	//patternTree = patternNode->getRightSibling();
	qVar = patternNode->getFirstDescendant();
	pType = qtable->at(qVar->getData());
	varNode = qVar->getFirstDescendant();
	varType = varNode->getType();
	paramNode = varNode->getRightSibling();
	paramType = paramNode->getType();
	qType = varNode->getType();
	varIndex = varNode->getData();
	
	//if variable name is unknown 
	if(varType==QUERYVAR){
		varTableIndex = qtable->at(varIndex);
	}
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
		data = p->getAllAssigns();
		for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
			ASSIGNENTRY entry = (*p).getAssignEntry(*itr);
			int currentVarTableIndex = entry.varIndex;
				//compare prefixTree between AssignTable and QueryTree
				if(ex != "_") {// has expression tree
					if(subFlag == true) {
						int r = (entry.prefixTree).find(ex);
						if(r > -1) {
							string temp = entry.prefixTree.substr(r+ex.length(), 1);
							if(temp == "" || temp == " ") {
								if(varType==QUERYVAR||varType==ANY){
									rlist.push_back(make_pair(*itr, currentVarTableIndex));
								}else if(varType==VARIABLE){
									if(varTableIndex== currentVarTableIndex)
										rlist.push_back(make_pair(*itr, currentVarTableIndex));
								}	
							}
						}
					}else {
						if(entry.prefixTree == ex) {
							if(varType==QUERYVAR||varType==ANY){
								rlist.push_back(make_pair(*itr, currentVarTableIndex));
							}else if(varType==VARIABLE){
								if(varTableIndex== currentVarTableIndex)
									rlist.push_back(make_pair(*itr, currentVarTableIndex));
							}	
						}
							}
					}else {//no expression
						if(varType==QUERYVAR||varType==ANY){
							rlist.push_back(make_pair(*itr, currentVarTableIndex));
						}else if(varType==VARIABLE){
							if(varTableIndex== currentVarTableIndex)
								rlist.push_back(make_pair(*itr, currentVarTableIndex));
						}	
					}
				}
				//data = p->getAllAssigns();
			}else if(pType == WHILE) {
				data = p->getAllWhiles();
				for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
					ctrl = p->getWhileCtrVar(*itr);
					//unknown var and "_" won't be checked against current control variable
					if(varType==QUERYVAR||varType==ANY){
						rlist.push_back(make_pair(*itr, ctrl));
					}else if(varType==VARIABLE){
						if(varTableIndex== ctrl)
							rlist.push_back(make_pair(*itr, ctrl));
					}	
				}
			}else if(pType == IF) {
				data = p->getAllIfs();
				for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
					ctrl = p->getIfCtrVar(*itr);
					//unknown var and "_" won't be checked against current control variable
					if(varType==QUERYVAR||varType==ANY){
						rlist.push_back(make_pair(*itr, ctrl));
					}else if(varType==VARIABLE){
						if(varTableIndex== ctrl)
							rlist.push_back(make_pair(*itr, ctrl));
					}	
				}
			}
	///delete patternNode, qVar, paramNode, varNode, expr;
	return rlist;
}

