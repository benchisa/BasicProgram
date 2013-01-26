#pragma once
#include "Global_Data_Abstraction.h"
#include "AST.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Procedure.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "Uses.h"
#include "CallTable.h"
#include "ConstantTable.h"
#include <hash_map>



class PKB
{	
	public:
		//constructor;
		PKB(void);
		~PKB(void);

		//Functions of AST
		AST* createAST(ASTNODE_TYPE type,STATEMENT_NUM stmt,INDEX data);
		bool setRootAST(AST* currentAST);
		bool setFirstDescendant(AST * currentAST, AST* firstDescendant);
		bool setAncestor(AST* currentAST, AST* ancestor);
		bool setTail(AST* tailNode);
		bool addSibling(AST*  currentAST,AST* newSibling);
		AST* getFirstDescendant(AST* currentAST);
		AST* getRightSibling(AST* currentAST);
		AST* getLeftSibling(AST* currentAST);
		AST* getAncestor(AST* currentAST);
		AST_LIST* getASTBy(STATEMENT_NUM statementNo);
		ASTNODE_TYPE getType(AST* currentAST);
		PROG_LINE getStatementNum(AST* currentAST);
		INDEX getData(AST* currentAST);
		AST* getRootAST();
		AST* getTail();
		
		//this is for new progline
		AST* createAST(ASTNODE_TYPE type,PROG_LINE progLine,STATEMENT_NUM stmt,INDEX data);
	
		PROG_LINE getProgLine(AST* currentAST);
		STATEMENT_LIST * getStmtList(PROG_LINE progLine);

		//Functions of VarTable
		VAR_INDEX insertVar(VAR_NAME varName);
		SIZE getVarTableSize();
		VAR_NAME getVarName(VAR_INDEX index);
		VAR_INDEX getVarIndex(VAR_NAME varName);
		bool isVarExists(VAR_NAME varName);
		VAR_LIST* getAllVar();
		

		//Functions of ProcTable
		Procedure * createProc(PROC_NAME procName, PROG_LINE startProgLine);
		void setEndProgLine(Procedure * currentProc,PROG_LINE endProgLine);
		PROC_INDEX insertProc(Procedure * proc);
		SIZE getProceTableSize();
		Procedure * getProcedure(PROC_INDEX index);
		PROC_INDEX getProcIndex(PROC_NAME name);
		bool isProcExists(PROC_NAME procName);
		bool isInSameProc(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		PROC_LIST * getAllProc();
		

		//Functions of Follows
		
		bool insertFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		bool isFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		FOLLOWS_LIST getFollows(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		
		

		//Functions of Modifies

		bool insertModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);
		MODIFIES_LIST getModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);
		bool isModifies(MODIFIES_TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);
	
	
		//Functions of Parent
		bool insertParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		bool isParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		PARENT_LIST getParent(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
	

		//Functions of Uses
		bool insertUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex);
		USES_LIST getUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex);
		bool isUses(USES_TYPE type, USES_INDEX index, VAR_INDEX varIndex);


		//Functions of Call Table
		CALL_INDEX insertCall(PROC_NAME caller, PROC_NAME callee);
		SIZE getCallTableSize();

		//if caller is empty, return callee procedure
		//if callee is empty, return caller procedure
		list<CALL_PAIR> * getCall(PROC_NAME caller,PROC_NAME callee);
		CALL_PAIR getCALLPair(CALL_INDEX index);
		CALL_INDEX getCallPairIndex(PROC_NAME caller,PROC_NAME callee);
		bool isExistsCall(PROC_NAME caller,PROC_NAME callee);
		CALL_LIST *  getAllCalls();

		//Functions of constant table
		CONSTANT_INDEX insertConst(int constantValue);
		SIZE getConstantTableSize();
		int getConstantValue(CONSTANT_INDEX index);
		CONSTANT_INDEX getConstantIndex(int constantValue);
		bool isExists(int constantValue);
		CONSTANT_LIST *  getAllConstant();


		//Other functions
		PROG_LINE getMaxProgLine();
		STATEMENT_NUM getMaxStatementNum();
private:
		//Abastract Data Types stored in PKB
		AST* rootAST;
		AST* tailAST;
		Follows* follows;
		Parent* parent;
		Modifies* modifies;
		Uses* uses;
		VarTable* varTable;
		ProcTable* procTable;
		CallTable* callTable;
		ConstantTable* constantTable;
		hash_map<STATEMENT_NUM,AST_LIST>* treeMap;
		unordered_multimap<PROG_LINE, STATEMENT_NUM> * progLineTable;

	
		
};

