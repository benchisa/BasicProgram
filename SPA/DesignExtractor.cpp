#include "DesignExtractor.h"


DesignExtractor::DesignExtractor(PKB* pkb)
{
	this->pkb = pkb;
	DesignExtractor::createCFG();
    DesignExtractor::insertProcModifiesUses();
}


DesignExtractor::~DesignExtractor(void)
{
	delete cfg;
}

// INPUT: Stack of Operators, Current Token
// OUTPUT: true if higher precedence, false otherwise
bool DesignExtractor::isHigherPrecedence(stack<pair<TYPE, TOKEN>>& operators, TOKEN t){
	pair<TYPE, TOKEN> tPair = operators.top();
	if(atoi(t.c_str()) > atoi(tPair.second.c_str())){
		return true;
	}
	else{
	return false;}
}

// INPUT: Expression
// OUTPUT: a list of pair of TYPE and its value
vector<pair<TYPE, TOKEN>> DesignExtractor::tokenize(EXPRESSION expr){
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

// INPUT: a type; PLUS, MINUS, MULIPLY
// OUTPUT: +, -, *
OPERATOR DesignExtractor::operatorToString(TYPE type){
	switch(type){
	case PLUS:
		return "+";
	case MINUS:
		return "-";
	case MULTIPLY:
		return "*";
	}
}

// INPUT: Operators and Operands Stack
// OUTPUT: EXPRESSIOn
EXPRESSION DesignExtractor::formExpression(stack<pair<TYPE, TOKEN>>& operators, stack<OPERAND> &operands){
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

// Convert an expression to prefix expression
// INPUT: ((1 * 2) + (3 * 4)) - ((5 - 6) + (7 - 8))
// OUTPUT: - + * 1 2 * 3 4 + - 5 6 - 7 8
PREFIXEXPR DesignExtractor::convertExprToPrefix(EXPRESSION expr){
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

//calltable

CALL_LIST DesignExtractor::getCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->getCall(caller, callee);
}
bool DesignExtractor::getIsCallResult(PROC_NAME caller, PROC_NAME callee)
{
	return pkb->isExistsCall(caller, callee);
}

CALL_LIST  DesignExtractor::getCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	list<pair<string,string>> finalResult;
	
	if (caller==" " && callee==" ")
	{
		set<string> caller=pkb->getAllCaller();
		 std::set<string>::iterator it;
		 for (it=caller.begin(); it!=caller.end(); it++)
		 {
			 
			 list<string> result;
			 DesignExtractor::computeCallStar(*it, " ", result);
			 if (result.size()>0)
			 {
				 list<string>::iterator itr;
				for (itr=result.begin(); itr!=result.end(); itr++)
				{
					finalResult.push_back(make_pair(*it, *itr));
				}
			 }

		 }
	}
	else

	{	
		
		list<string> firstResult;
		DesignExtractor::computeCallStar(caller, callee, firstResult);
	   if (firstResult.size()>0)
	   {
		
		  list<string>::iterator itr;
		  for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		  {
			
		 	if (caller!=" " && callee==" ")
			{
				
				finalResult.push_back(make_pair(caller, *itr));
			}
			else if (callee!=" " && caller==" ")
			{

				finalResult.push_back(make_pair(*itr, callee));
			}
			
		}
	  }
	}
	
	return finalResult;

}

bool DesignExtractor::getIsCallStarResult(PROC_NAME caller, PROC_NAME callee)
{
	if (caller!=" " && callee!=" ")
	{
		
		CALL_LIST result=DesignExtractor::getCallStarResult(caller," ");
		CALL_LIST::iterator itr;
		if (!result.empty())
		{ 
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (caller==itr->first && callee==itr->second)
				{
					return true;
				}
			}
		}

	}
	return false;
}



void DesignExtractor::createCFG()
{
	
	int size = pkb->getMaxProgLine();

	// create CFG of progline_size
	cfg = new CFG(size);

	// traverse AST and create the CFG
	AST* cAST = pkb->getRootAST();
	AST* tmp = cAST;
	buildCFG(tmp);
}

