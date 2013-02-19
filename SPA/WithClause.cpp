#include "WithClause.h"


WithClause::WithClause(PKB* pkb, DesignExtractor* extractor,QUERYTABLE* qrTable, QUERYPARAM* qrParam)
{
	this->pkb = pkb;
	this->extractor = extractor;
	this->qrTable = qrTable;
	this->qrParam = qrParam;
}


WithClause::~WithClause(void)
{
}

RELATION_LIST* WithClause::evaluateWithTree(QTREE* withTree){
	
	RELATION_LIST * returnList;

	//split the withTree into 4 subtrees
	//the order of withTree is: LHS->attri->RHS->attri
	leftVariable = withTree->getFirstDescendant();
	leftAttribute = leftVariable->getRightSibling();
	rightVariable = leftAttribute->getRightSibling();
	rightAttribute = rightVariable->getRightSibling();

	//rightVariable is not specified, which means the value at right hand side is a fixed string or int;
	//eg. proc.Name = "John" or  constant.Value = 5
	if(rightVariable->getType()==ANY&&leftVariable->getType()==ANY){
		//both sides are discovered, test if they are equal
		returnList= WithClause::findEqual();
	}else if(rightVariable->getType()!=ANY&&leftVariable->getType()!=ANY){
		//right variable is specified. Need to find two unknowns
		//eg. proc.Name = var.Name 
		returnList = WithClause::findMatchedPairs();
	}else{
		returnList = WithClause::findOneVariable();
	}

	return returnList;
}
// to do
RELATION_LIST* WithClause::findEqual(){
	RELATION_LIST * returnList = new RELATION_LIST();

	TYPE rightAttrType,leftAttrType;
	int rightAttrValue,leftAttrValue;

	rightAttrType = rightAttribute->getType();
	rightAttrValue = rightAttribute->getData();
	leftAttrType = leftAttribute->getType();
	leftAttrValue = leftAttribute->getData();

	if(rightAttrType==leftAttrType&&rightAttrValue==leftAttrValue){
		returnList->push_back(pair<int,int>(leftAttrValue,rightAttrValue));
	}else{
		switch(leftAttrType){
		case CALL:
			{	
				//get string value from param table
				QUERYPARAM::iterator itr1;
				itr1 = qrParam->find(rightAttrValue);
				string paramString = itr1->second;

				//get caller name
				PROC_NAME callerName = pkb->getProcedure(leftAttrValue)->getProcName();
				//get callees
				CALLPAIR_LIST callList;
				callList = pkb->getCall(callerName,"");

				//set flag
				bool found = false;
				if(callList.size()>0){
					CALLPAIR_LIST::iterator itr;
					for(itr = callList.begin();itr!=callList.end();itr++){

						string calleeName = itr->second;
						//compare the two values
						if(calleeName == paramString){
							found = true;
							break;
						}
					}
				}
				if(found) 
					returnList->push_back(pair<int,int>(leftAttrValue,rightAttrValue));
				else
					returnList = NULL;
			}
			break;
		case PROCEDURE:
			{
				//get string value from param table
				QUERYPARAM::iterator itr1;
				itr1 = qrParam->find(rightAttrValue);
				string paramString = itr1->second;
				
				//get the proc name
				string procName = pkb->getProcedure(leftAttrValue)->getProcName();

				//compare the two values
				if(procName==paramString) 
					returnList->push_back(pair<int,int>(leftAttrValue,rightAttrValue));
				else 
					returnList =NULL;
			}
			break;
		case VARIABLE:
			{
				//get string value from param table
				QUERYPARAM::iterator itr1;
				itr1 = qrParam->find(rightAttrValue);
				string paramString = itr1->second;
				
				//get the var name
				string varName = pkb->getVarName(leftAttrValue);

				//compare the two values
				if(varName==paramString) 
					returnList->push_back(pair<int,int>(leftAttrValue,rightAttrValue));
				else 
					returnList =NULL;
			}
			break;
		default://for all the statements
			if(rightAttrType==CONSTANT){
				if(leftAttrValue==rightAttrValue)
					returnList->push_back(pair<int,int>(leftAttrValue,rightAttrValue));
				else
					returnList =NULL;
			}else{
				returnList = NULL;
			}
			break;
		}
	}
	return returnList;
}
//the right attribute is either string index of integer value
RELATION_LIST* WithClause::findOneVariable(){
	//declare variables in the method
	RELATION_LIST * returnList = new RELATION_LIST();
	TYPE rightAttributeType = rightAttribute->getType();
	TYPE leftAttributeType = leftAttribute->getType();

	//set returnList to NULL as default return value
	//returnList = NULL;
	
	//find name in procedure table, variable table and call table
	if(rightAttributeType==PARAM||leftAttributeType==PARAM){
		returnList = WithClause::findOneString();
	}else if(rightAttributeType==CONSTANT||leftAttributeType==CONSTANT){ //find integer value in constant, progline, statement
		returnList = WithClause::findOneInteger();
	}
	if(returnList->size()<1) return NULL;
	return returnList;
}
RELATION_LIST* WithClause::findOneString(){
	RELATION_LIST* returnList = new RELATION_LIST();
	int knownAttrIndex, unknownAttrIndex;
	string knownAttrValue;
	TYPE unknownAttrType;
	bool isLR;

	TYPE rightAttributeType = rightAttribute->getType();
	TYPE leftAttributeType = leftAttribute->getType();

	//To detect which attribute is known and which is unknown
	if(rightAttributeType == PARAM){ //right is known
		//To get the string value of known(right) attribute from Query parameter table
		knownAttrIndex = rightAttribute->getData();
		QUERYPARAM::iterator itr1;
		itr1 = qrParam->find(knownAttrIndex);
		knownAttrValue = itr1->second;

		//To detect the type of unknown(left) variable
		int unknownAttrIndex = leftVariable->getData();
		QUERYTABLE::iterator itr2;
		itr2 = qrTable->find(unknownAttrIndex);
		unknownAttrType = itr2->second;
		isLR = true;
	}else{
		//To get the string value of known(left) attribute from Query parameter table
		knownAttrIndex = leftAttribute->getData();
		QUERYPARAM::iterator itr1;
		itr1 = qrParam->find(knownAttrIndex);
		knownAttrValue = itr1->second;

		//To detect the type of unknown(right) variable
		int unknownAttrIndex = rightVariable->getData();
		QUERYTABLE::iterator itr2;
		itr2 = qrTable->find(unknownAttrIndex);
		unknownAttrType = itr2->second;
		isLR = false;
	}
	
	
	switch(unknownAttrType){
	case PROCEDURE:
		{int procIndex = pkb->getProcIndex(knownAttrValue);
		if(procIndex!=NULL) {
			if(isLR)
				returnList->push_back(pair<int,int>(procIndex,knownAttrIndex));
			else
				returnList->push_back(pair<int,int>(knownAttrIndex,procIndex));
		}
		}
		break;
	case VARIABLE:
		{int varIndex = pkb->getVarIndex(knownAttrValue);
		if(varIndex!=NULL){
			if(isLR)
				returnList->push_back(pair<int,int>(varIndex,knownAttrIndex));
			else
				returnList->push_back(pair<int,int>(knownAttrIndex,varIndex));
		}
		}
		break;
	case CALL:
		{CALLPAIR_LIST callList;
		callList = pkb->getCall("",knownAttrValue);

		if(callList.size()>0){
			CALLPAIR_LIST::iterator itr;
			for(itr = callList.begin();itr!=callList.end();itr++){
				string callerName = itr->first;
				string calleeName = itr->second;

				if(calleeName == knownAttrValue){
					int callerIndex = pkb->getProcIndex(callerName);
					if(isLR)
						returnList->push_back(pair<int,int>(callerIndex,knownAttrIndex));
					else
						returnList->push_back(pair<int,int>(knownAttrIndex,callerIndex));
				}
			}
		}
		}
		break;
	}

	return returnList;
}
RELATION_LIST* WithClause::findOneInteger(){
	RELATION_LIST* returnList = new RELATION_LIST();
	int knownAttrIndex, unknownAttrIndex;
	int knownAttrValue;
	TYPE unknownAttrType;
	bool isLR; //correct order is left = right
	
	TYPE rightAttributeType = rightAttribute->getType();
	TYPE leftAttributeType = leftAttribute->getType();
	
	if(rightAttributeType ==CONSTANT){
		//To get the string value of known(right) attribute from Query parameter table
		knownAttrValue = rightAttribute->getData();
	
		//To detect the type of unknown(left) variable
		unknownAttrIndex = leftVariable->getData();
		QUERYTABLE::iterator itr2;
		itr2 = qrTable->find(unknownAttrIndex);
		unknownAttrType = itr2->second;
		isLR = true;
	}else{
		//To get the string value of known(left) attribute from Query parameter table
		knownAttrValue = leftAttribute->getData();
	
		//To detect the type of unknown(right) variable
		unknownAttrIndex = rightVariable->getData();
		QUERYTABLE::iterator itr2;
		itr2 = qrTable->find(unknownAttrIndex);
		unknownAttrType = itr2->second;
		isLR = false;
	}
	
	
	switch(unknownAttrType){
	case CONSTANT:
		{int constantIndex = pkb->getConstantIndex(knownAttrValue);
		if(constantIndex!=NULL){
			if(isLR)
				returnList->push_back(pair<int,int>(constantIndex,knownAttrValue));
			else
				returnList->push_back(pair<int,int>(knownAttrValue,constantIndex));
		}
		}
		break;
	case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
		int queryProgLine = knownAttrValue;

		if( queryProgLine <= maxProgLine)
				returnList->push_back(pair<int,int>(knownAttrValue,knownAttrValue));	
		}
		break;
	case STATEMENT:
		{int maxStatementNum = pkb->getMaxStatementNum();
		int queryStatementNum = knownAttrValue;

		if(queryStatementNum <= maxStatementNum){
			returnList->push_back(pair<int,int>(knownAttrValue,knownAttrValue));
		}
		}
		break;
	case ASSIGNMENT:
		{int maxStmtNum = pkb->getMaxStatementNum();
		int queryStmtNum = knownAttrValue;

		if(queryStmtNum <= maxStmtNum&&extractor->isStatementTypeOf(ASSIGNMENT,queryStmtNum)){
				returnList->push_back(pair<int,int>(queryStmtNum,queryStmtNum));
			}
		}
		break;
	case CALL:
		{int maxStmtNum = pkb->getMaxStatementNum();
		int queryStmtNum = knownAttrValue;

		if(queryStmtNum <= maxStmtNum&&extractor->isStatementTypeOf(CALL,queryStmtNum)){
				returnList->push_back(pair<int,int>(queryStmtNum,queryStmtNum));
			}
		}
		break;
	case WHILE:
		{int maxStmtNum = pkb->getMaxStatementNum();
		int queryStmtNum = knownAttrValue;

		if(queryStmtNum <= maxStmtNum&&extractor->isStatementTypeOf(WHILE,queryStmtNum)){
				returnList->push_back(pair<int,int>(queryStmtNum,queryStmtNum));
			}
		}
		break;
	case IF:
		{int maxStmtNum = pkb->getMaxStatementNum();
		int queryStmtNum = knownAttrValue;

		if(queryStmtNum <= maxStmtNum&&extractor->isStatementTypeOf(IF,queryStmtNum)){
				returnList->push_back(pair<int,int>(queryStmtNum,queryStmtNum));
			}
		}
		break;
	}

	return returnList;
}

