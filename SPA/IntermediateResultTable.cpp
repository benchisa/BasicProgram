#include "IntermediateResultTable.h"


IntermediateResultTable::IntermediateResultTable(SIZE tableSize,PKB* qrTree,QUERYTABLE* qrTable)
{
	this->tableSize = tableSize;
	this->pkb = pkb;
	this->qrTable = qrTable;
	qrVarTable = new int*[2];
	qrVarTable[0] = new int[tableSize];
	qrVarTable[1] = new int[tableSize];
}


IntermediateResultTable::~IntermediateResultTable(void)
{
}
bool IntermediateResultTable::isQrVarDiscovered(INDEX qrVarIndex){
	if (qrVarTable[0][qrVarIndex]!=1) return false;
	return true;
}
INDEX_LIST* IntermediateResultTable::getResultListOf(INDEX qrVarIndex){
	DATABASE::iterator currentTable;
	
	int tableNum = qrVarTable[1][qrVarIndex];
	if(tableNum>=0){
		currentTable = database.begin();
		advance(currentTable,tableNum);

		if(currentTable->isExists(qrVarIndex)){
			return &currentTable->getDataListOf(qrVarIndex);
		}
	}
	return NULL;
}
bool IntermediateResultTable::joinList(JOIN_ATTR qrVarIndex,INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newList){
	//case 1: both qrVar are not in the table, create a new table
	if(qrVarTable[0][firstQrVar]!=1&&qrVarTable[0][secondQrVar]!=1){
		//create a new table to store new list
		LinkedDataTable newTable;
		//add new list to new table
		newTable.addEntry(0,firstQrVar,secondQrVar,newList);
		//add the new table to database
		database.push_back(newTable);

		//update the information of the new added qrVars
		qrVarTable[0][firstQrVar] =1;
		qrVarTable[1][firstQrVar] = database.size()-1;
		qrVarTable[0][secondQrVar] =1;
		qrVarTable[1][secondQrVar] = database.size()-1;
	}
	//case 2:only one qrVar is in the table, merge into the existing table
	//one first qrVar exist
	if(qrVarTable[0][firstQrVar]==1&&qrVarTable[0][secondQrVar]!=1){
		//merge the new list in the existing table
		
		//get the table contains first qrVar
		int tableNum = qrVarTable[1][firstQrVar];
		DATABASE::iterator currentTable;
		currentTable = database.begin();
		advance(currentTable,tableNum);
		//merge the new list into table
		currentTable->addEntry(1,firstQrVar,secondQrVar,newList);
		//update the information of the second qrVar
		qrVarTable[0][secondQrVar] = 1;
		qrVarTable[1][secondQrVar] = qrVarTable[1][firstQrVar];
	}
	//only second qrVar exist
	if(qrVarTable[0][firstQrVar]!=1&&qrVarTable[0][secondQrVar]==1){
		//merge the new list in the existing table
		
		//get the table contains first qrVar
		int tableNum = qrVarTable[1][secondQrVar];
		DATABASE::iterator currentTable;
		currentTable = database.begin();
		advance(currentTable,tableNum);
		//merge the new list into table
		currentTable->addEntry(2,firstQrVar,secondQrVar,newList);
		//update the information of the second qrVar
		qrVarTable[0][firstQrVar] = 1;
		qrVarTable[1][firstQrVar] = qrVarTable[1][secondQrVar];
	}
	/*case 3:both qrVar are in the table
		a. both in one table
		b. first and second are in different table, merge two tables
	*/
	if(qrVarTable[0][firstQrVar]==1&&qrVarTable[0][secondQrVar]==1){
		//case a
		if(qrVarTable[1][firstQrVar]==qrVarTable[1][secondQrVar]){
			int tableNum = qrVarTable[1][firstQrVar];
			DATABASE::iterator currentTable;
			currentTable = database.begin();
			advance(currentTable,tableNum);
			//merge the new list into table
			return currentTable->mergeEntries(firstQrVar,secondQrVar,newList);
		}else{
			DATABASE::iterator table1;
			DATABASE::iterator table2;
		//caes b
			//merge the new list to first table
			//get the table contains first qrVar
			int tableNum1 = qrVarTable[1][firstQrVar];
			table1 = database.begin();
			advance(table1,tableNum1);
			table1->addEntry(1,firstQrVar,secondQrVar,newList);

			//merge the new list to second table
			int tableNum2 = qrVarTable[1][secondQrVar];
			table2 = database.begin();
			advance(table2,tableNum2);
			table2->addEntry(2,firstQrVar,secondQrVar,newList);

			LinkedDataTable  table2Pointer;
			table2Pointer = *table2;

			//merge two tables
			table1->mergeTable(firstQrVar,secondQrVar,&table2Pointer);

			//update qrVar info (table num)
			for(int i =0;i<tableSize;i++){
				if(qrVarTable[1][i]==tableNum2){
					qrVarTable[1][i]= tableNum1;
				}
			}

			qrVarTable[1][secondQrVar] = tableNum1;
		}
		
	}
	return true;
}
RAWDATA *IntermediateResultTable::findResultOf(DATA_LIST resultNodeList){
	RAWDATA * returnRaw;
	returnRaw = NULL;
	GROUP nodeGroup;
	DATA_LIST keyTable;
	

	//group the qrVar nodes order according to their database number
	DATA_LIST::iterator nodeListItr;
	int i =0;
	for(nodeListItr = resultNodeList.begin();nodeListItr!=resultNodeList.end();nodeListItr++){
		//add the key_value pair into group
		int key = qrVarTable[1][*nodeListItr];
		if(key<1){
			key =-1;
		}else{
			nodeGroup.insert(valuePair(key,*nodeListItr));
		}

		//store the new key into the keyTable
		DATA_LIST::iterator keyItr;
		keyItr = find(keyTable.begin(),keyTable.end(),key);
		if(keyItr==keyTable.end())
			keyTable.push_back(key);
	}

	//extract the qrVar names in one table, and merge them into the final returnRaw
	for(int i=0;i<keyTable.size();i++){
		pair<GROUP::iterator, GROUP::iterator> keyValueList;
		keyValueList = nodeGroup.equal_range(keyTable[i]);

		DATA_LIST * selectedVarList;

		for(GROUP::iterator keyValueItr=keyValueList.first;keyValueItr!=keyValueList.second;keyValueItr++){
			selectedVarList->push_back(keyValueItr->second);
		}
		//add into retrunRaw 
		if(keyTable[i]==-1){
			returnRaw = IntermediateResultTable::joinCombinations(returnRaw,selectedVarList);
		}else{
			returnRaw = IntermediateResultTable::joinRaw(returnRaw,keyTable[i],selectedVarList);
		}
	}

	//adjust the order of returnRaw
	//TODO

	return returnRaw;
}