AST* DesignExtractor::buildCFG(AST * node)
{
	AST* tmp = node;
	AST* containerTmp, *lastNode;

	while(tmp)
	{
		switch(pkb->getType(tmp)){
		
		case ASSIGNMENT:
			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}
			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		
		case WHILE:
			cfgWhileKeepers.push_back(pkb->getProgLine(tmp));

			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}
			
			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());
			if(pkb->getType(lastNode) != IF)
				cfg->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());

			cfgWhileKeepers.pop_back();
			tmp = tmp->getRightSibling();
			break;
		case IF:

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());
			if(cfgWhileKeepers.size()!= 0)
				cfg->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());

			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}

			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling()->getRightSibling());
			if(cfgWhileKeepers.size() != 0)
				cfg->addEdge(pkb->getProgLine(lastNode), cfgWhileKeepers.back());
			
			
			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}

			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()));
			
			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case CALL:
			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}

			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case PROCEDURE:
			lastNode = buildCFG(tmp->getFirstDescendant());
			tmp = tmp->getRightSibling();
			break;
		case STMT_LIST:
			tmp = tmp->getFirstDescendant();
			break;
		}
	}

	return lastNode;
}

bool DesignExtractor::isNextResult(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->isNext(p1, p2);
}

// already stored in CFG
NEXT_LIST DesignExtractor::getNextResult(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->getNext(p1, p2);
}

// on demand
bool DesignExtractor::isNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	int size = pkb->getMaxProgLine();

	if(p1 > size || p2 > size || p1 <= 0 || p2 <=0)
		return false;

	list<PROG_LINE> tmp = cfg->getAllProgLines(p1, p2);
	
	if(tmp.size() != 0){
		list<PROG_LINE>::iterator itr = tmp.begin();
		while(itr!=tmp.end()){
			if(*itr==p2)
				return true;

			itr++;
		}
	}
	return false;
}

// on demand
NEXT_LIST DesignExtractor::getNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	NEXT_LIST result;
	int size = pkb->getMaxProgLine();

	// Next*(n1, n2) --> findAll
	if(p1 == 0 && p2 == 0){
		for(int i = 1; i <=size; i++){
			list<PROG_LINE> tmp = cfg->getAllProgLines(i, i);

			list<PROG_LINE>::iterator itr = tmp.begin();
			while(itr!=tmp.end())
			{
				pair<PROG_LINE, PROG_LINE> tPair;
				tPair.first = i;
				tPair.second = *itr;
				result.push_back(tPair);
				itr++;
			}
		}
	}
	//Next*(n, n), Next*(1,n1), Next*(n1,2) --> findAll
	else if(p1 == p2 || (p1 != 0 && p2 == 0) || (p1 == 0 && p2 != 0) || (p1 != 0 && p2 != 0)){
		list<PROG_LINE> tmp = cfg->getAllProgLines(p1, p2);
		list<PROG_LINE>::iterator itr = tmp.begin();
		while(itr!=tmp.end())
		{
			pair<PROG_LINE, PROG_LINE> tPair;
			if(p1 != 0) {
				tPair.first = p1;
				tPair.second = *itr;
			}
			else if(p1 == 0) {
				tPair.first = *itr;
				tPair.second = p2;
			}
			result.push_back(tPair);
			itr++;
		}
	}
	return result;
}
NEXT_LIST DesignExtractor::getAllPaths(PROG_LINE p1, PROG_LINE p2){
	return cfg->getAllPaths(p1, p2);
}
list<PROG_LINE> DesignExtractor::getAllProgLines(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->getAllProgLines(p1, p2);
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type1, TYPE type2){

	FOLLOWS_LIST result, followLst;
	
	followLst = pkb->getFollows(0, 0);
	DesignExtractor::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type1, TYPE type2){
	FOLLOWS_LIST result, followLst, tmpLst;
	FOLLOWS_LIST::iterator followItr;
	
	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = pkb->getMaxProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = DesignExtractor::getFollowsStar(i, 0);

		if(!tmpLst.empty()){
			followItr = tmpLst.begin();
			while(followItr !=  tmpLst.end()){
				followLst.push_back(*followItr);
				followItr++;
			}
		}
	}
	
	DesignExtractor::computeTypeOnly(result, followLst, type1, type2);
	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getFollows(s1, s2);
		DesignExtractor::computeFollows(result, tmp, type, s1, s2);
	}

	return result;
}

FOLLOWS_LIST DesignExtractor::getFollowsStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	FOLLOWS_LIST result, tmp;
	

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = DesignExtractor::getFollowsStar(s1, s2);
		DesignExtractor::computeFollows(result, tmp, type, s1, s2);
	}
	return result;
}

