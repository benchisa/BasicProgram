#include "IntermediateResultTable.h"


IntermediateResultTable::IntermediateResultTable(SIZEX tableSize,PKB* pkb,QUERYTABLE* qrTable,DesignExtractor * extractor)
{
	this->tableSize = tableSize;
	this->pkb = pkb;
	this->qrTable = qrTable;
	this->extractor = extractor;
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
			return currentTable->getDataListOf(qrVarIndex);
		}
	}
	return NULL;
}
bool IntermediateResultTable::joinList(JOIN_ATTR qrVarIndex,INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newList){
	//if either qrVar has index value -1, dont merge the value into the table	
	if(firstQrVar!=-1&&secondQrVar==-1){
		//only merge the first one
		//case 1: firstQrVar is not in the table
		if(qrVarTable[0][firstQrVar]!=1){
			//create a new table to store new list
			LinkedDataTable newTable;
			//add new list to new table
			if(!newTable.addEntry(0,firstQrVar,-1,newList)) return false;
			//add the new table to database
			database.push_back(newTable);

			//update the information of the new added qrVars
			qrVarTable[0][firstQrVar] =1;
			qrVarTable[1][firstQrVar] = database.size()-1;
		}else{
		//case 2: firstQrVar is in the table
			//get the table contains first qrVar
			int tableNum = qrVarTable[1][firstQrVar];
			DATABASE::iterator currentTable;
			currentTable = database.begin();
			advance(currentTable,tableNum);
			//merge the new list into table
			if(!currentTable->addEntry(1,firstQrVar,-1,newList))return false;
		}
	}else if(firstQrVar==-1&&secondQrVar!=-1){
		//only merge the second one
		//case 1: secondQrVar is not in the table
		if(qrVarTable[0][secondQrVar]!=1){
			//create a new table to store new list
			LinkedDataTable newTable;
			//add new list to new table
			if(!newTable.addEntry(0,-1,secondQrVar,newList))return false;
			//add the new table to database
			database.push_back(newTable);

			//update the information of the new added qrVars
			qrVarTable[0][secondQrVar] =1;
			qrVarTable[1][secondQrVar] = database.size()-1;
		}else{
		//case 2: secondQrVar is in the table
			//get the table contains second qrVar
			int tableNum = qrVarTable[1][secondQrVar];
			DATABASE::iterator currentTable;
			currentTable = database.begin();
			advance(currentTable,tableNum);
			//merge the new list into table
			if(!currentTable->addEntry(2,secondQrVar,-1,newList)) return false;
		}
	}else if(firstQrVar==-1 && secondQrVar==-1){
		//don't merge
		return true;
	}else{

	//case 1: both qrVar are not in the table, create a new table
	if(qrVarTable[0][firstQrVar]!=1&&qrVarTable[0][secondQrVar]!=1){
		//create a new table to store new list
		LinkedDataTable newTable;
		//add new list to new table
		if(!newTable.addEntry(0,firstQrVar,secondQrVar,newList))return false;
		//add the new table to database
		database.push_back(newTable);

		//update the information of the new added qrVars
		qrVarTable[0][firstQrVar] =1;
		qrVarTable[1][firstQrVar] = database.size()-1;
		qrVarTable[0][secondQrVar] =1;
		qrVarTable[1][secondQrVar] = database.size()-1;
	}else
	//case 2:only one qrVar is in the table, merge into the existing table
	//only first qrVar exist
	if(qrVarTable[0][firstQrVar]==1&&qrVarTable[0][secondQrVar]!=1){
		//merge the new list in the existing table
		
		//get the table contains first qrVar
		int tableNum = qrVarTable[1][firstQrVar];
		DATABASE::iterator currentTable;
		currentTable = database.begin();
		advance(currentTable,tableNum);
		//merge the new list into table
		if(!currentTable->addEntry(1,firstQrVar,secondQrVar,newList))return false;
		//update the information of the second qrVar
		qrVarTable[0][secondQrVar] = 1;
		qrVarTable[1][secondQrVar] = qrVarTable[1][firstQrVar];
	}else
	//only second qrVar exist
	if(qrVarTable[0][firstQrVar]!=1&&qrVarTable[0][secondQrVar]==1){
		//merge the new list in the existing table
		
		//get the table contains first qrVar
		int tableNum = qrVarTable[1][secondQrVar];
		DATABASE::iterator currentTable;
		currentTable = database.begin();
		advance(currentTable,tableNum);
		//merge the new list into table
		if(!currentTable->addEntry(2,secondQrVar,firstQrVar,newList))return false;
		//update the information of the second qrVar
		qrVarTable[0][firstQrVar] = 1;
		qrVarTable[1][firstQrVar] = qrVarTable[1][secondQrVar];
	}else
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

			//merge the new list to second table
			int tableNum2 = qrVarTable[1][secondQrVar];
			table2 = database.begin();
			advance(table2,tableNum2);

			//merge two tables
				
			LinkedDataTable  table2Pointer;
			table2Pointer = *table2;
			if(!table1->mergeTable(firstQrVar,secondQrVar,&table2Pointer,newList))return false;

			//update qrVar info (table num)
			for(int i =0;i<tableSize;i++){
				int curTableNum = qrVarTable[1][i];

				if(tableNum1<tableNum2){
					if(curTableNum==tableNum2){
						qrVarTable[1][i]= tableNum1;
					}else if(curTableNum>tableNum2){
						qrVarTable[1][i] = curTableNum-1;
					}
				}else if(tableNum1>tableNum2){
					if(curTableNum==tableNum1){
						qrVarTable[1][i]= tableNum2;
					}
					if(curTableNum>tableNum2){
						qrVarTable[1][i] = curTableNum-1;
					}
				}
			}

			//after merging two tables,delete the second table
			database.erase(table2);
			
		}
		
	}
	}
	return true;
}
FINALRAW *IntermediateResultTable::findResultOf(DATA_LIST resultNodeList){
	RAWDATA * returnRaw;
	returnRaw = NULL;
	GROUP nodeGroup;
	DATA_LIST keyTable;
	bool repeatedEntity = false;

	//group the qrVar nodes order according to their database number
	DATA_LIST::iterator nodeListItr;

	int i =0;
	for(nodeListItr = resultNodeList.begin();nodeListItr!=resultNodeList.end();nodeListItr++){
		//add the key_value pair into group, key = databaseNum
		int key = qrVarTable[1][*nodeListItr];
		if(key<0){ //special key for qrVars that didn't appear in any clauses
			key =-1;
		}
		nodeGroup.insert(valuePair(key,*nodeListItr));
		
		//store the new key into the keyTable
		DATA_LIST::iterator keyItr;
		keyItr = find(keyTable.begin(),keyTable.end(),key);
		if(keyItr==keyTable.end()){
			keyTable.push_back(key);
		}
	}

	//extract the qrVar names in one table, and merge them into the final returnRaw
	for(int i=0;i<keyTable.size();i++){
		pair<GROUP::iterator, GROUP::iterator> keyValueList;
		keyValueList = nodeGroup.equal_range(keyTable[i]);

		DATA_LIST selectedVarList;

		for(GROUP::iterator keyValueItr=keyValueList.first;keyValueItr!=keyValueList.second;keyValueItr++){
			int qrVar =keyValueItr->second;
			selectedVarList.push_back(qrVar);
			
		}
		//add into retrunRaw 
		if(keyTable[i]==-1){
			returnRaw = IntermediateResultTable::joinCombinations(returnRaw,&selectedVarList);
		}else{
			returnRaw = IntermediateResultTable::joinRaw(returnRaw,keyTable[i],&selectedVarList);
		}
	}

	//adjust the order of returnRaw
	RAWDATA * tempRaw = new RAWDATA();
	for(nodeListItr = resultNodeList.begin();nodeListItr!=resultNodeList.end();nodeListItr++){
		for(RAWDATA::iterator rawItr=returnRaw->begin();rawItr!=returnRaw->end();rawItr++){
			if(*nodeListItr==rawItr->at(0)){
				//add to temp return List
				tempRaw->push_back(*rawItr);
				//remove current added entry to avoid duplicate result
				returnRaw->erase(rawItr);
				break;
			}
		}
	}
	//returnRaw = tempRaw;

	//unique the return result
	FINALRAW * finalRaw = IntermediateResultTable::uniqueRaw(tempRaw);
	delete returnRaw, tempRaw;

	return finalRaw;
	
	
}
FINALRAW * IntermediateResultTable::uniqueRaw(RAWDATA * currentRaw){
	FINALRAW * finalRaw = new FINALRAW;
	FINALRAW::iterator finalRawItr;
	int rowSize = currentRaw->at(0).size();
	for(int row =1; row<rowSize; row++){
		string key="";
		DATA_LIST value;
		for(int col=0;col<currentRaw->size();col++){
			//create the final raw key
			//create the final raw value
			int currentData = currentRaw->at(col).at(row);
			key.append(static_cast<ostringstream*>( &(ostringstream() << currentData))->str());
			key.append(",");
			value.push_back(currentData);
		}
		//check unique of current entry
		finalRawItr = finalRaw->find(key);

		if(finalRawItr==finalRaw->end()){
			(*finalRaw)[key]=value; //add to final raw
		}
	}
	
	return finalRaw;

}

