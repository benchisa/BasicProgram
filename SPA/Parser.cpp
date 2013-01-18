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
	curToken = tokens.at(0);
	prevToken = "";
	tokenIndex = 0;
	progLine = 0;
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
		if(tokenIndex < tokens.size()-1) 
			curToken = tokens.at(++tokenIndex);
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
		if(tokenIndex < tokens.size()-1) 
			curToken = tokens.at(++tokenIndex);
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
		pkb->getProcedure(curProcIndex)->setEndProgLine(progLine);
		if(matchToken(" ")){
			return true;
		}
		else
		{
			// add to procedureContainer
			lastProcEndLine = progLine;
			// run again
			return program();
		}
	}

	return true;
}

bool Parser::procedure(){
	if(matchToken("procedure")){
		if(name()) 
		{
			Procedure *cProc = pkb->createProc(prevToken, progLine+1);
			curProcIndex = pkb->insertProc(cProc);
			if(progLine == 0){
				curAST = pkb->createAST(PROCEDURE, 0, curProcIndex);
				if(pkb->setRootAST(curAST));
				else
				{
					return false;
				}
			}
		}
		else
		{
			error(INVALID_PROCEDURE_NAME);
			return false;
		}
		if(matchToken("{")){
			prevAST = curAST;
			curAST = pkb->createAST(STMT_LIST, 0, -1);
			if(!pkb->setFirstDescendant(prevAST, curAST))
				pkb->setAncestor(curAST, prevAST);

			if(stmtlst()) 
			{
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
			// can only add if it is not end of procedure
			if(curToken.compare("}") != 0) {
				insertFollows(containerIndex.back().first, progLine+1);
			}
			
			containerIndex.pop_back(); // remove the last container.
		}
		return true;
	}
	else if(!stmtlst()) {
		return false;
	}

	return true;
}

bool Parser::stmt(){
	// stmt: assign | while | if (bonus)
	if(stmt_while()){
		return true;
	}
	else if(stmt_if()){

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

bool Parser::stmt_if(){
	if(matchToken("if")){
		progLine++;

		return true;
	}
	return false;
}

bool Parser::stmt_while(){
	// while: while var_name '{' stmtlst '}'
	TOKEN tmpToken = prevToken; // use for follows

	if(matchToken("while"))
	{	
		progLine++;
		nesting = progLine;

		AST *whileNode = pkb->createAST(WHILE, progLine, -1);
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
			pkb->insertUses(WHILE, progLine, curVarIndex);

			leftNode = pkb->createAST(VARIABLE, progLine, curVarIndex);

			if(!pkb->setFirstDescendant(whileNode, leftNode))
				pkb->setAncestor(leftNode, whileNode);

			if(!containerIndex.empty())
			{
				if(progLine-1 != containerIndex.back().first){
					//cout << "Insertfollows241: " << progLine-1 << ", " << progLine << "\n";
					insertFollows(progLine-1, progLine);
				}

				pkb->insertParent(containerIndex.back().first, progLine);
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
					insertFollows(progLine-1, progLine);
				}
			}

			containerInfo.first = progLine;
			containerInfo.second = WHILE;
			containerIndex.push_back(containerInfo);

			if(matchToken("{")){
				rightNode = pkb->createAST(STMT_LIST, progLine, -1);
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
	progLine++;

	AST *assignNode = pkb->createAST(ASSIGNMENT, progLine, -1);
	AST *leftNode, *rightNode;

	if(!pkb->setFirstDescendant(curAST, assignNode))
	{
		pkb->addSibling(curAST, assignNode);
		pkb->setAncestor(assignNode, curAST->getAncestor());
	}

	curAST = assignNode;

	if(!containerIndex.empty() && progLine != 1){
		// not first line, and previous line not a container
		pkb->insertParent(containerIndex.back().first, progLine);
	
		// while {} a=b; a=b;
		if(progLine-1 != containerIndex.back().first && prevToken.compare("}") != 0)
		{	
			insertFollows(progLine-1, progLine);
		}
	}
	else
	{
		if(prevToken.compare("}") != 0) {
			insertFollows(progLine-1, progLine);
		}
	}

	if(name()){
		curVarIndex = pkb->insertVar(prevToken);
		pkb->insertModifies(PROCEDURE, curProcIndex, curVarIndex);
		pkb->insertModifies(ASSIGNMENT, progLine, curVarIndex);

		if(!containerIndex.empty())
		{
			vector<pair<PROG_LINE, TYPE>>::iterator itr = containerIndex.begin();
			while(itr != containerIndex.end()){
				//cout << "insertmodifies1: " << itr->first << ", " << curVarIndex << "\n";
				pkb->insertModifies(itr->second, itr->first, curVarIndex);
				itr++;
			}
		}

		leftNode = pkb->createAST(VARIABLE, progLine, curVarIndex);
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
				operators.push(pkb->createAST(PLUS, progLine, -1));
			else
			{
				if(pkb->getType(operators.top()) == MULTIPLY)
				{
					createExprTree();
				}
				else
				{
					operators.push(pkb->createAST(PLUS, progLine, -1));
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
				operators.push(pkb->createAST(MINUS, progLine, -1));
			else
			{
				if(pkb->getType(operators.top()) == MULTIPLY)
				{
					createExprTree();
				}
				else
				{
					operators.push(pkb->createAST(MINUS, progLine, -1));
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
				operators.push(pkb->createAST(MULTIPLY, progLine, -1));
			else
			{
				operators.push(pkb->createAST(MULTIPLY, progLine, -1));

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
		pkb->insertUses(ASSIGNMENT, progLine, curVarIndex);

		if(!containerIndex.empty())
		{
			vector<pair<PROG_LINE, TYPE>>::iterator itr = containerIndex.begin();
			while(itr != containerIndex.end()){
				pkb->insertUses(itr->second, itr->first, curVarIndex);
				itr++;
			}
		}

		operands.push(pkb->createAST(VARIABLE, progLine, curVarIndex));
		return true;
	}
	else if(const_value())
	{
		operands.push(pkb->createAST(CONSTANT, progLine, atoi(prevToken.c_str())));
		return true;
	}
	else if(openBracket())
	{
		operators.push(pkb->createAST(BRACKET, progLine, -1));
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
			errorMsg = "Source Code Error(): invalid procedure name";
			break;
		case INVALID_WHILE_NAME:
			errorMsg = "Source Code Error(): invalid while var_name";
			break;
		case MISSING_BRACKET:
			errorMsg = "Source Code Error(): missing bracket";
			break;
		case MISSING_COLON:
			errorMsg = "Source Code Error(): missing colon";
			break;
		default:
			errorMsg  = "Source Code Error(): unknown error";
		}
	}
}