bool DesignExtractor::getIsFollowsResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if(s1 != 0 && s2 != 0 &&pkb->isFollows(s1, s2) ){
		return true;
	}

	return false;
}

bool DesignExtractor::getIsFollowsStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2){
	if (s1!=NULL && s2!=NULL)
	{
		FOLLOWS_LIST result=DesignExtractor::getFollowsStar(s1,NULL);
		FOLLOWS_LIST::iterator itr;
		if (!result.empty())
		{
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (s1==itr->first && s2==itr->second)
				{
					return true;
				}
			}
		}

	}
	return false;
}



PARENT_LIST DesignExtractor::getParentResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst;
	
	parentLst = pkb->getParent(0, 0);
	DesignExtractor::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type1, TYPE type2)
{	
	PARENT_LIST result, parentLst, tmpLst;
	PARENT_LIST::iterator parentItr;
	AST_LIST::iterator astItr_t1, astItr_t2;

	PROC_LIST *procLst = pkb->getAllProc();
	PROG_LINE startProgLine = procLst->begin()->getStartProgLine();
	PROG_LINE endProgLine = pkb->getMaxProgLine();

	// iterate from 1....n
	for(int i = startProgLine; i <= endProgLine; i++){
		tmpLst = DesignExtractor::getParentStar(i, 0);

		if(!tmpLst.empty()){
			parentItr = tmpLst.begin();
			while(parentItr !=  tmpLst.end()){
				parentLst.push_back(*parentItr);
				parentItr++;
			}
		}
	}
	DesignExtractor::computeTypeOnly(result, parentLst, type1, type2);

	return result;
}

PARENT_LIST DesignExtractor::getParentResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;
	
	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = pkb->getParent(s1, s2);
		DesignExtractor::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

PARENT_LIST DesignExtractor::getParentStarResult(TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
	PARENT_LIST result, tmp;

	if((s1 == 0 && s2 != 0) || (s1 != 0 && s2 == 0)){
		tmp = DesignExtractor::getParentStar(s1, s2);
		DesignExtractor::computeParent(result, tmp, type, s1, s2);
	}
	return result;
}

bool DesignExtractor::getIsParentResult(STATEMENT_NUM s1, STATEMENT_NUM s2){	
	if(s1 != 0 && s2 != 0 && pkb->isParent(s1, s2))
		return true;
	else
		return false;
}

bool DesignExtractor::getIsParentStarResult(STATEMENT_NUM s1, STATEMENT_NUM s2)
{
	if (s1!=NULL && s2!=NULL)
	{
		PARENT_LIST result=DesignExtractor::getParentStar(s1,NULL);
		PARENT_LIST::iterator itr;
		if (!result.empty())
		{
			for (itr=result.begin(); itr!=result.end(); itr++)
			{
				if (s1==itr->first && s2==itr->second)
				{
					return true;
				}
			}
		}
	}
	return false;
}