RAWDATA * IntermediateResultTable::joinRaw(RAWDATA * rawData,int tableNum,DATA_LIST * selectedVarList){
	//target the table with tableNum
	DATABASE::iterator currentTable;
	
	currentTable = database.begin();
	advance(currentTable,tableNum);
	SIZE tableSize = currentTable->getSize();
	
	if(rawData==NULL){
		
		//create the rawData
		RAWDATA newRaw;
		rawData = &newRaw;

		for(int i =0;i< selectedVarList->size();i++){
			DATA_LIST newList;
			rawData->push_back(newList);
		}
		
		//extract the data of selectedVars
		for(int i = 0;i<tableSize;i++){
			
			DATA_LIST::iterator selectedVar;
			int varNum =0;
			for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
				int currentColNum = currentTable->getColNumOf(*selectedVar);
				int currentEntry = currentTable->getEntryAt(i,currentColNum);
				
				rawData[varNum].push_back(currentEntry);
				varNum++;
			}
		}
	}else{
		//merge the new data into existing rawData
		
		//expand the rawList
		RAWDATA tempRaw;
		SIZE newSize = rawData->size()+selectedVarList->size();
		for(int i =0;i< newSize ;i++){
			DATA_LIST newList;
			tempRaw.push_back(newList);
		}

		//merge entries 
		SIZE rawRowNum = rawData[0].size();
		SIZE rawColNum = rawData->size();
		
		for(int i =0;i<rawRowNum;i++){
			for(int j =0;j<tableSize;i++){
				//first step: copy the old qrVar data
				for(int k=0;k<rawColNum;k++){
					tempRaw[k].push_back(rawData[i][k]);
				}
				//second step: append new qrVar data
				DATA_LIST::iterator selectedVar;
				int count =0;
				for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
					int currentColNum = currentTable->getColNumOf(*selectedVar);
					int currentEntry = currentTable->getEntryAt(i,currentColNum);
					
					tempRaw[rawColNum+count].push_back(currentEntry);
					count++;
				}
			}
		}
		rawData = &tempRaw;
	}
	return rawData;
}

