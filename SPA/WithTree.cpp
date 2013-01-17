#include "WithTree.h"


WithTree::WithTree(void)
{
}


WithTree::~WithTree(void)
{
}

RELATION_LIST* WithTree::evaluateWith(QTREE* withTree){
	
	RELATION_LIST * returnList;

	//split the withTree into 4 subtrees
	//the order of withTree is: LHS->attri->RHS->attri
	leftVariable = withTree->getFirstDescendant();
	rightVariable = leftVariable->getRightSibling();
	rightAttribute = rightVariable->getRightSibling();

	//rightVariable is not specified, which means the value at right hand side is a fixed string or int;
	//eg. proc.Name = "John" or  constant.Value = 5
	if(rightVariable->getType()==ANY){
		returnList = findLeftVariable();
	}else{
		//right variable is specified. Need to find two unknowns
		//eg. proc.Name = var.Name 
		returnList = findMatchedPairs(leftVariable,rightVariable);
	}

	return returnList;
}

//the right attribute is either string index of integer value
RELATION_LIST* WithTree::findLeftVariable(){
	TYPE attributeType = rightAttribute->getType();

	//find name in procedure table, variable table and call table
	if(attributeType==PARAM){
		
	}
	//find integer value in constant, progline, statement
	if(attributeType==INTEGER){
		
	}
}