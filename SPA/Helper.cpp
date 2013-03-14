#include "Helper.h"

PKB * Helper::pkb = NULL;

Helper::Helper(void)
{
}


Helper::~Helper(void)
{
}
/*** PUBLIC ***/
PREFIXEXPR Helper::convertExprToPrefix(EXPRESSION expr){
	PREFIXEXPR result;

	stack<pair<TYPE, TOKEN>> operators;
	stack<OPERAND> operands;

	vector<pair<TYPE, TOKEN>> s = tokenize(expr); 
	pair<TYPE, TOKEN> tPair;

	vector<pair<TYPE, TOKEN>>::iterator itr = s.begin();
	while(itr != s.end()){

		// If it is a left parentheses or the stack is empty or higher precendence
		if(itr->first == STRING){
			//cout << "Pushing: " << itr->second << " to operands\n";
			operands.push(itr->second);
		}
		else if(itr->second == "(" || operators.empty() || isHigherPrecedence(operators,itr->second)){
			//cout << "Pushing: " << itr->first << " to operators\n";
			operators.push(*itr);
		}
		else if(itr->second == ")"){
			while(operators.top().second != "("){
				operands.push(formExpression(operators, operands));
			}

			// remove (
			operators.pop();
		}
		// lower precedence
		else if(!isHigherPrecedence(operators,itr->second)){
			while(!operators.empty() && !isHigherPrecedence(operators,operators.top().second) && operators.top().second != "("){
				operands.push(formExpression(operators, operands));
			}
			operators.push(*itr);
		}

		itr++;
	}

	// handle the leftover
	while(!operators.empty()){
		operands.push(formExpression(operators, operands));
	}

	result = operands.top();
	operands.pop();
	return result;
}

bool Helper::isStatementTypeOf(TYPE typeName,STATEMENT_NUM stmtNum){
	//find the type of statement
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){

		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName) return true;
		}
	}
	return false;
}

// only for PLUS, MINUS, TIMES
bool Helper::isStatementDescendantTypeDataOf(TYPE typeName, TYPE typeName2, int data, STATEMENT_NUM stmtNum){
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		cout << typeName << ", " << typeName2 << ", " << data << ", " << stmtNum << "\n";
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName){
				// check descendant
				AST * descendant = (*itr)->getFirstDescendant();
				if(pkb->getType(descendant) == typeName2 && pkb->getData(descendant) == data){
					return true;
				}
				else if(pkb->getType(descendant->getRightSibling()) == typeName2 && pkb->getData(descendant->getRightSibling()) == data){{
					return true;
				}
				}
			}
		}
	}
	
	return false;
}

bool Helper::isStatementDescendantTypeOf(TYPE typeName, TYPE typeName2,STATEMENT_NUM stmtNum){
	//find the type of statement
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){

		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName && ((*itr)->getFirstDescendant()->getRootType()||(*itr)->getFirstDescendant()->getRightSibling()->getRootType())){
				// check descendant

			}
		}
	}
	return false;
}

AST_LIST *	Helper::getASTListTypeOf(TYPE typeName, STATEMENT_NUM stmtNum){
	AST_LIST* resultLst = NULL;

	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){

		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName) {
				cout << "Pushing: " << typeName << "\n";
				resultLst->push_back((*itr)->getFirstDescendant());
				resultLst->push_back((*itr)->getFirstDescendant()->getRightSibling());
			}
		}
	}
	return resultLst;
}

TYPE Helper::getStatementType(STATEMENT_NUM stmtNum){
	//find the type of statement
	if(stmtNum>0&&stmtNum <= pkb->getMaxStatementNum()){
		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			return (*itr)->getRootType();
		}
	}
}

DATA_LIST* Helper::getAllCallStmts(){
	return pkb->getAllCallerStmt();
}

