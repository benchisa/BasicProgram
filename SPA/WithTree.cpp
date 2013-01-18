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
		returnList = findMatchedPairs();
	}

	return returnList;
}

//the right attribute is either string index of integer value
RELATION_LIST* WithTree::findLeftVariable(){
	//declare variables in the method
	RELATION_LIST * returnList;
	TYPE rightAttributeType = rightAttribute->getType();

	//set returnList to NULL as default return value
	returnList = NULL;
	
	//find name in procedure table, variable table and call table
	if(rightAttributeType==PARAM){
		returnList = WithTree::findLeftString();
	}
	//find integer value in constant, progline, statement
	if(rightAttributeType==INTEGER){
		returnList = WithTree::findLeftInteger();
	}

	return returnList;
}
RELATION_LIST* WithTree::findLeftString(){
	RELATION_LIST* returnList;

	returnList = NULL;

	//To get the string value of right attribute from Query parameter table
	int rightAttributeIndex = rightAttribute->getData();
	QUERYVALUE::iterator itr1;
	itr1 = qrValue->find(rightAttributeIndex);
	string rightAttributeValue = itr1->second;

	//To detect the type of left variable
	int leftAttributeIndex = leftVariable->getData();
	QUERYTABLE::iterator itr2;
	itr2 = qrTable->find(leftAttributeIndex);
	TYPE leftAttributeType = itr2->second;
	
	switch(leftAttributeType){
	case PROCEDURE:
		int procIndex = pkb->getProcIndex(rightAttributeValue);
		if(procIndex!=NULL) 
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(PROCEDURE,procIndex),pair<int,int>(PARAM,rightAttributeIndex)));
		break;
	case VARIABLE:
		int varIndex = pkb->getVarIndex(rightAttributeValue);
		if(varIndex!=NULL)
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(VARIABLE,varIndex),pair<int,int>(PARAM,rightAttributeIndex)));
		break;
	case CALL:
		Procedure * caller;
		caller = pkb->getCall(NULL,rightAttributeValue);

		if(caller!=NULL){
			int callerIndex = pkb->getProcIndex(caller->getProcName());
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(CALL,callerIndex),pair<int,int>(PARAM,rightAttributeIndex)));
		}
		break;
	}

	return returnList;
}
RELATION_LIST* WithTree::findLeftInteger(){
	RELATION_LIST* returnList;

	returnList = NULL;

	//To get the string value of right attribute from Query parameter table
	int rightAttributeValue = rightAttribute->getData();
	
	//To detect the type of left variable
	int leftAttributeIndex = leftVariable->getData();
	QUERYTABLE::iterator itr2;
	itr2 = qrTable->find(leftAttributeIndex);
	TYPE leftAttributeType = itr2->second;
	
	switch(leftAttributeType){
	case CONSTANT:
		int constantIndex = pkb->getConstantIndex(rightAttributeValue);
		if(constantIndex!=NULL) 
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(CONSTANT,constantIndex),pair<int,int>(INTEGER,rightAttributeValue)));
		break;
	case PROGLINE:
		int maxProgLine = pkb->getMaxProgLine();
		int queryProgLine = rightAttributeValue;

		if( queryProgLine <= maxProgLine)
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(PROGLINE,rightAttributeValue),pair<int,int>(INTEGER,rightAttributeValue)));
		break;
	case STATEMENT:
		int maxStatementNum = pkb->getMaxStatementNum();
		int queryStatementNum = rightAttributeValue;

		if(queryStatementNum <= maxStatementNum){
			returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(STATEMENT,rightAttributeValue),pair<int,int>(INTEGER,rightAttributeValue)));
		}
		break;
	}

	return returnList;
}

RELATION_LIST* WithTree::findMatchedPairs(){
	//get left variable and right variable

	RELATION_LIST * returnList;

	//convert the left and right variables from query table to known types
	QUERYTABLE::iterator itr1;
	//right
	int rightVarialbeIndex = rightVariable->getData();
	itr1 = qrTable->find(rightVarialbeIndex);
	TYPE rightVariableType = itr1->second;

	//left
	int leftVarialbeIndex = leftVariable->getData();
	itr1 = qrTable->find(leftVarialbeIndex);
	TYPE leftVariableType = itr1->second;

	//set returnList to NULL as default return value
	returnList = NULL;
	
	switch(leftVariableType){
	case PROCEDURE:
		SIZE procTableSize = pkb->getProceTableSize();

		switch(rightVariableType){
		case PROCEDURE:
			for(int i=1;i<=procTableSize;i++){
				returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(PROCEDURE,i),pair<int,int>(PROCEDURE,i)));
			}
		case VARIABLE:
			SIZE varTableSize = pkb->getVarTableSize();
			for(int i=1;i<=procTableSize;i++){
				for(int j=1;j<=varTableSize;j++){
					string currentProcName = pkb->getProcedure(i)->getProcName();
					string currentVarName = pkb->getVarName(j);

					if(currentProcName==currentVarName)
						returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(PROCEDURE,i),pair<int,int>(VARIABLE,j)));
				}
			}
		case CALL:
			SIZE callTableSize = pkb->getCallTableSize();
			for(int i=1;i<=procTableSize;i++){
				for(int j=1;j<=callTableSize;j++){
					string currentProcName = pkb->getProcedure(i)->getProcName();
					string currentCallerName = pkb->getCALLPair(j).first->getProcName();

					if(currentProcName==currentCallerName){
						int callerProcIndex= pkb->getProcIndex(currentCallerName);
						returnList->insert(pair<pair<int,int>,pair<int,int>>(pair<int,int>(PROCEDURE,i),pair<int,int>(CALL,callerProcIndex)));
					}
				}
			}
		}
	case VARIABLE:

	case CALL:

	case CONSTANT:
	case PROGLINE:
	case STATEMENT:
	}
	return returnList;
}