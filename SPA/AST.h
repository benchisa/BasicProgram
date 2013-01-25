#pragma once
#include "Global_Data_Abstraction.h"

class AST
{
	public:
		AST(void);
		~AST(void);
		AST(ASTNODE_TYPE type,STATEMENT_NUM stmt,INDEX data);
		//this is for new prog Line
	 	AST(ASTNODE_TYPE type,PROG_LINE progLine,STATEMENT_NUM stmt, INDEX data);
		
		bool setFirstDescendant( AST* firstDescendant);
		bool setAncestor(AST* ancestor);
		bool addSibling(AST* rightSibling);
		bool setTail(AST* tail);

		//AST* getRoot();
		AST* getFirstDescendant();
		AST* getRightSibling();
		AST* getLeftSibling();
		AST* getAncestor();

		ASTNODE_TYPE getRootType();
		PROG_LINE getRootStatementNum();
		INDEX getRootData();
		
		//this is for new prog line
		STATEMENT_NUM  getRootStmtNum();
		PROG_LINE getRootProgLineNum();
private:

	ASTNODE_TYPE myType;
	STATEMENT_NUM myStmt;
	ProgLine * myProgLine;
	int myData;

	AST* leftSibling;
	AST* rightSibling;
	AST* ancestor;
	AST* firstDescendant;
	AST* root;
};

