#include "LinkedDataTable.h"


LinkedDataTable::LinkedDataTable(void)
{

}


LinkedDataTable::~LinkedDataTable(void)
{

}

bool LinkedDataTable::addEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries){
	if(mergeAttrPos==0){
		LinkedDataTable::createEntry(oldQrVar,newQrVar,newEntries);
	}else{
		return LinkedDataTable::addOneEntry(mergeAttrPos,oldQrVar,newQrVar,newEntries);
	}
	return true;
}
hash_map<int,DATA_LIST> * LinkedDataTable::convertToHash(int mergeAttrPos,RELATION_LIST * newEntries){
	hash_map<int,DATA_LIST> * returnResult = new hash_map<int,DATA_LIST>;
	hash_map<int,DATA_LIST>::iterator mapItr;
	RELATION_LIST::iterator lstItr;

	if(mergeAttrPos==1){
		for(lstItr = newEntries->begin();lstItr!=newEntries->end();lstItr++){
			mapItr = returnResult->find(lstItr->first);

			if(mapItr!=returnResult->end()){ //key exists,append the data to current key entry
				mapItr->second.push_back(lstItr->second);
			}else{	//key does not exist, create a new vector list
				DATA_LIST newList;
				newList.push_back(lstItr->second);
				(*returnResult)[lstItr->first] = newList;
			}
		}
	}else{
		for(lstItr = newEntries->begin();lstItr!=newEntries->end();lstItr++){
			mapItr = returnResult->find(lstItr->second);

			if(mapItr!=returnResult->end()){ //key exists,append the data to current key entry
				mapItr->second.push_back(lstItr->first);
			}else{	//key does not exist, create a new vector list
				DATA_LIST newList;
				newList.push_back(lstItr->first);
				(*returnResult)[lstItr->second] = newList;
			}
		}
	}

	return returnResult;
}
bool LinkedDataTable::addOneEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries){
	//create new column
	if(newQrVar!=-1)
		qrVarList.push_back(newQrVar);

	//create a temporary row list
	ROW_LIST * tempRowList = new ROW_LIST;
	INDEX oldColNum = LinkedDataTable::getColNumOf(oldQrVar);
	
	//convert the newEntries to hashmap
	hash_map<int,DATA_LIST> * entryMap = LinkedDataTable::convertToHash(mergeAttrPos,newEntries);
	hash_map<int,DATA_LIST>::iterator mapItr;

	for(int i=0;i<rowList.size();i++){	
		mapItr = entryMap->find(rowList[i][oldColNum]);

		if(mapItr!=entryMap->end()){  //the entry in database is also in incoming list
			//expand the database
			DATA_LIST::iterator newListItr;
			DATA_LIST newList = mapItr->second;
			//loop through the entryList with the same firstData
			for(newListItr = newList.begin();newListItr!=newList.end();newListItr++){
				ROW tempRow = rowList[i];
				tempRow.push_back(*newListItr);
				tempRowList->push_back(tempRow);
			}
		}
	}
	delete entryMap;
	
	//replace the old row list
	if(tempRowList==NULL||tempRowList->size()<1){
		delete tempRowList;
		return false;
	}
	rowList = *tempRowList;
	delete tempRowList;
	return true;
}
bool LinkedDataTable::createEntry(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newEntries){
	//create 2 new columns
	if(firstQrVar !=-1){
		qrVarList.push_back(firstQrVar);
	}
	if(secondQrVar !=-1){
		qrVarList.push_back(secondQrVar);
	}

	RELATION_LIST::iterator currentEntry;
	for(currentEntry = newEntries->begin();currentEntry!=newEntries->end();currentEntry++){
		ROW newRow;

		if(firstQrVar !=-1)newRow.push_back(currentEntry->first);
		if(secondQrVar != -1)newRow.push_back(currentEntry->second);
		
		rowList.push_back(newRow);
	}
	return true;
}
bool LinkedDataTable::mergeEntries(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * mergeList){ //both vars are in the database
	//create a temporary row list
	ROW_LIST * tempRowList = new ROW_LIST;
	INDEX firstColNum = LinkedDataTable::getColNumOf(firstQrVar);
	INDEX secondColNum = LinkedDataTable::getColNumOf(secondQrVar);
	
	//convert the newEntries to hashmap
	hash_map<int,DATA_LIST> * entryMap1 = LinkedDataTable::convertToHash(1,mergeList);
	hash_map<int,DATA_LIST>::iterator mapItr1;

	for(int i=0;i<rowList.size();i++){	
		ROW curRow= rowList[i];
		mapItr1= entryMap1->find(curRow[firstColNum]);

		if(mapItr1!=entryMap1->end()){ //first data is matched
			//check if second field data is matched
			DATA_LIST hashedList = mapItr1->second;
			if(find(hashedList.begin(),hashedList.end(),curRow[secondColNum])!=hashedList.end()){
				tempRowList->push_back(curRow);	
			}				
		}
	}
	delete entryMap1;

	//replace the old row list
	if(tempRowList->size()>0){
		rowList = *tempRowList;
		delete tempRowList;
		return true;
	}
	delete tempRowList;
	return false;
}
bool LinkedDataTable::mergeTable(INDEX firstQrVar,INDEX secondQrVar,LinkedDataTable * newTable){
	//create a temporary row list
	ROW_LIST * tempRowList = new ROW_LIST;
	//find the location of the two qrVar in two tables
	INDEX firstColNum1 = LinkedDataTable::getColNumOf(firstQrVar);
	INDEX secondColNum1 = LinkedDataTable::getColNumOf(secondQrVar);

	INDEX firstColNum2 = newTable->getColNumOf(firstQrVar);
	INDEX secondColNum2 = newTable->getColNumOf(secondQrVar);

	//STEP1: convert the new Table to hash table
	hash_map<string,ROW_LIST>  hashedTable; 
	hash_map<string,ROW_LIST>::iterator mapItr;

	for(int i=0;i<newTable->getSize();i++){
		ROW currentRow = newTable->getRow(i);
		//construct the key
		string qVarValue1 = static_cast<ostringstream*>( &(ostringstream() << currentRow.at(firstColNum2)))->str();
		string qVarValue2 = static_cast<ostringstream*>( &(ostringstream() << currentRow.at(secondColNum2)))->str();
		string key=qVarValue1+","+qVarValue2;
		
		//find if the key exists
		mapItr = hashedTable.find(key);

		if(mapItr!=hashedTable.end()){ //key exists,append the row to the rowList
				mapItr->second.push_back(currentRow);
			}else{	//key does not exist, create a new row list
				ROW_LIST newList;
				newList.push_back(currentRow);
				hashedTable[key] = newList;
			}
	}

	//STEP 2:marge two tables, and store the merged table into current LinkedDataTable
	for(int i=0;i<rowList.size();i++){	
		ROW curRow= rowList[i];
		//build the key
		string qVarValue1 = static_cast<ostringstream*>( &(ostringstream() << curRow.at(firstColNum1)))->str();
		string qVarValue2 = static_cast<ostringstream*>( &(ostringstream() << curRow.at(secondColNum1)))->str();
		string key=qVarValue1+","+qVarValue2;

		//find if the key exists in the hashed table
		mapItr= hashedTable.find(key);

		if(mapItr!=hashedTable.end()){ //the key is found append, merge the entries
			ROW_LIST listTobeMerged = mapItr->second;
	
			//loop through the list to be merged and expand the current database
			for(int k =0;k<listTobeMerged.size();k++){
				ROW tempRow = rowList[i]; //the row in the current database
				ROW curList = listTobeMerged.at(k); // the list in the hashed table
				//append the mached entry one by one
				for(int j=0;j<curList.size();j++){
					if(j!=firstColNum2&&j!=secondColNum2)
						tempRow.push_back(curList.at(j));
				}
				//push the merged entry into a temp row  list
				tempRowList->push_back(tempRow);
			}
							
		}
	}
	//STEP 3:after merging, replace the old row list and update the qrVarList
	if(tempRowList->size()>0){
		rowList = *tempRowList;
		delete tempRowList;

		//update the qrVarList
		ROW mergeQrVarList = newTable->getQrVarList();
		for(int i =0;i<mergeQrVarList.size();i++){
			if(i!=firstColNum2&&i!=secondColNum2)
				qrVarList.push_back(mergeQrVarList.at(i));
		}
		return true;
	}
	delete tempRowList;
	return false;

/*
	ROW_LIST::iterator currentRow1;
	ROW_LIST::iterator endRow = rowList.end();
	currentRow1 = rowList.begin();
	
	bool newVarAdded = false;
	//run until reach the last row
	while(currentRow1!=endRow){
		//get the value of the two old entries in the table
		int firstEntry1 = currentRow1->at(firstColNum1);
		int secondEntry1 = currentRow1->at(secondColNum1);

		//compare the oldEntry and newEntry value
		SIZE newTableSize = newTable->getSize();
		for(int i= 0;i<newTableSize;i++){
			ROW currentRow2  = newTable->getRow(i);
			currentEntry = currentRow2.begin();
			advance(currentEntry,firstColNum2);
			int firstEntry2 = *currentEntry;

			currentEntry = currentRow2.begin();
			advance(currentEntry,secondColNum2);
			int secondEntry2 = *currentEntry;
			
			//matched entry is found, add to temp table
			if(firstEntry1==firstEntry2&&secondEntry1==secondEntry2){
				if(!newVarAdded){
					newVarAdded = true;
					//add new qrVar
					ROW qrVarList2 = newTable->getQrVarList();
					ROW::iterator currentQrVar;
					for(currentQrVar = qrVarList2.begin();currentQrVar!=qrVarList2.end();currentQrVar++){
						if(*currentQrVar!=firstQrVar&&*currentQrVar!=secondQrVar){
							qrVarList.push_back(*currentQrVar);
						}
					}
				}
				//append the data from new table in the row in current table
				ROW tempRow = *currentRow1;
				SIZE rowSize2 = currentRow2.size();
				currentEntry = currentRow2.begin();

				for(int i =0;i<rowSize2;i++){
					if(i!=firstColNum2&&i!=secondColNum2){
						tempRow.push_back(*currentEntry);
					}
					currentEntry++;
				}
				//add the updated temp row into temp row list
				tempRowList->push_back(tempRow);
			}
		}
		currentRow1++;
	}
	//replace the old row list
	if(tempRowList->size()>0){
		rowList = *tempRowList;
		delete tempRowList;
		return true;
	}
	delete tempRowList;
	return false;*/
	
}
void LinkedDataTable::deleteRowBy(INDEX qrVar,int data){
	/*INDEX colNum = LinkedDataTable::getColNumOf(qrVar);
	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	
	//run until reach the last row
	while(currentRow!=rowList.end()){
		ROW::iterator currentEntry;
		currentEntry = currentRow->begin();
		advance(currentEntry,colNum);

		//check if the entry[row][col] is same as the data
		if(*currentEntry==data){
			currentRow = rowList.erase(currentRow);
		}else{
			currentRow++;
		}
	}*/
	
}
INDEX_LIST * LinkedDataTable::getDataListOf(INDEX qrVar){
	INDEX_LIST * returnList = new INDEX_LIST;
	INDEX colNum = LinkedDataTable::getColNumOf(qrVar);
//	SIZE rowNum = rowList.size();

	ROW_LIST::iterator currentRow;
	//loop through the row list 
	for(int i =0;i<rowList.size();i++){
		returnList->push_back(rowList[i][colNum]);
	}
	/*for(currentRow = rowList.begin();currentRow!=rowList.end();currentRow++){
		ROW::iterator currentEntry;
		//locate the position of entry[row][col]
		currentEntry = currentRow->begin();
		advance(currentEntry,colNum);
		//add the value in return list
		returnList->push_back(*currentEntry);
	}*/
	
	returnList->sort();
	returnList->unique();

	return returnList;
	
}
int LinkedDataTable::getColNumOf(INDEX qrVar){
	SIZE tableSize = qrVarList.size();
	ROW::iterator itr;
	int count =0;
	//count the col number where qrVar locates
	for(itr = qrVarList.begin();itr!=qrVarList.end();itr++){
		if(*itr == qrVar) return count;
		count++;
	}
	return -1;
}
bool LinkedDataTable::isExists(INDEX qrVar){
	ROW::iterator itr;
	//find if the qrVar exists in current table
	itr= find(qrVarList.begin(),qrVarList.end(),qrVar);
	if(itr!=qrVarList.end()) return true;

	return false;
}
ROW LinkedDataTable::getQrVarList(){
	return qrVarList;
}
SIZE LinkedDataTable::getSize(){
	return rowList.size();
}
ROW LinkedDataTable::getRow(INDEX rowNum){

	return rowList[rowNum];/*
	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	advance(currentRow, rowNum);

	return *currentRow;*/
}
int LinkedDataTable::getEntryAt(INDEX row,INDEX col){
	return rowList[row][col];
	/*
	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	advance(currentRow,row);

	ROW::iterator currentEntry;
	currentEntry = currentRow->begin();
	advance(currentEntry,col);

	return *currentEntry;
	*/
}