MODIFIES_LIST DesignExtractor::getModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST result,tmpLst;
	MODIFIES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	//cout << "TYPE: " << type << "\n";

	// Modifies(w, 1)
	// Modifies(w, 2)
	// Modifies(c, 2)
	if((type == WHILE  || type == ASSIGNMENT||type == IF|| type == PROCEDURE)){
		//cout << "v1: " << pkb->getVarName(v1) << "\n";
			result = pkb->getModifies(type, arg1, v1);

	}
	// modifies(c, 2)
	// modifies(c, _)
	else if(type == CALL){
		result = computeCallModifies(arg1, v1);
	}
	
	// Modifies(1, _), 
	else if((type == STATEMENT && arg1 != 0 && v1 == 0
		|| type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();

		if(pkb->isExistCallStmt(arg1)){
			result = computeCallModifies(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT||pkb->getType(*astItr)==IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->getModifies(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == ANY || (type == STATEMENT && arg1 == 0)){
		// Modifies(_,_) or Modifies(s, "x")
		tmpLst = pkb->getModifies(WHILE, 0, 0);
		iterateAndStore(result, tmpLst, v1);

		tmpLst = this->computeCallModifies(0,0);
		iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getModifies(ASSIGNMENT, 0, 0);
		iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getModifies(IF, 0, 0);
		iterateAndStore(result, tmpLst, v1);
	}


	//cout << "SIZE OF RESULT (MODIFIES): " << result.size() << "\n";
	
	return result;
}

bool DesignExtractor::getIsModifiesResult(TYPE type, int arg1, VAR_INDEX v1){
	MODIFIES_LIST modifiesLst;
	bool result = false;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;

	//WHY dont consider assignment?
	if(type == STATEMENT && arg1 != 0 && v1 != 0){
		// Find the type of first argument
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
		
		// check for stmt existence in call table
		/*if(pkb->cal){
		}*/
		if(pkb->isExistCallStmt(arg1)){
			result = isCallModifies(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE||pkb->getType(*astItr) == IF||pkb->getType(*astItr) == ASSIGNMENT)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->isModifies(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == PROCEDURE){ //need to be change
		result = pkb->isModifies(PROCEDURE, arg1, v1);
	}


	return result;
}

USES_LIST DesignExtractor::getUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST result,tmpLst;
	USES_LIST::iterator itr;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	
	/*cout << "asdasd\n";
	cout << "TYPE: " << type << "\n";
	cout << "ARG1: " << arg1 << "\n";
	cout << "v1: " << v1 << "\n";
	*/

	//Uses(w, "b")
	if((type == WHILE  || type == ASSIGNMENT || type == IF || type == PROCEDURE)){
		result = pkb->getUses(type, arg1, v1);
	}
	// uses(c, 2)
	// uses(c, _)
	else if(type == CALL){
		result = computeCallUses(arg1, v1);
	}
	// Uses(1, a)
	else if((type == STATEMENT && arg1 != 0 && v1 == 0) || (type == ANY && arg1 != 0 && v1 == 0)){
		// Find the type first
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
		if(pkb->isExistCallStmt(arg1)){
			result = computeCallUses(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT ||pkb->getType(*astItr) == IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->getUses(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == ANY || (type == STATEMENT && arg1 == 0))
	{
		tmpLst = pkb->getUses(WHILE, 0, 0);
		iterateAndStore(result, tmpLst, v1);
		
		tmpLst = pkb->getUses(IF, 0, 0);
		iterateAndStore(result, tmpLst, v1);
		
		tmpLst = this->computeCallUses(0,0);
		iterateAndStore(result, tmpLst, v1);

		tmpLst = pkb->getUses(ASSIGNMENT, 0, 0);
		iterateAndStore(result, tmpLst, v1);
	}

	//cout << "SIZE OF RESULT (USES): " << result.size() << "\n";

	return result;
}

bool DesignExtractor::getIsUsesResult(TYPE type, int arg1, VAR_INDEX v1){
	USES_LIST usesLst;
	bool result = false;
	AST_LIST *astLst;
	AST_LIST::iterator astItr;
	TYPE getStatementType;
	/*cout << "asdasd\n";
	cout << "TYPE: " << type << "\n";
	cout << "ARG1: " << arg1 << "\n";
	cout << "v1: " << v1 << "\n";*/

	if(type == STATEMENT && arg1 != 0 && v1 != 0){
		// Find the type of first argument
		astLst = pkb->getASTBy(arg1);
		astItr = astLst->begin();
		
		if(pkb->isExistCallStmt(arg1)){
			result = isCallModifies(arg1, v1);
		}
		else{
			while(astItr != astLst->end()){
				if(pkb->getType(*astItr) == WHILE || pkb->getType(*astItr) == ASSIGNMENT || pkb->getType(*astItr) == IF)
				{
					getStatementType = pkb->getType(*astItr);
					result = pkb->isUses(getStatementType, arg1, v1);
					break;
				}
				astItr++;
			}
		}
	}
	else if(type == PROCEDURE) //need to be changed and need to add call
	{
		result = pkb->isUses(PROCEDURE, arg1, v1);
	}
	return result;
}

MODIFIES_LIST DesignExtractor::computeCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	MODIFIES_LIST resultList;
			
	if(callStmt==0&&varIndex==0){
		
		vector<int> callStmt=pkb->getAllCallerStmt();
		vector<int>::iterator s_itr;
		for (s_itr=callStmt.begin();s_itr!=callStmt.end(); s_itr++)
		{
			string calleeName=pkb->getCalleeName(*s_itr);
			if (calleeName!=" ")
			{
				MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, pkb->getProcIndex(calleeName), 0);
				MODIFIES_LIST::iterator m_itr;
				for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
				{
					resultList.push_back(make_pair(*s_itr,m_itr->second));
				}
			}
		}
	}else if(callStmt!=0&&varIndex==0){

	
		string calleeName=pkb->getCalleeName(callStmt);
		if (calleeName!=" ")
		{
			MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, pkb->getProcIndex(calleeName), 0);
			MODIFIES_LIST::iterator m_itr;
			for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
			{
				resultList.push_back(make_pair(callStmt,m_itr->second));
			}
		}
		


	
	}else if(callStmt==0&&varIndex!=0){
	
		
		MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, 0,varIndex);
		MODIFIES_LIST::iterator m_itr;
		MODIFIES_LIST::iterator m_itr2;
		for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
		{
			for (m_itr2=m_list.begin(); m_itr2!=m_list.end(); m_itr2++)
			{
				string callerName=pkb->getProcedure(m_itr->first)->getProcName();
				string calleeName=pkb->getProcedure(m_itr2->first)->getProcName();
				
				int callerStmt=pkb->getCallStmt(callerName, calleeName);
				if (callerStmt!=0)
				{	
					
					resultList.push_back(make_pair(callerStmt, varIndex));
				}
			}
		}
	}
	return resultList;
}


bool DesignExtractor::isCallModifies(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if (callStmt!=0 && varIndex!=-1)
	{
		MODIFIES_LIST m_list=DesignExtractor::computeCallModifies(callStmt, 0);
		MODIFIES_LIST::iterator m_itr;
		for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
		{
			if (m_itr->first==callStmt && m_itr->second==varIndex)
			{
				return true;
			}
		}
	}

	return false;
}
USES_LIST DesignExtractor::computeCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	USES_LIST resultList;

	if(callStmt==0&&varIndex==0){
		
		vector<int> callStmt=pkb->getAllCallerStmt();
		vector<int>::iterator s_itr;
		for (s_itr=callStmt.begin();s_itr!=callStmt.end(); s_itr++)
		{
			string calleeName=pkb->getCalleeName(*s_itr);
			if (calleeName!=" ")
			{
				USES_LIST u_list=pkb->getUses(PROCEDURE, pkb->getProcIndex(calleeName), 0);
				USES_LIST::iterator u_itr;
				for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
				{
					resultList.push_back(make_pair(*s_itr,u_itr->second));
				}
			}
		}
	}else if(callStmt!=0&&varIndex==0){

	
		string calleeName=pkb->getCalleeName(callStmt);
		if (calleeName!=" ")
		{
			USES_LIST u_list=pkb->getUses(PROCEDURE, pkb->getProcIndex(calleeName), 0);
			USES_LIST::iterator u_itr;
			for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
			{
				resultList.push_back(make_pair(callStmt,u_itr->second));
			}
		}
		


	
	}else if(callStmt==0&&varIndex!=0){
	
		
		USES_LIST u_list=pkb->getUses(PROCEDURE, 0,varIndex);
		USES_LIST::iterator u_itr;
		USES_LIST::iterator u_itr2;
		for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
		{
			for (u_itr2=u_list.begin(); u_itr2!=u_list.end(); u_itr2++)
			{
				string callerName=pkb->getProcedure(u_itr->first)->getProcName();
				string calleeName=pkb->getProcedure(u_itr2->first)->getProcName();
				
				int callerStmt=pkb->getCallStmt(callerName, calleeName);
				if (callerStmt!=0)
				{	
					
					resultList.push_back(make_pair(callerStmt, varIndex));
				}
			}
		}
	}
	return resultList;
}


bool DesignExtractor::isCallUses(STATEMENT_NUM callStmt,VAR_INDEX varIndex){
	if (callStmt!=0 && varIndex!=0)
	{
		
		USES_LIST u_list=DesignExtractor::computeCallUses(callStmt, 0);
		USES_LIST::iterator u_itr;
		for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
		{
			
			if (u_itr->first==callStmt && u_itr->second==varIndex)
			{
				return true;
			}
		}
	}

	return false;
}
//above done with testing//

void DesignExtractor::computeParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2, list<int> &result)
{
	if (stmt1!=NULL || stmt2!=NULL)
	{
		list<pair<int,int>> parentChild=pkb->getParent(stmt1, stmt2);
		if (parentChild.size()>0)
		{
			list<pair<int,int>>::iterator itr;
			for (itr=parentChild.begin(); itr!=parentChild.end(); itr++)
			{	

				if (stmt1!=0 && stmt2==0)

				{	
					result.push_back(itr->second);
					DesignExtractor::computeParentStar(itr->second, 0,result);
				}
				else if (stmt1==0 && stmt2!=0)
				{	
					result.push_back(itr->first);
					DesignExtractor::computeParentStar(0,itr->first,result);
				}
			}
		}

	}
}

FOLLOWS_LIST DesignExtractor::getParentStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	DesignExtractor::computeParentStar(stmt1, stmt2, firstResult);

	if (firstResult.size()>0)
	{   

		list<int>::iterator itr;
		for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		{
			if (stmt1!=NULL && stmt2==NULL)
			{

				finalResult.push_back(make_pair(stmt1, *itr));
			}
			else if (stmt2!=NULL && stmt1==NULL)
			{

				finalResult.push_back(make_pair(*itr, stmt2));
			}
		}

	}
	return finalResult;

}

