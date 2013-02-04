#include "PKB.h"



PKB::PKB(void){
	rootAST=NULL;
	tailAST=NULL;
	follows = new Follows;
	parent = new Parent;
	modifies = new Modifies;
	uses = new Uses;
	varTable = new 	VarTable;
	procTable = new ProcTable;
	treeMap = new hash_map<STATEMENT_NUM,AST_LIST>;
	callTable=new CallTable;
	constantTable=new ConstantTable;
	progLineTable=new unordered_multimap<PROG_LINE, STATEMENT_NUM>;
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
	delete callTable;
	delete constantTable;
	delete progLineTable;
}
AST* PKB::createAST(ASTNODE_TYPE type,PROG_LINE progLine,STATEMENT_NUM stmt, int data){
	
	//for prog line table.
	unordered_map<PROG_LINE, STATEMENT_NUM>::iterator p_itr;
	p_itr=progLineTable->find(progLine);
	if (p_itr==progLineTable->end())
	{
		progLineTable->insert(make_pair(progLine, stmt));
	}
	else
	{
		bool exist=false;
			
		for (p_itr=progLineTable->begin(); p_itr!=progLineTable->end(); p_itr++)
			{
			
				if (p_itr->first==progLine &&p_itr->second==stmt)
				{	
					exist=true;
					break;
				}
			}
			if (!exist)
			{
				progLineTable->insert(make_pair(progLine, stmt));
				
			}
	}

	AST newAST(type, progLine, stmt,data);
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
bool PKB::setTail(AST* tailNode){
	if(tailAST==NULL){
		tailAST = tailNode;
		return true;
	}
	return false;
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
ASTNODE_TYPE PKB::getType(AST* currentAST){
	return (*currentAST).getRootType();
}
STATEMENT_NUM PKB::getStatementNum(AST* currentAST){
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
AST* PKB::getTail(){
	return tailAST;
}

PROG_LINE PKB::getProgLine(AST* currentAST)
{
	return currentAST->getRootProgLineNum();
}

STATEMENT_LIST * PKB::getStmtList(PROG_LINE progLine)
{   
	if (progLine==NULL)
	{
		return nullptr;
	}
	STATEMENT_LIST * stmtList=new STATEMENT_LIST;

	unordered_multimap<int, int>::iterator p_itr;
	p_itr=progLineTable->find(progLine);
	
	if (p_itr!=progLineTable->end())
	{
		for (p_itr; p_itr!=progLineTable->end(); p_itr++)
		{
			if (p_itr->first==progLine)
			{
				stmtList->push_back(p_itr->second);
			}
		}
		return stmtList;
	}
	else
	{
		return nullptr;
	}
	

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
VAR_NAME PKB::getVarName(VAR_INDEX index)
{
	return *(*varTable).getVarName(index);
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

//Function of CallTable

CALL_INDEX PKB::insertCall(PROC_NAME caller, PROC_NAME callee)
{
	return callTable->insertCall(caller, callee);
}

SIZE PKB::getCallTableSize()
{
	return callTable->getCallTableSize();
}

list<CALL_PAIR>  PKB::getCall(PROC_NAME caller,PROC_NAME callee)
{
	return callTable->getCall(caller, callee);
}
CALL_PAIR PKB::getCALLPair(CALL_INDEX index)
{
	return callTable->getCallPair(index);
}

CALL_INDEX PKB::getCallPairIndex(PROC_NAME caller,PROC_NAME callee)
{
	return callTable->getCallPairIndex(caller, callee);
}

bool PKB::isExistsCall(PROC_NAME caller,PROC_NAME callee)
{
	return callTable->isExistsCall(caller, callee);
}

CALL_LIST  PKB::getAllCalls()
{
	return callTable->getAllCalls();
}

//Functions of constant table
CONSTANT_INDEX PKB::insertConst(int constantValue)
{
	return constantTable->insertConst(constantValue);
}
SIZE PKB::getConstantTableSize()
{
	return constantTable->getConstantTableSize();
}
int PKB::getConstantValue(CONSTANT_INDEX index)
{
	return constantTable->getConstantValue(index);

}
CONSTANT_INDEX PKB::getConstantIndex(int constantValue)
{
	return constantTable->getConstantIndex(constantValue);
}

bool PKB::isExists(int constantValue)
{
	return constantTable->isExists(constantValue);
}
CONSTANT_LIST *  PKB::getAllConstant()
{
	return constantTable->getAllConstant();
}

// function for CFG
void PKB::createCFG(int size)
{
	cfg = new CFG(size);
}

bool PKB::addEdge(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->addEdge(p1, p2);
}

bool PKB::isNext(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->isNext(p1, p2);
}

NEXT_LIST PKB::getNext(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->getNext(p1, p2);
}


list<PROG_LINE> PKB::findAllPaths(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->findAllPaths(p1, p2);
}

PROG_LINE PKB::getMaxProgLine()
{
	int lastProc=procTable->getProceTableSize();
	Procedure * proc=procTable->getProcedure(lastProc);
	return proc->getEndProgLine();

}
