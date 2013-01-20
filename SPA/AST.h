#pragma once
#include "Global_Data_Abstraction.h"

class AST
{
	public:
		AST(void);
		~AST(void);
		AST(ASTNODE_TYPE type,STATEMENT_NUM stmt,INDEX data);
		bool setFirstDescendant( AST* firstDescendant);
		bool setAncestor(AST* ancestor);
		bool addSibling(AST* rightSibling);
		bool setTail(AST* tail);

		//AST* getRoot();
		AST* getFirstDescendant();
		AST* getRightSibling();
		AST* getLeftSibling();
		AST* getAncestor();
		AST* getTail();

		ASTNODE_TYPE getRootType();
		PROG_LINE getRootStatementNum();
		INDEX getRootData();
private:

	ASTNODE_TYPE myType;
	STATEMENT_NUM myStmt;
	int myData;

	AST* leftSibling;
	AST* rightSibling;
	AST* ancestor;
	AST* firstDescendant;
	AST* root;
	AST* tail;
};