void  DesignExtractor::computeFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2,list<int> &result)
{
	if (stmt1!=NULL || stmt2!=NULL)
	{
		list<pair<int,int>> follows=pkb->getFollows(stmt1, stmt2);
		if (follows.size()>0)
		{
			list<pair<int, int>>::iterator itr;

			for (itr=follows.begin(); itr!=follows.end(); itr++)
			{

				if (stmt1!=NULL && stmt2==NULL)
				{	
					result.push_back(itr->second);
					DesignExtractor::computeFollowsStar(itr->second, 0,result);
				}
				else if (stmt1==NULL && stmt2!=NULL)
				{
					result.push_back(itr->first);
					DesignExtractor::computeFollowsStar(0,itr->first,result);
				}
			}


		}
	}
}

FOLLOWS_LIST DesignExtractor::getFollowsStar(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	list<pair<int,int>> finalResult;
	list<int> firstResult;
	DesignExtractor::computeFollowsStar(stmt1, stmt2, firstResult);
	if (firstResult.size()>0)
	{
		list<int>::iterator itr;
		for (itr=firstResult.begin(); itr!=firstResult.end(); itr++)
		{
			if (stmt1!=NULL && stmt2==NULL)
			{
				finalResult.push_back(make_pair(stmt1, *itr));
			}
			else if (stmt2!=NULL && stmt1==NULL)
			{
				finalResult.push_back(make_pair(*itr, stmt2));
			}
		}
	}
	return finalResult;
}

