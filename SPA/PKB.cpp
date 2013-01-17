#include "PKB.h"



PKB::PKB(void){
	rootAST=NULL;
	follows = new Follows;
	parent = new Parent;
	modifies = new Modifies;
	uses = new Uses;
	varTable = new 	VarTable;
	procTable = new ProcTable;
	treeMap = new hash_map<STATEMENT_NUM,AST_LIST>;
}
PKB::~PKB(void){
	delete rootAST;
	delete follows;
	delete parent;
	delete modifies;
	delete uses;
	delete varTable;
	delete procTable;
	delete treeMap;
}
AST* PKB::createAST(ASTNODE_TYPE type,STATEMENT_NUM stmt,int data){
	
	AST newAST(type, stmt,data);
	AST * newASTP = new AST(newAST);
	//return the newAST without add into AST map, if the statement number is less than 1
	if(stmt<1) return newASTP;

	hash_map<STATEMENT_NUM,AST_LIST>::iterator itr;
	itr = (*treeMap).find(stmt);

	//add the new AST into AST map with stmt NO.
	AST_LIST currentList;

	if(itr!=(*treeMap).end()){
		//stmt exists in the AST map
		itr->second.push_back(newASTP);
	}else{
		//stmt does not exists in the AST map
		currentList.push_back(newASTP);
		(*treeMap)[stmt]=currentList;
	}

	return newASTP;
}
bool PKB::setRootAST(AST * currentAST){
	if(rootAST==NULL){
		rootAST = currentAST;
		return true;
	}
	return false;
}
bool PKB::setFirstDescendant(AST * currentAST, AST* firstDescendant){
	return (*currentAST).setFirstDescendant(firstDescendant);
}
bool PKB::setAncestor(AST* currentAST, AST* ancestor){
	return (*currentAST).setAncestor(ancestor);
}
bool PKB::setTail(AST* currentAST,AST* tail){
	return (*currentAST).setTail(tail);
}
bool PKB::addSibling(AST*  currentAST,AST* newSibling){
	return (*currentAST).addSibling(newSibling);
}
AST* PKB::getFirstDescendant(AST* currentAST){
	return (*currentAST).getFirstDescendant();
}
AST* PKB::getAncestor(AST* currentAST){
	return (*currentAST).getAncestor();
}
AST* PKB::getRightSibling(AST* currentAST){
	return (*currentAST).getRightSibling();
}
AST* PKB::getLeftSibling(AST* currentAST){
	return (*currentAST).getLeftSibling();
}
AST* PKB::getTail(AST* currentAST){
	return (*currentAST).getTail();
}
ASTNODE_TYPE PKB::getType(AST* currentAST){
	return (*currentAST).getRootType();
}
PROG_LINE PKB::getStatementNum(AST* currentAST){
	return (*currentAST).getRootStatementNum();
}
INDEX PKB::getData(AST* currentAST){
	return currentAST->getRootData();
}
AST_LIST* PKB::getASTBy(STATEMENT_NUM statementNo){
	hash_map<STATEMENT_NUM,AST_LIST>::iterator itr;

	itr = (*treeMap).find(statementNo);
	if(itr!=(*treeMap).end()){
		return &itr->second;
	}
	return NULL;
}
AST* PKB::getRootAST(){
	return rootAST;
	
}
		

//Functions of Procedure Table
Procedure * PKB::createProc(PROC_NAME procName, PROG_LINE startProgLine){
	return new Procedure(Procedure(procName,startProgLine));
	
}

void PKB::setEndProgLine(Procedure * currentProc, PROG_LINE endProgLine){
	currentProc->setEndProgLine(endProgLine);
}

PROC_INDEX PKB::insertProc(Procedure * proc)
{
	return (*procTable).insertProc(proc);
}

SIZE PKB::getProceTableSize()
{
	return (*procTable).getProceTableSize();
}

Procedure * PKB::getProcedure(PROC_INDEX index)
{
	return (*procTable).getProcedure(index);
}

PROC_INDEX PKB::getProcIndex(PROC_NAME name)
{
	return (*procTable).getProcIndex(name);
}

bool PKB::isProcExists(PROC_NAME procName)
{
	return (*procTable).isExists(procName);
}
bool PKB::isInSameProc(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*procTable).isInSameProc(stmt1, stmt2);
}
PROC_LIST * PKB::getAllProc()
{
	return (*procTable).getAllProc();
}


//Function of follows
bool PKB::insertFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*follows).insertFollows(stmt1, stmt2);
}

bool PKB::isFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*follows).isFollows(stmt1, stmt2);
}

FOLLOWS_LIST PKB::getFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return(*follows).getFollows(stmt1,stmt2);
}



//Functions of Parent
bool PKB::insertParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*parent).insertParent(stmt1, stmt2);
}
bool PKB::isParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*parent).isParent(stmt1, stmt2);
}
PARENT_LIST PKB::getParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2)
{
	return (*parent).getParent(stmt1, stmt2);
}


//Function of Modifies
bool PKB::insertModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex)
{
	return (*modifies).insertModifies(type, index, varIndex);
}
MODIFIES_LIST PKB::getModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex)
{
	return (*modifies).getModifies(type, index, varIndex);
}
bool PKB::isModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex)
{
	return (*modifies).isModifies(type, index, varIndex);
}

//Function of Uses
bool PKB::insertUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex)
{
	return (*uses).insertUses(type, index, varIndex);
}
USES_LIST PKB::getUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex)
{
	return (*uses).getUses(type, index, varIndex);
}
bool PKB::isUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex)
{
	return  (*uses).isUses(type, index, varIndex);
}

//Functions of VarTable
VAR_INDEX PKB::insertVar(VAR_NAME varName)
{
	return (*varTable).insertVar(varName);
}
SIZE PKB::getVarTableSize()
{
	return (*varTable).getVarTableSize();
}
VAR_NAME* PKB::getVarName(VAR_INDEX index)
{
	return (*varTable).getVarName(index);
}
VAR_INDEX PKB::getVarIndex(VAR_NAME varName)
{
	return (*varTable).getVarIndex(varName);
}
bool PKB::isVarExists(VAR_NAME varName)
{
	return (*varTable).isExists(varName);
}
VAR_LIST* PKB::getAllVar()
{
	return (*varTable).getAllVar();
}