RAWDATA * IntermediateResultTable::joinCombinations(RAWDATA* rawData, DATA_LIST * selectedVarList){
	if(rawData==NULL){	
		//create the rawData
		RAWDATA newRaw;
		rawData = &newRaw;

		//create first col
		TYPE resultVarType = qrTable->find(selectedVarList->at(0))->second;
		rawData->push_back(*IntermediateResultTable::getStmtListOf(resultVarType));
	}else{
		//merge the new data into existing rawData
		
		//expand the rawList by one col
		RAWDATA tempRaw;
		DATA_LIST newList;
		tempRaw.push_back(newList);

		//merge entries 
		SIZE rawRowNum = rawData[0].size();
		SIZE rawColNum = rawData->size();

		TYPE resultVarType = qrTable->find(selectedVarList->at(0))->second;
		DATA_LIST * selectedDataList;
		selectedDataList = IntermediateResultTable::getStmtListOf(resultVarType);
		
		for(int i =0;i<rawRowNum;i++){
			for(int j =0;j<selectedDataList->size();i++){
				//first step: copy the old qrVar data
				for(int k=0;k<rawColNum;k++){
					tempRaw[k].push_back(rawData[i][k]);
				}
				//second step: append new qrVar data
				tempRaw[rawColNum+1].push_back(selectedDataList->at(j));					
			}
		}
		rawData = &tempRaw;
	}
	DATA_LIST::iterator itr;
	//remove current selected var
	itr = selectedVarList->begin();
	selectedVarList->erase(itr);

	if(itr==selectedVarList->end()){
		return rawData;
	}
	rawData = IntermediateResultTable::joinCombinations(rawData,selectedVarList);
	return rawData;
}

DATA_LIST * IntermediateResultTable::getStmtListOf(TYPE nodeType){
	DesignExtractor extractor(pkb);
	DATA_LIST * returnList;
	returnList = new DATA_LIST;

	int maxStmtNum = pkb->getMaxStatementNum();
	int maxProgLineNum = pkb->getMaxProgLine();

	switch(nodeType){
	case PROGLINE:
		for(int i=1; i<=maxProgLineNum;i++){
			returnList->push_back(i);
		}
		break;
	case STATEMENT:
		for(int i=1; i<=maxStmtNum;i++){
			returnList->push_back(i);
		}
		break;
	case ASSIGNMENT:
		returnList = extractor.getAllAssigns();
		break;
	case WHILE:
		returnList = extractor.getAllWhiles();
		break;
	case IF:
		returnList = extractor.getAllIfs();
		break;
	case CALL:
		returnList = extractor.getAllCallStmts();
		break;
	case VARIABLE:
		{
			int varSize = pkb->getVarTableSize();
			for(int i =1;i<=varSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	case PROCEDURE:
		{
			int procSize = pkb->getProceTableSize();
			for(int i =1;i<=procSize;i++){
				returnList->push_back(i);
			}
		}
		break;
	}

	return returnList;
}


		