// Computational Methods
void DesignExtractor::computeTypeOnly(list<pair<int, int>> &result, list<pair<int, int>> &tmpLst, TYPE type1, TYPE type2){
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
	
void DesignExtractor::computeParent(PARENT_LIST &result,PARENT_LIST tmpLst, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2)
	{
		PARENT_LIST::iterator tmpItr;
		AST_LIST *astLst;
		AST_LIST::iterator astItr;

		if(!tmpLst.empty()){
			// Check TYPE for WHILE or ASSIGNMENT
			if(type == WHILE || type == ASSIGNMENT || type == IF){
				tmpItr = tmpLst.begin(); // should have only 1 pair
				// Deciding which is empty

				if(s1 == 0) astLst = pkb->getASTBy(tmpItr->first);
				else astLst = pkb->getASTBy(tmpItr->second);
				
				//cout << tmp << "\n";
				if(!astLst->empty())
				{
					astItr = astLst->begin();

					while(astItr!=astLst->end()){
						if(pkb->getType(*astItr) == type){
							result = tmpLst;
							//result = tmp;
							break; // does not need to go through all the nodes.
						}

						astItr++;
					}
				}
			}
			//Parent(_, 2) or Parent(s1, 2) is the same.
			else if(type == STATEMENT || type == ANY)
			{
				// just want the pair of statement.
							
				result = tmpLst;
			}
		}
	}

void DesignExtractor::computeFollows(FOLLOWS_LIST &result, FOLLOWS_LIST tmp, TYPE type, STATEMENT_NUM s1, STATEMENT_NUM s2){
		FOLLOWS_LIST::iterator followItr;
		AST_LIST *astLst;
		AST_LIST::iterator astItr;
		if(!tmp.empty()){

			// Check TYPE for WHILE or ASSIGNMENT
			if(type == WHILE || type == ASSIGNMENT || type == IF){
				followItr = tmp.begin(); 
				while (followItr!=tmp.end())
				{
					if(s1 == 0) astLst = pkb->getASTBy(followItr->first);
					else astLst = pkb->getASTBy(followItr->second);
				
					astItr = astLst->begin();
					while(astItr!=astLst->end())
					{	
						if(pkb->getType(*astItr) == type){		
							result.push_back(*followItr);
							 
						}
						astItr++;
					}
					followItr++;
				}

			}
			else if(type == STATEMENT || type == ANY)
			{
				result = tmp;
			}
		}
	}

	//This function does not work properly
void DesignExtractor::iterateAndStore(list<pair<int, int>> &result, list<pair<int, int>> tmpLst, int v1){
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
/*
void DesignExtractor::iterateAndStore(RELATION_LIST *result, RELATION_LIST list){
	if(list.empty()){
			result=NULL;
		}else{
			    RELATION_LIST::iterator itr = list.begin();
				while(itr!=list.end()){
					result->push_back(*itr);
					itr++;
				}	
		}
	
}
	*/
void  DesignExtractor::computeCallStar(PROC_NAME caller, PROC_NAME callee,list<string> &result)
{
	if (caller!=" " || callee!=" ")
	{
		CALL_LIST calls=pkb->getCall(caller, callee);
		if (calls.size()>0)
		{
			CALL_LIST::iterator itr;

			for (itr=calls.begin(); itr!=calls.end(); itr++)
			{

				if (caller!=" " && callee==" ")
				{	
					result.push_back(itr->second);
					DesignExtractor::computeCallStar(itr->second, " ",result);
				}
				else if (caller==" " && callee!=" ")
				{
					result.push_back(itr->first);
					DesignExtractor::computeCallStar(" ",itr->first,result);
				}
			}


		}
	}
	
}

DATA_LIST * DesignExtractor::getAllConstants(){
	DATA_LIST * returnList = new DATA_LIST();

	int size = pkb->getConstantTableSize();
	for(int i =1;i<=size;i++){
		returnList->push_back(pkb->getConstantValue(i));
	}
	return returnList;
}

DATA_LIST * DesignExtractor::getAllCallStmts(){
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int stmtNo = pkb->getMaxStatementNum();
	for(int i=1; i<=stmtNo;i++){
		AST_LIST * stmtTree;
		stmtTree = pkb->getASTBy(i);
		AST_LIST::iterator itr;

		for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
			if((*itr)->getRootType()==CALL){
				returnList->push_back(i);
			} 
		}
	}

	return returnList;
}
bool DesignExtractor::isStatementTypeOf(TYPE typeName,STATEMENT_NUM stmtNum){
	//find the type of statement
	if(stmtNum>0&&stmtNum<=pkb->getMaxStatementNum()){

		AST_LIST* currentAST = pkb->getASTBy(stmtNum);
		AST_LIST::iterator itr;
		for(itr = currentAST->begin();itr!=currentAST->end();itr++){
			if((*itr)->getRootType()==typeName) return true;
		}
	}
	return false;
}
DATA_LIST * DesignExtractor::getStmtListOf(TYPE nodeType){
	DATA_LIST * returnList = new DATA_LIST();

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
		returnList = pkb->getAllAssigns();
		break;
	case WHILE:
		returnList = pkb->getAllWhiles();
		break;
	case IF:
		returnList = pkb->getAllIfs();
		break;
	case CALL:
		returnList = DesignExtractor::getAllCallStmts();
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
	}
	return returnList;
}

void DesignExtractor::insertProcModifiesUses()
{
	list<CALL_PAIR> procs=DesignExtractor::getCallStarResult(" "," ");
	list<CALL_PAIR>::iterator c_itr;

	for (c_itr=procs.begin(); c_itr!=procs.end(); c_itr++)
	{  
		int callerIndex=pkb->getProcIndex(c_itr->first);
		int calleeIndex=pkb->getProcIndex(c_itr->second);
		 
		MODIFIES_LIST m_list=pkb->getModifies(PROCEDURE, calleeIndex, 0);
		USES_LIST u_list=pkb->getUses(PROCEDURE, calleeIndex, 0);

		MODIFIES_LIST::iterator m_itr;
		USES_LIST::iterator u_itr;

		for (m_itr=m_list.begin(); m_itr!=m_list.end(); m_itr++)
		{
			pkb->insertModifies(PROCEDURE, callerIndex, m_itr->second);
		}

		for (u_itr=u_list.begin(); u_itr!=u_list.end(); u_itr++)
		{
			pkb->insertUses(PROCEDURE, callerIndex, u_itr->second);
		}	

	}
}