RAWDATA * IntermediateResultTable::simplifyRaw(RAWDATA * currentRaw){
	FINALRAW * finalRaw = new FINALRAW;
	FINALRAW::iterator finalRawItr;
	int rowSize = currentRaw->at(0).size();
	int colSize= currentRaw->size();

	//get qrVarList
	DATA_LIST qrVarList;
	for(int i=0;i<colSize;i++){
		qrVarList.push_back(currentRaw->at(i).at(0));
	}
	for(int row =1; row<rowSize; row++){
		string key="";
		DATA_LIST value;
		for(int col=0;col<currentRaw->size();col++){
			//create the final raw key
			//create the final raw value
			int currentData = currentRaw->at(col).at(row);
			key.append(static_cast<ostringstream*>( &(ostringstream() << currentData))->str());
			key.append(",");
			value.push_back(currentData);
		}
		//check unique of current entry
		finalRawItr = finalRaw->find(key);

		if(finalRawItr==finalRaw->end()){
			(*finalRaw)[key]=value; //add to final raw
		}
	}
	delete currentRaw;
	//convert it back to raw data structure
	RAWDATA * returnRaw = new RAWDATA();
	int newRowSize = finalRaw->size();

	for(int i =0;i<colSize;i++){
		ROW newRow;
		newRow.push_back(qrVarList.at(i));
		returnRaw->push_back(newRow);
	}
	//put the itr at the second row
	finalRawItr = finalRaw->begin();

	for(int i =0;i<newRowSize;i++){
		for(int j=0;j<colSize;j++){
			returnRaw->at(j).push_back(finalRawItr->second.at(j));
		}
		finalRawItr++;
	}

	delete finalRaw;
	return returnRaw;

}
RAWDATA * IntermediateResultTable::joinRaw(RAWDATA * rawData,int tableNum,DATA_LIST * selectedVarList){
	//target the table with tableNum
	DATABASE::iterator currentTable;
	DATA_LIST::iterator selectedVar;

	currentTable = database.begin();
	advance(currentTable,tableNum);
	SIZEX tableSize = currentTable->getSize();
	
	if(rawData==NULL){
		
		//create the rawData
		rawData = new RAWDATA();
		for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
			DATA_LIST newList;
			newList.push_back(*selectedVar);
			rawData->push_back(newList);
		}
		
		//extract the data of selectedVars
		for(int i = 0;i<tableSize;i++){
			int varNum =0;
			for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
				int currentColNum = currentTable->getColNumOf(*selectedVar);
				int currentEntry = currentTable->getEntryAt(i,currentColNum);
				
				rawData->at(varNum).push_back(currentEntry);
				varNum++;
			}
		}

		//unique result
		if(selectedVarList->size()!=currentTable->getQrVarList().size())
			rawData = IntermediateResultTable::simplifyRaw(rawData);
	}else{
		//merge the new data into existing rawData
		
		//expand the rawList
		RAWDATA tempRaw;
		RAWDATA * simplifyRaw = new RAWDATA();

		//copy the old col
		for(int i =0;i<rawData->size();i++){
			DATA_LIST newList;
			newList.push_back(rawData->at(i).at(0));
			tempRaw.push_back(newList);
		}
		//create new col
		for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
			DATA_LIST newList;
			newList.push_back(*selectedVar);
			tempRaw.push_back(newList);
			simplifyRaw->push_back(newList);
		}

		//extract the data of selected qrVar
		for(int j =0;j<tableSize;j++){
			DATA_LIST::iterator selectedVar;
			int count =0;
			for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
				int currentColNum = currentTable->getColNumOf(*selectedVar);
				simplifyRaw->at(count).push_back(currentTable->getEntryAt(j,currentColNum));
				count++;
			}

		}
		//unique result
		simplifyRaw = IntermediateResultTable::simplifyRaw(simplifyRaw);

		//merge entries 
		SIZEX rawRowNum = rawData->at(0).size()-1;
		SIZEX rawColNum = rawData->size();
	
		for(int i =1;i<=rawRowNum;i++){
			for(int j =1;j<simplifyRaw->at(0).size();j++){
				//first step: copy the old qrVar data
				for(int k=0;k<rawColNum;k++){
					tempRaw[k].push_back(rawData->at(k).at(i));
				}
				//second step: append new qrVar data
			
				for(int k =0;k<simplifyRaw->size();k++){
					int  currentEntry = simplifyRaw->at(k).at(j);
					tempRaw[rawColNum+k].push_back(currentEntry);
				}
			}
		}
		
		/*for(int i =1;i<=rawRowNum;i++){
			for(int j =0;j<tableSize;j++){
				//first step: copy the old qrVar data
				for(int k=0;k<rawColNum;k++){
					tempRaw[k].push_back(rawData->at(k).at(i));
				}
				//second step: append new qrVar data
				DATA_LIST::iterator selectedVar;
				int count =0;
				for(selectedVar = selectedVarList->begin();selectedVar!=selectedVarList->end();selectedVar++){
					int currentColNum = currentTable->getColNumOf(*selectedVar);
					int currentEntry = currentTable->getEntryAt(j,currentColNum);
					
					tempRaw[rawColNum+count].push_back(currentEntry);
					count++;
				}
			}
		}*/
		rawData = new RAWDATA(tempRaw);
	}
	
	return rawData;
}
RAWDATA * IntermediateResultTable::joinCombinations(RAWDATA* rawData, DATA_LIST * selectedVarList){
	if(rawData==NULL){	
		//create the rawData
		rawData = new RAWDATA();

		//create first col
		TYPE resultVarType = qrTable->find(selectedVarList->at(0))->second;
		rawData->push_back(*extractor->getStmtListOf(resultVarType));
		
		//add the col name, which is the qrVar index
		DATA_LIST::iterator itr;
		itr = rawData->at(0).begin();
		rawData->at(0).insert(itr,selectedVarList->at(0));
	}else{
		//merge the new data into existing rawData
		
		//expand the rawList by one col
		RAWDATA tempRaw;
		//first step:copy the old col
		for(int i =0;i<rawData->size();i++){
			DATA_LIST newList;
			newList.push_back(rawData->at(i).at(0));
			tempRaw.push_back(newList);
		}
		//create one new col
		DATA_LIST newList;
		newList.push_back(selectedVarList->at(0));
		tempRaw.push_back(newList);
		

		//merge entries 
		SIZEX rawRowNum = rawData->at(0).size()-1; //number of data sets
		SIZEX rawColNum = rawData->size(); //number of qrVars in raw

		TYPE resultVarType = qrTable->find(selectedVarList->at(0))->second;
		DATA_LIST * selectedDataList;
		selectedDataList = extractor->getStmtListOf(resultVarType);
		
		for(int i =1;i<=rawRowNum;i++){
			for(int j =0;j<selectedDataList->size();j++){
				//first step: copy the old qrVar data
				for(int k=0;k<rawColNum;k++){
					tempRaw[k].push_back(rawData->at(k).at(i));
				}
				//second step: append new qrVar data
				tempRaw[rawColNum].push_back(selectedDataList->at(j));					
			}
		}
		rawData = new RAWDATA(tempRaw);
	}
	DATA_LIST::iterator itr;
	//remove current selected var
	itr = selectedVarList->begin();
	selectedVarList->erase(itr);

	if(selectedVarList->size()<1){
		return rawData;
	}
	rawData = IntermediateResultTable::joinCombinations(rawData,selectedVarList);
	return rawData;
}



		