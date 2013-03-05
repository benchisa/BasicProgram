#include "Pattern.h"



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
	QTREE * stmtNode, * paramNode, *varNode, *expr;
	TYPE stmtType,varType, paramType;
	VAR_INDEX varIndex;
	DATA_LIST * data;
	PREFIXEXPR ex;
	bool subFlag;
	pair<int, int> result;
	RELATION_LIST::iterator itr;
	INDEX ctrl;

	//check if query contains Pattern
	//patternTree = patternNode->getRightSibling();
	stmtNode = patternNode->getFirstDescendant();
	stmtType = qtable->at(stmtNode->getData());
	varNode = stmtNode->getFirstDescendant();
	varType = varNode->getType();
	paramNode = varNode->getRightSibling();
	paramType = paramNode->getType();
	varIndex = varNode->getData();
	
	//if variable name is unknown 
	if(varType==QUERYVAR){
		varIndex = qtable->at(varNode->getData());
	}else{
		varIndex= varNode->getData();
	}
	//translate the expression to prefix
	ex = param->find(paramNode->getData())->second;
	if(ex.size()==1){
		ex = "_";
	}else{
		unsigned firstPos = ex.find("_");
		if(firstPos!=std::string::npos){ //subtree expression
			ex = ex.erase(firstPos,2);
			ex = ex.erase(ex.size()-2,2);
			ex = DesignExtractor::convertExprToPrefix(ex);
			subFlag = true;
		}else{ //full tree
			ex = ex.erase(0,1);
			ex = ex.erase(ex.size()-1,1);
			ex = DesignExtractor::convertExprToPrefix(ex);
			subFlag = false;
		}
	}
/*	if(ex != "_") {
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
	*/
	//check if pattern is Assignment/While/If
	if(stmtType == ASSIGNMENT) {
		//check assignment table and return all statements
		data = p->getAllAssigns();
		for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
			ASSIGNENTRY entry = (*p).getAssignEntry(*itr);
			int currentVarTableIndex = entry.varIndex;
			string currentPrefix = entry.prefixTree;
				//compare prefixTree between AssignTable and QueryTree
				if(ex != "_") {// has expression tree
					if(subFlag == true) {
						int r = currentPrefix.find(ex);
						if(r > -1) {
							bool endOfString = r + ex.length()==currentPrefix.length();
							bool matchSubStr = false;
							if(!endOfString){
								string temp = currentPrefix.substr(r+ex.length(), 1);	
								if(temp == " ") matchSubStr = true;
							}
							if(endOfString||matchSubStr) {
								if(varType==QUERYVAR||varType==ANY){
									rlist.push_back(make_pair(*itr, currentVarTableIndex));
								}else if(varType==VARIABLE){
									if(varIndex== currentVarTableIndex)
										rlist.push_back(make_pair(*itr, currentVarTableIndex));
								}	
							}
						}
					}else {
						if(entry.prefixTree == ex) {
							if(varType==QUERYVAR||varType==ANY){
								rlist.push_back(make_pair(*itr, currentVarTableIndex));
							}else if(varType==VARIABLE){
								if(varIndex== currentVarTableIndex)
									rlist.push_back(make_pair(*itr, currentVarTableIndex));
							}	
						}
							}
					}else {//no expression
						if(varType==QUERYVAR||varType==ANY){
							rlist.push_back(make_pair(*itr, currentVarTableIndex));
						}else if(varType==VARIABLE){
							if(varIndex== currentVarTableIndex)
								rlist.push_back(make_pair(*itr, currentVarTableIndex));
						}	
					}
				}
				//data = p->getAllAssigns();
			}else if(stmtType == WHILE) {
				data = p->getAllWhiles();
				for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
					ctrl = p->getWhileCtrVar(*itr);
					//unknown var and "_" won't be checked against current control variable
					if(varType==QUERYVAR||varType==ANY){
						rlist.push_back(make_pair(*itr, ctrl));
					}else if(varType==VARIABLE){
						if(varIndex== ctrl)
							rlist.push_back(make_pair(*itr, ctrl));
					}	
				}
			}else if(stmtType == IF) {
				data = p->getAllIfs();
				for(DATA_LIST::iterator itr = data->begin(); itr != data->end(); itr++) {
					ctrl = p->getIfCtrVar(*itr);
					//unknown var and "_" won't be checked against current control variable
					if(varType==QUERYVAR||varType==ANY){
						rlist.push_back(make_pair(*itr, ctrl));
					}else if(varType==VARIABLE){
						if(varIndex== ctrl)
							rlist.push_back(make_pair(*itr, ctrl));
					}	
				}
			}
	///delete patternNode, qVar, paramNode, varNode, expr;
	return rlist;
}