DATA_LIST* Helper::getStmtListOf(TYPE nodeType){
	DATA_LIST* returnList = NULL;

	int maxStmtNum = pkb->getMaxStatementNum();
	int maxProgLineNum = pkb->getMaxProgLine();

	switch(nodeType){
	case PROGLINE:
		for(int i=1; i<=maxProgLineNum;i++){
			returnList->push_back(i);
		}
		break;
	case STATEMENT:
		for(int i=1; i<=maxStmtNum;i++){
			returnList->push_back(i);
		}
		break;
	case ASSIGNMENT:
		returnList = (*pkb).getAllAssigns();
		break;
	case WHILE:
		returnList = pkb->getAllWhiles();
		break;
	case IF:
		returnList = pkb->getAllIfs();
		break;
	case CALL:
		returnList = pkb->getAllCallerStmt();
		break;
	case VARIABLE:
		{
			int varSize = pkb->getVarTableSize();
			for(int i =1;i<=varSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	case PROCEDURE:
		{
			int procSize = pkb->getProceTableSize();
			for(int i =1;i<=procSize;i++){
				returnList->push_back(i);
			}
		}
		break;

	case CONSTANT:
		{
			int constSize = pkb->getConstantTableSize();
			for(int i=1;i<=constSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	case STMT_LIST:
		{
			returnList = pkb->getAllStmtLst();
		}
	}
	return returnList;
}

DATA_LIST * Helper::getAllConstants(){
	DATA_LIST * returnList = new DATA_LIST();

	int size = pkb->getConstantTableSize();
	for(int i =1;i<=size;i++){
		returnList->push_back(pkb->getConstantValue(i));
	}
	return returnList;
}

// Computational Methods
void Helper::computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &tmpLst, TYPE type1, TYPE type2){
	PARENT_LIST::iterator tmpItr;
	AST_LIST *astLst_t1, *astLst_t2;
	AST_LIST::iterator astItr_t1, astItr_t2;

	// Parent(s1, s2)/ Parent(_, _)/Parent(ANY, s2)/Parent(s1, ANY)
	if((type1 == STATEMENT && type2 == STATEMENT ) || (type1== ANY && type2 == ANY)
		|| (type1 == STATEMENT && type2 == ANY) || (type1 == ANY && type2==STATEMENT)){
			result = tmpLst;
	}

	// Parent(_, type2), Parent(type1, _)
	// Parent(STATEMENT, type2)
	else if(type1 == ANY || type2 == ANY 
		|| type1 == STATEMENT || type2 == STATEMENT){

			tmpItr = tmpLst.begin();
			while(tmpItr != tmpLst.end()){

				// Decide which statement to check with type
				if(type1 == ANY || type1 == STATEMENT) 
					astLst_t1 = pkb->getASTBy(tmpItr->second);
				else if(type2 == ANY || type2 == STATEMENT) 
					astLst_t1 = pkb->getASTBy(tmpItr->first);

				astItr_t1 = astLst_t1->begin();
				while(astItr_t1 != astLst_t1->end())
				{
					if(pkb->getType(*astItr_t1) == type2 || pkb->getType(*astItr_t1) == type1)
						result.push_back(*tmpItr);

					astItr_t1++;
				}

				tmpItr++;
			}
	}

	else if(type1 == WHILE || type1 == ASSIGNMENT || type1 == IF){

		tmpItr = tmpLst.begin();
		while(tmpItr != tmpLst.end()){
			astLst_t1 = pkb->getASTBy(tmpItr->first);
			astItr_t1 = astLst_t1->begin();
			while(astItr_t1 != astLst_t1->end()){
				if(pkb->getType(*astItr_t1) == type1){
					astLst_t2 = pkb->getASTBy(tmpItr->second);
					astItr_t2 = astLst_t2->begin();
					while(astItr_t2 != astLst_t2->end()){
						if(pkb->getType(*astItr_t2) == type2)
						{
							// push this guy to our result
							result.push_back(*tmpItr);
						}
						astItr_t2++;
					}
				}
				astItr_t1++;
			}
			tmpItr++;
		}
	}
}

void Helper::iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmpLst, int v1){
	list<pair<int,int>>::iterator itr;
	if(!tmpLst.empty())
	{
		itr = tmpLst.begin();

		while(itr != tmpLst.end()){
			if(v1 != 0 && itr->second == v1){
				result.push_back(*itr);
			}
			else if(v1 == 0){
				result.push_back(*itr);
			}
			itr++;
		}	
	}
}

/**** PRIVATE ****/

bool Helper::isHigherPrecedence(stack<pair<TYPE, TOKEN>>& operators, TOKEN t){
	pair<TYPE, TOKEN> tPair = operators.top();
	if(atoi(t.c_str()) > atoi(tPair.second.c_str())){
		return true;
	}
	else{
		return false;}
}

vector<pair<TYPE, TOKEN>> Helper::tokenize(EXPRESSION expr){
	vector<pair<TYPE, TOKEN>> res;
	pair<TYPE, TOKEN> tPair;

	// separate the expr
	regex rx("\\(|\\)|\\=|\\;|\\-|\\*|\\+|[^\\s\\n\\-\\*\\+\\;\\=\\(\\)]+");
	sregex_iterator rxItr(expr.begin(), expr.end(), rx), rxend;

	// format the tokens for easy process
	for (rxItr; rxItr != rxend; ++rxItr)
	{
		//cout << "rxItr: " << rxItr->str() << "\n";
		if(rxItr->str() == "+"){
			tPair.first = PLUS;
			tPair.second = "1";
		}
		else if(rxItr->str() == "-"){
			tPair.first = MINUS;
			tPair.second = "1";
		}
		else if(rxItr->str() == "*"){
			tPair.first = MULTIPLY;
			tPair.second = "2";
		}
		else if(rxItr->str() == "("){
			tPair.first = BRACKET;
			tPair.second = "(";
		}
		else if(rxItr->str() == ")"){
			tPair.first = BRACKET;
			tPair.second = ")";
		}
		else {
			tPair.first = STRING;
			tPair.second = rxItr->str();
		}
		//cout << tPair.first << ", " << tPair.second << "\n";
		res.push_back(tPair);
	}
	return res;
}

OPERATOR Helper::operatorToString(TYPE type){
	switch(type){
	case PLUS:
		return "+";
	case MINUS:
		return "-";
	case MULTIPLY:
		return "*";
	}
}

EXPRESSION Helper::formExpression(stack<pair<TYPE, TOKEN>>& operators, stack<OPERAND> &operands){
	OPERATOR op = operatorToString(operators.top().first);
	operators.pop();

	OPERAND right = operands.top();
	operands.pop();
	OPERAND left = operands.top();
	operands.pop();

	OPERAND resExpr = op.append(" " + left + " " + right);
	//cout << "Expression: " << resExpr << "\n";

	return resExpr;
}