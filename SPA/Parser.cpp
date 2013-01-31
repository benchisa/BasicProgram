#pragma once
#include "Parser.h"
#include "PKB.h"

using namespace std;

Parser::Parser(void)
{

}

Parser::~Parser(void)
{
}

//--Public--//

void Parser::setSource(SOURCE src){
	this->errorMsg = "";
	this->src = src;
}

SOURCE Parser::getSource(){
	return src;
}

ERROR_MSG Parser::getLastError(){
	return this->errorMsg;
}

PKB* Parser::getPKB(){
	return this->pkb;
}

int Parser::startParse(){
	// initialize everything
	tokens = ParserTokenizer::tokenize(this->src);
	//ParserTokenizer t;
	//t.printAll(tokens);
	curToken = tokens.at(0).first;
	prevToken = "";
	curProgLine = tokens.at(0).second;
	tokenIndex = 0;
	stmt_num = 0;
	nesting = 0;
	lastProcEndLine = 0;

	pkb = new PKB();

	if(Parser::parse()){
		//debugging
		return 1;
	}else{
		return -1;
	}
}

//--Private--//
bool Parser::parse(){
	return Parser::program();
}

bool Parser::matchToken(TOKEN token){
	if(curToken.compare(token) == 0){
		prevToken = curToken;
		prevProgLine = curProgLine;
		if(tokenIndex < tokens.size()-1) {
			curToken = tokens.at(++tokenIndex).first;
			curProgLine = tokens.at(tokenIndex).second;
		}
		else
			curToken = " ";
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::matchToken(regex reg){
	if(regex_match(curToken, reg)){
		prevToken = curToken;
		prevProgLine = curProgLine;
		if(tokenIndex < tokens.size()-1) {
			curToken = tokens.at(++tokenIndex).first;
			curProgLine = tokens.at(tokenIndex).second;
		}
		else
			curToken = " ";
		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::program(){
	if(!procedure()){
		return false;	
	}
	else{
		pkb->getProcedure(curProcIndex)->setEndProgLine(curProgLine);
		if(matchToken(" ")){
			return true;
		}
		else
		{
			// add to procedureContainer
			lastProcEndLine = stmt_num;
			// run again
			return program();
		}
	}

	
	return true;
}

bool Parser::procedure(){
	if(matchToken("procedure")){
		AST* prevProc;
		if(name()) 
		{
			Procedure *cProc = pkb->createProc(prevToken, stmt_num+1);
			curProc = prevToken;
			if(pkb->isProcExists(prevToken))			
			{				
				error(INVALID_PROCEDURE_NAME);				
				return false;			
			}
			curProcIndex = pkb->insertProc(cProc);
			if(stmt_num == 0){
				curAST = pkb->createAST(PROCEDURE , prevProgLine, 0, curProcIndex);
				if(pkb->setRootAST(curAST));
				else
				{
					return false;
				}
			}
			// next procedure
			else
			{
				AST *newProc = pkb->createAST(PROCEDURE, prevProgLine, stmt_num+1, curProcIndex);
				pkb->addSibling(curAST, newProc);
				curAST = newProc;
			}
		}
		else
		{
			error(INVALID_PROCEDURE_NAME);
			return false;
		}
		if(matchToken("{")){
			prevProc = curAST;
			curAST = pkb->createAST(STMT_LIST, prevProgLine, 0, -1);
			if(!pkb->setFirstDescendant(prevProc, curAST))
				pkb->setAncestor(curAST, prevProc);

			if(stmtlst()) 
			{
				curAST = prevProc; // set back pointer to procedure
				return true;
			}else{
				return false;
			}
		}else
		{
			error(MISSING_BRACKET);
			return false;
		}
	}else
	{
		error(INVALID_PROCEDURE);
		return false;
	}

	return true;
}

bool Parser::stmtlst(){
	// stmtlst: stmt+
	if(!stmt()){
		return false;
	}

	if(matchToken("}")) {
		if(!containerIndex.empty())
		{
			if(curToken.compare("}") != 0 && curToken.compare("else") != 0) {
				//cout << "insertFollows1: " << containerIndex.back().first << "," << progLine+1 << "\n";
				insertFollows(containerIndex.back().first, stmt_num+1);
			}
			
			// dont pop if it's else
			if(curToken.compare("else") != 0) 
			{
				//cout << "curtoken: " << curToken << "\n";
				containerIndex.pop_back(); // remove the last container.
			}
		}
		return true;
	}
	else if(!stmtlst()) {
		return false;
	}

	return true;
}

bool Parser::stmt(){
	// stmt: assign | while | if | call
	if(stmt_while()){
		return true;
	}
	else if(stmt_if()){

		return true;
	}
	else if(stmt_call())
	{
		return true;
	}
	else
	{
		int assign = stmt_assign();
		if(assign == 1)
			return true;
		else
			return false;
	}
}

bool Parser::stmt_call(){
	// call: call proc_name
	if(matchToken("call")){
		if(name()){
			// add to call table
			pkb->insertCall(curProc, prevToken);

			// create AST
			AST *callNode = pkb->createAST(CALL, prevProgLine, stmt_num, pkb->getProcIndex(prevToken));
			if(!pkb->setFirstDescendant(curAST, callNode))
			{
				pkb->addSibling(curAST, callNode);
				pkb->setAncestor(callNode, curAST->getAncestor());
			}

			curAST = callNode;
			
			// create follows, parent
			insertFollowsParentForStmt(stmt_num-1, stmt_num);

			if(!matchToken(";")) {
				error(MISSING_COLON);
				return false;
			}
		}
		else
		{
			error(INVALID_CALL_NAME);
			return false;
		}
	}
	else
	{
		error(INVALID_CALL_NAME);
		return false;
	}

	return true;
}

bool Parser::stmt_if(){
	// if: if var_name then '{' stmtlst '}' else '{' stmtlst '}'
	TOKEN tmpToken = prevToken;

	if(matchToken("if")){
		stmt_num++;
		nesting = stmt_num;

		AST *ifNode = pkb->createAST(IF, prevProgLine, stmt_num, -1);
		AST *leftNode, *thenNode, *elseNode;

		if(!pkb->setFirstDescendant(curAST, ifNode))
		{
			pkb->addSibling(curAST, ifNode);
			pkb->setAncestor(ifNode, curAST->getAncestor());
		}

		curAST = ifNode;

		if(name()){
			// add var_name to Use
			curVarIndex = pkb->insertVar(prevToken);
			pkb->insertUses(PROCEDURE, curProcIndex, curVarIndex);
			pkb->insertUses(IF, stmt_num, curVarIndex);

			leftNode = pkb->createAST(VARIABLE, prevProgLine, stmt_num, curVarIndex);
		
			if(matchToken("then") && matchToken("{"))
			{
				thenNode = pkb->createAST(STMT_LIST, prevProgLine, stmt_num, THEN); // then node
				
				pkb->setFirstDescendant(ifNode, leftNode);
				pkb->setAncestor(thenNode, ifNode);
				pkb->addSibling(leftNode, thenNode);
				curAST = thenNode;

				insertFollowsParentForCon(tmpToken, stmt_num-1, stmt_num);

				containerInfo.first = stmt_num;
				containerInfo.second = IF;
				containerIndex.push_back(containerInfo);

				if(stmtlst())
				{
					if(matchToken("else") && matchToken("{"))
					{
						//progLine++;
						//??? do we need a progline, no right.
						elseNode = pkb->createAST(STMT_LIST, 0, stmt_num, ELSE); // then node
						pkb->setAncestor(elseNode, ifNode);
						pkb->addSibling(thenNode, elseNode);
						
						curAST = elseNode;

						if(stmtlst())
						{
							curAST = ifNode; // move to pointer back to ifNode
						}else
						{
							error(INVALID_STMTLST);
							return false;
						}
					}
					else
					{
						error(INVALID_IF_FORMAT);
						return false;
					}
				}
				else
				{
					error(INVALID_STMTLST);
					return false;
				}
			}
			else
			{
				error(INVALID_IF_FORMAT);
				return false;
			}
		}
		else{
			error(INVALID_IF_NAME);
			return false;
		}

		return true;
	}
	return false;
}

bool Parser::stmt_while(){
	// while: while var_name '{' stmtlst '}'
	TOKEN tmpToken = prevToken; // use for follows

	if(matchToken("while"))
	{	
		stmt_num++;
		nesting = stmt_num;

		AST *whileNode = pkb->createAST(WHILE, prevProgLine, stmt_num, -1);
		AST *leftNode, *rightNode;

		if(!pkb->setFirstDescendant(curAST, whileNode))
		{
			pkb->addSibling(curAST, whileNode);
			pkb->setAncestor(whileNode, curAST->getAncestor());
		}

		curAST = whileNode;

		if(name()){
			curVarIndex = pkb->insertVar(prevToken);
			pkb->insertUses(PROCEDURE, curProcIndex, curVarIndex);
			pkb->insertUses(WHILE, stmt_num, curVarIndex);

			leftNode = pkb->createAST(VARIABLE, prevProgLine,stmt_num, curVarIndex);

			if(!pkb->setFirstDescendant(whileNode, leftNode))
				pkb->setAncestor(leftNode, whileNode);
			
			insertFollowsParentForCon(tmpToken, stmt_num-1, stmt_num);

			containerInfo.first = stmt_num;
			containerInfo.second = WHILE;
			containerIndex.push_back(containerInfo);

			if(matchToken("{")){
				rightNode = pkb->createAST(STMT_LIST,prevProgLine, stmt_num, -1);
				pkb->setAncestor(rightNode, whileNode);
				pkb->addSibling(leftNode, rightNode);

				prevAST = curAST;
				curAST = rightNode;

				if(!stmtlst()){
					return false;
				}
				else{
					curAST = whileNode; //move pointer back to while loop
				}
			}else
			{
				error(MISSING_BRACKET);
				return false;
			}
		}
		else
		{
			error(INVALID_WHILE_NAME);
			return false;
		}


	}else
	{
		// not necessary an error;
		return false;
	}
	return true;
}

bool Parser::stmt_assign(){
	//assign: var_name '=' expr
	stmt_num++;

	AST *assignNode = pkb->createAST(ASSIGNMENT, curProgLine, stmt_num, -1);
	AST *leftNode, *rightNode;

	if(!pkb->setFirstDescendant(curAST, assignNode))
	{
		pkb->addSibling(curAST, assignNode);
		pkb->setAncestor(assignNode, curAST->getAncestor());
	}

	curAST = assignNode;

	insertFollowsParentForStmt(stmt_num-1, stmt_num);

	if(name()){
		curVarIndex = pkb->insertVar(prevToken);
		pkb->insertModifies(PROCEDURE, curProcIndex, curVarIndex);
		pkb->insertModifies(ASSIGNMENT, stmt_num, curVarIndex);

		if(!containerIndex.empty())
		{
			vector<pair<PROG_LINE, TYPE>>::iterator itr = containerIndex.begin();
			while(itr != containerIndex.end()){
				//cout << "insertmodifies1: type->" << itr->second << ",Modifies_Index: " << itr->first << ", varindex: " << curVarIndex << "\n";
				pkb->insertModifies(itr->second, itr->first, curVarIndex);
				itr++;
			}
		}

		leftNode = pkb->createAST(VARIABLE, prevProgLine, stmt_num, curVarIndex);
		pkb->setFirstDescendant(assignNode, leftNode);

		if(matchToken("=")){
			if(expr()){
				if(matchToken(";")) {
					rightNode = operands.top();
					if(!operands.empty()) operands.pop();

					pkb->setAncestor(rightNode, assignNode);
					pkb->addSibling(leftNode, rightNode);
					curAST = assignNode;
					return true;	
				}
				else{
					error(MISSING_COLON);
					return false;
				}
			}
			else
				return false;
		}else
		{
			//print error
			error(MISSING_OPERATOR);
			return false;
		}
	}
	else{
		return false;
	}
	return true;
}

bool Parser::expr(){
	// expr: expr '+' term | expr '-' term | term
	if(term())
	{
		if(matchToken("+"))
		{
			if(operators.empty()) 
				operators.push(pkb->createAST(PLUS, prevProgLine, stmt_num, -1));
			else
			{
				if(pkb->getType(operators.top()) == MULTIPLY)
				{
					createExprTree();
				}
				else
				{
					operators.push(pkb->createAST(PLUS, prevProgLine, stmt_num, -1));
				}
				
			}

			if(expr())
			{
				createExprTree();
			}
			else
			{
				return false;
			}
		}
		else if(matchToken("-"))
		{
			if(operators.empty()) 
				operators.push(pkb->createAST(MINUS, prevProgLine, stmt_num, -1));
			else
			{
				if(pkb->getType(operators.top()) == MULTIPLY)
				{
					createExprTree();
				}
				else
				{
					operators.push(pkb->createAST(MINUS,prevProgLine, stmt_num, -1));
				}
			}

			if(term())
			{
				createExprTree();
			}
			else
			{
				return false;
			}
		}
		else{
			
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::term(){
	// term: term "*" factor | factor
	if(factor())
	{
		if(matchToken("*"))
		{
			if(operators.empty())
				operators.push(pkb->createAST(MULTIPLY, prevProgLine,stmt_num, -1));
			else
			{
				operators.push(pkb->createAST(MULTIPLY, prevProgLine,stmt_num, -1));

			}

			if(factor())
			{
				createExprTree();
				return true;
			}
			else
			{
				return false;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Parser::factor(){
	// factor: var_name | const_value | "(" expr ")"
	if(name())
	{
		curVarIndex = pkb->insertVar(prevToken);
		pkb->insertUses(PROCEDURE, curProcIndex, curVarIndex);
		pkb->insertUses(ASSIGNMENT, stmt_num, curVarIndex);

		if(!containerIndex.empty())
		{
			vector<pair<PROG_LINE, TYPE>>::iterator itr = containerIndex.begin();
			while(itr != containerIndex.end()){
				pkb->insertUses(itr->second, itr->first, curVarIndex);
				itr++;
			}
		}

		operands.push(pkb->createAST(VARIABLE, prevProgLine,stmt_num, curVarIndex));
		return true;
	}
	else if(const_value())
	{
		pkb->insertConst(atoi(prevToken.c_str()));
		operands.push(pkb->createAST(CONSTANT, prevProgLine,stmt_num, atoi(prevToken.c_str())));
		return true;
	}
	else if(openBracket())
	{
		operators.push(pkb->createAST(BRACKET,prevProgLine, stmt_num, -1));
		if(expr())
		{
			if(closeBracket())
			{
				// remove open bracket from operands stack
				if(!operators.empty()) operators.pop();
			}
		}
	}
	
}

bool Parser::openBracket(){
	// const_value: INTEGER
	if(matchToken("(")){
		return true;
	}

	return false;
}

bool Parser::closeBracket(){
	// const_value: INTEGER
	if(matchToken(")")){
		return true;
	}

	return false;
}


bool Parser::const_value(){
	// const_value: INTEGER
	if(matchToken(regex("[0-9]*"))){
		return true;
	}

	return false;
}

bool Parser::name(){
	// NAME: LETTER(LETTER | DIGIT)*
	if(matchToken(regex("[a-zA-Z][a-zA-Z0-9]*"))){
		return true;
	}

	return false;
}
void Parser::insertFollowsParentForStmt(PROG_LINE p1, PROG_LINE p2){
	if(!containerIndex.empty() && p2 != 1){
		// not first line, and previous line not a container
		pkb->insertParent(containerIndex.back().first, p2);
	
		// while {} a=b; a=b;
		if(p1 != containerIndex.back().first && prevToken.compare("{") != 0 && prevToken.compare("}") != 0)
		{	
			//cout << "assign's insertfollow: " << curToken << "\n";
			//cout << "assign's insertFollows: " << progLine-1 << "," << progLine << "\n";
			insertFollows(p1, p2);
		}
	}
	else
	{
		if(prevToken.compare("{") != 0 && prevToken.compare("}") != 0) {
			//cout << "assign's insertFollows2: " << progLine-1 << "," << progLine << "\n";
			insertFollows(p1, p2);
		}
	}
}

void Parser::insertFollowsParentForCon(TOKEN tmpToken, PROG_LINE p1, PROG_LINE p2){
	if(!containerIndex.empty())
	{
		if(p1 != containerIndex.back().first){
			//cout << "Insertfollows241: " << progLine-1 << ", " << progLine << "\n";
			insertFollows(p1, p2);
		}

		pkb->insertParent(containerIndex.back().first, p2);
		//cout << "InsertParent1: Parent(" << containerIndex.back().first << "," << progLine << ")\n";
					
		vector<pair<PROG_LINE, TYPE>>::iterator itr = containerIndex.begin();
		while(itr != containerIndex.end()){
			pkb->insertUses(itr->second, itr->first, curVarIndex);
			itr++;
		}
	}
	else
	{
		if(tmpToken != "}")
		{
			insertFollows(p1, p2);
		}
	}
}

void Parser::createExprTree(){
	AST *rightNode = operands.top();
	if(!operands.empty()) operands.pop();
	AST *leftNode = operands.top();
	if(!operands.empty()) operands.pop();
	AST *oNode = operators.top();
	if(!operators.empty()) operators.pop();

	pkb->setFirstDescendant(oNode, leftNode);
	pkb->addSibling(leftNode, rightNode);

	operands.push(oNode);
}

bool Parser::isSameProc(PROG_LINE p1, PROG_LINE p2){
	//cout << "p1: " << p1 << ", p2: " << p2 << "\n";
	//cout << "lastProcEndLine: " << lastProcEndLine << "\n";
	if(lastProcEndLine == 0)
		return true;
	else if(p1 <= lastProcEndLine && p2 <= lastProcEndLine)
		return true;
	else if(p1 > lastProcEndLine && p2 > lastProcEndLine)
		return true;
	else
		return false;
}

void Parser::insertFollows(PROG_LINE p1, PROG_LINE p2){
	if(p1 != 0 && p2 != 1 && isSameProc(p1, p2))
	{
		//cout << "follows: " << p1 << ", " << p2 << "\n";
		pkb->insertFollows(p1, p2);	
	}
}

// remember to refine.
void Parser::error(int errorCode){
	if(errorMsg.empty()){
		switch(errorCode){
		case INVALID_PROCEDURE:
			errorMsg = "Source Code Error(): invalid procedure";
			break;
		case INVALID_EXPR:
			errorMsg = "Source Code Error(): invalid expression";
			break;
		case INVALID_PROCEDURE_NAME:
			errorMsg = "Source Code Error(): invalid/repeated procedure name";
			break;
		case INVALID_WHILE_NAME:
			errorMsg = "Source Code Error(): invalid while var_name";
			break;
		case INVALID_IF_NAME:
			errorMsg = "Source Code Error(): invalid if var_name";
			break;
		case INVALID_IF_FORMAT:
			errorMsg = "Source Code Error(): invalid if format. missing then or else.";
			break;
		case MISSING_BRACKET:
			errorMsg = "Source Code Error(): missing bracket";
			break;
		case MISSING_COLON:
			errorMsg = "Source Code Error(): missing colon";
			break;
		case INVALID_CALL_NAME:
			//errorMsg = "Source Code Error(): invalid call proc_name";
			break;
		default:
			errorMsg  = "Source Code Error(): unknown error";
		}
	}
}
