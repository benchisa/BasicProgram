#include "AST.h"
AST::AST(void){}
AST::~AST(void){}

AST::AST(ASTNODE_TYPE type,STATEMENT_NUM stmt,INDEX data){
	this->myType = type;
	this->myStmt = stmt;
	this->myData = data;

	leftSibling = NULL;
	rightSibling = NULL;
	ancestor = NULL;
	firstDescendant = NULL;
	root = NULL;
}

AST::AST(ASTNODE_TYPE type,PROG_LINE progLine,STATEMENT_NUM stmt,INDEX data)
{
	this->myProgLine=progLine;
	this->myStmt = stmt;
	this->myType=type;
	this->myData=data;

	leftSibling = NULL;
	rightSibling = NULL;
	ancestor = NULL;
	firstDescendant = NULL;
	root = NULL;
}
bool AST::setFirstDescendant(AST *firstDescendant){
	//if current tree does not have the first descendant
	// and the firstDescendant has not ancestor, set the links
	if(this->firstDescendant==NULL && (*firstDescendant).ancestor==NULL){
		this->firstDescendant = firstDescendant;
		(*firstDescendant).ancestor = this;
		return true;	
	}
	return false;
}
bool AST::setAncestor(AST* ancestor){
	if(this->ancestor==NULL){
		this->ancestor=ancestor;
		return true;
	}
	return false;
}
bool AST::addSibling(AST* sibling){
	if(rightSibling==NULL && (*sibling).leftSibling==NULL){
		rightSibling =sibling;
		(*sibling).leftSibling = this;
		return true;
	}
	return false;
}
AST* AST::getFirstDescendant(){
	return firstDescendant;
}
AST* AST::getRightSibling(){
	return rightSibling;
}
AST* AST::getLeftSibling(){
	return leftSibling;
}
AST* AST::getAncestor(){
	return ancestor;
}

ASTNODE_TYPE AST::getRootType(){
	return myType;
}
STATEMENT_NUM AST::getRootStatementNum(){
	return myStmt;
}



PROG_LINE AST::getRootProgLineNum()
{
	return myProgLine;
}
INDEX AST::getRootData(){
	return myData;
}

