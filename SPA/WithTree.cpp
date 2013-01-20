#include "WithTree.h"


WithTree::WithTree(PKB* pkb)
{
	this->pkb = pkb;
}


WithTree::~WithTree(void)
{
}
RELATION_LIST* WithTree::evaluateWithTree(QTREE* qrTree,QUERYTABLE* qrTable, QUERYVALUE* qrValue){
	WithTree::setQueryTable(qrTable);
	WithTree::setQueryValue(qrValue);
	return WithTree::evaluateWith(qrTree);
}
void WithTree::setQueryTable(QUERYTABLE* qrTable){
	this->qrTable = qrTable;
}
void WithTree::setQueryValue(QUERYVALUE* qrValue){
	this->qrValue = qrValue;
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
		{int procIndex = pkb->getProcIndex(rightAttributeValue);
		if(procIndex!=NULL) 
			returnList->push_back(pair<int,int>(procIndex,rightAttributeIndex));
		}
		break;
	case VARIABLE:
		{int varIndex = pkb->getVarIndex(rightAttributeValue);
		if(varIndex!=NULL)
			returnList->push_back(pair<int,int>(varIndex,rightAttributeIndex));
		}
		break;
	case CALL:
		{CALLPAIR_LIST callList;
		callList = pkb->getCall(NULL,rightAttributeValue);

		if(callList.size()>0){
			CALLPAIR_LIST::iterator itr;
			for(itr = callList.begin();itr!=callList.end();itr++){
				string callerName = itr->first;
				string calleeName = itr->second;

				if(calleeName == rightAttributeValue){
					int callerIndex = pkb->getProcIndex(callerName);
					returnList->push_back(pair<int,int>(callerIndex,rightAttributeIndex));
				}
			}
		}
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
		{int constantIndex = pkb->getConstantIndex(rightAttributeValue);
		if(constantIndex!=NULL) 
			returnList->push_back(pair<int,int>(constantIndex,rightAttributeValue));
		}
		break;
	case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
		int queryProgLine = rightAttributeValue;

		if( queryProgLine <= maxProgLine)
			returnList->push_back(pair<int,int>(rightAttributeValue,rightAttributeValue));
		}
		break;
	case STATEMENT:
		{int maxStatementNum = pkb->getMaxStatementNum();
		int queryStatementNum = rightAttributeValue;

		if(queryStatementNum <= maxStatementNum){
			returnList->push_back(pair<int,int>(rightAttributeValue,rightAttributeValue));
		}
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
		{SIZE procTableSize = pkb->getProceTableSize();
		//inner switch
		switch(rightVariableType){
		case PROCEDURE:
			{for(int i=1;i<=procTableSize;i++){
				returnList->push_back(pair<int,int>(i,i));
			}
			}
			break;
		case VARIABLE:
			{SIZE varTableSize = pkb->getVarTableSize();
			for(int i=1;i<=procTableSize;i++){
				for(int j=1;j<=varTableSize;j++){
					string currentProcName = pkb->getProcedure(i)->getProcName();
					string currentVarName = pkb->getVarName(j);

					if(currentProcName==currentVarName)
						returnList->push_back(pair<int,int>(i,j));
				}
			}
			}
			break;
		case CALL:
			{SIZE callTableSize = pkb->getCallTableSize();
			for(int i=1;i<=procTableSize;i++){
				for(int j=1;j<=callTableSize;j++){
					string currentProcName = pkb->getProcedure(i)->getProcName();
					string currentCalleeName = pkb->getCALLPair(j).second;

					if(currentProcName==currentCalleeName){
						string currentCallerName = pkb->getCALLPair(j).first;
						int callerProcIndex= pkb->getProcIndex(currentCallerName);
						returnList->push_back(pair<int,int>(i,callerProcIndex));
					}
				}
			}
			}
			break;
		}
		}
		break;
	case VARIABLE:
		{SIZE varTableSize = pkb->getVarTableSize();
		//inner switch
		switch(rightVariableType){
		case VARIABLE:
			{for(int i=1;i<=varTableSize;i++){
				returnList->push_back(pair<int,int>(i,i));
			}
			}
			break;
		case PROCEDURE:
			{SIZE procTableSize = pkb->getProceTableSize();
			for(int i=1;i<=varTableSize;i++){
				for(int j=1;j<=procTableSize;j++){
					string currentProcName = pkb->getProcedure(j)->getProcName();
					string currentVarName = pkb->getVarName(i);

					if(currentProcName==currentVarName)
						returnList->push_back(pair<int,int>(i,j));
				}
			}
			}
			break;
		case CALL:
			{SIZE callTableSize = pkb->getCallTableSize();
			for(int i=1;i<=varTableSize;i++){
				for(int j=1;j<=callTableSize;j++){
					string currentVarName = pkb->getVarName(i);
					string currentCalleeName = pkb->getCALLPair(j).second;

					if(currentVarName==currentCalleeName){
						PROC_NAME currentCallerName = pkb->getCALLPair(j).first;
						int callerProcIndex= pkb->getProcIndex(currentCallerName);
						returnList->push_back(pair<int,int>(i,callerProcIndex));
					}
				}
			}
			}
			break;
		}
		}
		break;
	case CALL:
		{SIZE callTableSize = pkb->getCallTableSize();
		//inner switch
		switch(rightVariableType){
		case CALL:
			{list<string> callerList;
			for(int i=1;i<=callTableSize;i++){
				PROC_NAME callerName = pkb->getCALLPair(i).first;
				callerList.push_back(callerName);
			}
			if(callerList.size()>0){
				callerList.sort();
				callerList.unique();

				list<string>::iterator callerListItr;
				for(callerListItr=callerList.begin();callerListItr != callerList.end();callerListItr++){
					PROC_INDEX callerProcIndex = pkb->getProcIndex(*callerListItr);
					returnList->push_back(pair<int,int>(callerProcIndex,callerProcIndex));
				}

			}	
			}
			break;
		case VARIABLE:
			{SIZE varTableSize = pkb->getVarTableSize();
			for(int i=1;i<=callTableSize;i++){
				for(int j=1;j<=varTableSize;j++){
					PROC_NAME currentCalleeName = pkb->getCALLPair(i).second;
					string currentVarName = pkb->getVarName(j);

					if(currentCalleeName==currentVarName){
						PROC_NAME callerName = pkb->getCALLPair(i).first;
						PROC_INDEX callerIndex = pkb->getProcIndex(callerName);
						returnList->push_back(pair<int,int>(callerIndex,j));
				
					}
				}
			}
			}
			break;
		case PROCEDURE:
			{SIZE procTableSize = pkb->getProceTableSize();
			for(int i=1;i<=procTableSize;i++){
				for(int j=1;j<=callTableSize;j++){
					string currentProcName = pkb->getProcedure(i)->getProcName();
					string currentCalleeName = pkb->getCALLPair(j).second;

					if(currentProcName==currentCalleeName){
						PROC_NAME currentCallerName = pkb->getCALLPair(j).first;
						int callerProcIndex= pkb->getProcIndex(currentCallerName);
						returnList->push_back(pair<int,int>(callerProcIndex,i));
					}
				}
			}
			}
			break;
		}
		}
		break;

	case CONSTANT:
		{int constantTableSize = pkb->getConstantTableSize();
		//inner switch
		switch(rightVariableType){
		case CONSTANT:
			{if (constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					returnList->push_back(pair<int,int>(i,i));
				}
			}
			}
			break;
		case PROGLINE:
			{int maxProgLine = pkb->getMaxProgLine();
			if (constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(currentConstantValue<=maxProgLine);
					returnList->push_back(pair<int,int>(i,currentConstantValue));
				}
			}
			}
			break;
		case STATEMENT:
			{int maxStatementNum = pkb->getMaxStatementNum();
			if (constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(currentConstantValue<=maxStatementNum);
					returnList->push_back(pair<int,int>(i,currentConstantValue));
				}
			}
			}
			break;
		}
		}
		break;
	case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
		//inner switch
		switch(rightVariableType){
		case PROGLINE:
			{for(int i =1;i<=maxProgLine;i++){
				returnList->push_back(pair<int,int>(i,i));	
			}
			}
			break;
		case CONSTANT:
			{SIZE constantTableSize = pkb->getConstantTableSize();
			if (constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(currentConstantValue<=maxProgLine);
					returnList->push_back(pair<int,int>(currentConstantValue,i));
				}
			}
			}
			break;
		case STATEMENT:
			{for(int i =1;i<=maxProgLine;i++){
				returnList->push_back(pair<int,int>(i,i));
				}
			}
			break;
		}
		}
		break;
	case STATEMENT:
		{int maxStatementNum = pkb->getMaxStatementNum();
		//inner switch
		switch(rightVariableType){
		case STATEMENT:
			{for(int i =1;i<=maxStatementNum;i++){
				returnList->push_back(pair<int,int>(i,i));	
			}
			}
			break;
		case CONSTANT:
			{SIZE constantTableSize = pkb->getConstantTableSize();
			if (constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(currentConstantValue<=maxStatementNum);
					returnList->push_back(pair<int,int>(currentConstantValue,i));
				}
			}
			}
			break;
		case PROGLINE:
			{int maxProgLine =pkb->getMaxProgLine();
			for(int i =1;i<=maxProgLine;i++){
				returnList->push_back(pair<int,int>(i,i));
				}
			}
			break;
		}
		}
		break;
	}
	return returnList;
}