RELATION_LIST* WithClause::findMatchedPairs(){
	//get left variable and right variable

	RELATION_LIST * returnList = new RELATION_LIST();

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
	
	switch(leftVariableType){
	//p.ProcName = 
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
	//var.VarName = 
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
	//c.callName = or c.stmt#=
	case CALL:
		{SIZE callTableSize = pkb->getCallTableSize();
		//inner switch
		switch(rightVariableType){
		case CALL:
			{	
				list<string> callerList;
					for(int i=1;i<=callTableSize;i++){
						PROC_NAME callerName = pkb->getCALLPair(i).first;
						callerList.push_back(callerName);
				}
					//c1.stmt#=c2.stmt#
				if(leftAttribute->getType()==INTEGER&&rightAttribute->getType()==INTEGER){
					if(callerList.size()>0){
						callerList.sort();
						callerList.unique();

						list<string>::iterator callerListItr;
						for(callerListItr=callerList.begin();callerListItr != callerList.end();callerListItr++){
							PROC_INDEX callerProcIndex = pkb->getProcIndex(*callerListItr);
							returnList->push_back(pair<int,int>(callerProcIndex,callerProcIndex));
						}

					}	//c1.procName = c2.procName
				}else if(leftAttribute->getType()==STRING&&rightAttribute->getType()==STRING){
					for(int i=1;i<=callTableSize;i++){
						for(int j =1;j<=callTableSize;i++){
							CALL_PAIR callPair1 = pkb->getCALLPair(i);
							CALL_PAIR callPair2 = pkb->getCALLPair(j);
							string callee1 = callPair1.second;
							string callee2 = callPair2.second;
							//two procs call the same proc, add to the return list
							if(callee1==callee2){
								PROC_INDEX caller1 = pkb->getProcIndex(callPair1.first);
								PROC_INDEX caller2 = pkb->getProcIndex(callPair2.first);
								returnList->push_back(pair<int,int>(caller1,caller2));
							}

						}
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
		case STATEMENT:
			{DATA_LIST * callStmts;
			callStmts = extractor->getAllCallStmts();

			if(callStmts!=NULL){
				DATA_LIST::iterator itr;
				for(itr= callStmts->begin();itr!=callStmts->end();itr++){
					returnList->push_back(pair<int,int>(*itr,*itr));
				}
			}
			}
			break;
		case CONSTANT:
			{SIZE consTableSize = pkb->getConstantTableSize();

			if(consTableSize>0){
				for(int i =1;i<=consTableSize;i++){
					int currentConst = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(CALL,currentConst)){
						returnList->push_back(pair<int,int>(currentConst,currentConst));
					}
				}
			}
			}
			break;
		case PROGLINE:
			{int maxProgLine = pkb->getMaxProgLine();

			for(int i =1;i<=maxProgLine;i++){
				if(extractor->isStatementTypeOf(CALL,i)){
					returnList->push_back(pair<int,int>(i,i));
				}
			}
			}
			break;
		}
		}
		break;
	//c.Value = 
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
		case ASSIGNMENT:
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(ASSIGNMENT,currentConstantValue)){
						returnList->push_back(pair<int,int>(i,currentConstantValue));
					}
				}
			}
			break;
		case WHILE:
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(WHILE,currentConstantValue)){
						returnList->push_back(pair<int,int>(i,currentConstantValue));
					}
				}
			}
			break;
		case IF:
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(IF,currentConstantValue)){
						returnList->push_back(pair<int,int>(i,currentConstantValue));
					}
				}
			}
			break;
		case CALL:
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(CALL,currentConstantValue)){
						returnList->push_back(pair<int,int>(i,currentConstantValue));
					}
				}
			}
			break;
		}
		}
		break;
	//n.progLine# = 
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
		case ASSIGNMENT:	
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(ASSIGNMENT,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
			break;
		case WHILE:
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(WHILE,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
			break;
		case IF:
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(IF,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
			break;
		case CALL:
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(CALL,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
			break;
		}
		}
		break;
	//stmt.stmt# = 
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
		case ASSIGNMENT:
			{
				DATA_LIST * assignList;
				assignList = extractor->getAllAssigns();
				if(assignList != NULL){
					DATA_LIST::iterator itr;
					for(itr = assignList->begin();itr!=assignList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case WHILE:
			{
				DATA_LIST * whileList;
				whileList = extractor->getAllWhiles();
				if(whileList != NULL){
					DATA_LIST::iterator itr;
					for(itr = whileList->begin();itr!=whileList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case IF:
			{
				DATA_LIST * ifList;
				ifList = extractor->getAllIfs();
				if(ifList != NULL){
					DATA_LIST::iterator itr;
					for(itr = ifList->begin();itr!=ifList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case CALL:
			{
				DATA_LIST * callList;
				callList = extractor->getAllCallStmts();
				if(callList != NULL){
					DATA_LIST::iterator itr;
					for(itr = callList->begin();itr!=callList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		}
		}
		break;
	//a.stmt# = 
	case ASSIGNMENT:
		{
		//inner switch
		switch(rightVariableType){
		case STATEMENT:
			{
				DATA_LIST * assignList;
				assignList = extractor->getAllAssigns();
				if(assignList != NULL){
					DATA_LIST::iterator itr;
					for(itr = assignList->begin();itr!=assignList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case CONSTANT:
			{SIZE constantTableSize = pkb->getConstantTableSize();
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(ASSIGNMENT,currentConstantValue)){
						returnList->push_back(pair<int,int>(currentConstantValue,i));
					}
				}
			}
			}
			break;
		case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(ASSIGNMENT,i))
						returnList->push_back(pair<int,int>(i,i));	
				}
			}
		}
			break;
		case ASSIGNMENT:
			{
				DATA_LIST * assignList;
				assignList = extractor->getAllAssigns();
				if(assignList != NULL){
					DATA_LIST::iterator itr;
					for(itr = assignList->begin();itr!=assignList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		}
		}
		break;
	//w.stmt# = 
	case WHILE:
		{
		//inner switch
		switch(rightVariableType){
		case STATEMENT:
			{
				DATA_LIST * whileList;
				whileList = extractor->getAllWhiles();
				if(whileList != NULL){
					DATA_LIST::iterator itr;
					for(itr = whileList->begin();itr!=whileList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case CONSTANT:
			{SIZE constantTableSize = pkb->getConstantTableSize();
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(WHILE,currentConstantValue)){
						returnList->push_back(pair<int,int>(currentConstantValue,i));
					}
				}
			}
			}
			break;
		case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(WHILE,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
		}
			break;
		case WHILE:
			{
				DATA_LIST * whileList;
				whileList = extractor->getAllWhiles();
				if(whileList != NULL){
					DATA_LIST::iterator itr;
					for(itr = whileList->begin();itr!=whileList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		}
		}
		break;
	//if.stmt# = 
	case IF:
		{
		//inner switch
		switch(rightVariableType){
		case STATEMENT:
			{
				DATA_LIST * ifList;
				ifList = extractor->getAllIfs();
				if(ifList != NULL){
					DATA_LIST::iterator itr;
					for(itr = ifList->begin();itr!=ifList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		case CONSTANT:
			{SIZE constantTableSize = pkb->getConstantTableSize();
			if(constantTableSize!=0){
				for(int i =1;i<=constantTableSize;i++){
					int currentConstantValue = pkb->getConstantValue(i);
					if(extractor->isStatementTypeOf(IF,currentConstantValue)){
						returnList->push_back(pair<int,int>(currentConstantValue,i));
					}
				}
			}
			}
			break;
		case PROGLINE:
		{int maxProgLine = pkb->getMaxProgLine();
			if(maxProgLine!=0){
				for(int i =1;i<=maxProgLine;i++){
					if(extractor->isStatementTypeOf(IF,i)){
						returnList->push_back(pair<int,int>(i,i));
					}
				}
			}
		}
			break;
		case ASSIGNMENT:
			{
				DATA_LIST * ifList;
				ifList = extractor->getAllIfs();
				if(ifList != NULL){
					DATA_LIST::iterator itr;
					for(itr = ifList->begin();itr!=ifList->end();itr++){
						returnList->push_back(pair<int,int>(*itr,*itr));
					}
				}
			}
			break;
		}
		}
		break;
	}

	if(returnList->size()<1) return NULL;
	return returnList;
}