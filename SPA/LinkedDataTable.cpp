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
	RELATION_LIST::iterator endItr = newEntries->end();
	if(mergeAttrPos==1){
		for(lstItr = newEntries->begin();lstItr!=endItr;lstItr++){
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
		for(lstItr = newEntries->begin();lstItr!=endItr;lstItr++){
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
	
	//------------------STEP1:convert the newEntries to hashmap---------------------------
	hash_map<int,DATA_LIST> * entryMap = LinkedDataTable::convertToHash(mergeAttrPos,newEntries);
	hash_map<int,DATA_LIST>::iterator mapItr;

	//------------------STEP2:Merge entries---------------------------
	for(int i=0;i<rowList.size();i++){	
		mapItr = entryMap->find(rowList[i][oldColNum]);

		if(mapItr!=entryMap->end()){  //the entry in database is also in incoming list
			//expand the database
			DATA_LIST newList = mapItr->second;
			DATA_LIST::iterator newListItr;
			DATA_LIST::iterator endItr = newList.end();
			//loop through the entryList with the same firstData
			for(newListItr = newList.begin();newListItr!=endItr;newListItr++){
				ROW tempRow = rowList[i];
				tempRow.push_back(*newListItr);
				tempRowList->push_back(tempRow);
			}
		}
	}
	delete entryMap;
	
	////------------------STEP1:replace the old row list---------------------------
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
	RELATION_LIST::iterator endItr = newEntries->end();
	for(currentEntry = newEntries->begin();currentEntry!=endItr;currentEntry++){
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
	
	//----------------------------STEP1:convert the newEntries to hashmap-----------------------------
	hash_map<int,DATA_LIST> * entryMap1 = LinkedDataTable::convertToHash(1,mergeList);
	hash_map<int,DATA_LIST>::iterator mapItr1;
	//----------------------------STEP2: MERGE entries-----------------------------
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

	//----------------------------STEP3:replace the old row list-----------------------------
	if(tempRowList->size()>0){
		rowList = *tempRowList;
		delete tempRowList;
		return true;
	}
	delete tempRowList;
	return false;
}
bool LinkedDataTable::mergeTable(INDEX firstQrVar,INDEX secondQrVar,LinkedDataTable * newTable,RELATION_LIST* newEntries){
	//create a temporary row list
	ROW_LIST * tempRowList = new ROW_LIST;

	//find the location of the two qrVar in two tables
	INDEX firstColNum1 = LinkedDataTable::getColNumOf(firstQrVar); //only in current table
	INDEX secondColNum2 = newTable->getColNumOf(secondQrVar);//only in the newTable

	//----------------------STEP1a: convert the new Table to hash table---------------------------------------
	hash_map<int,ROW_LIST>  hashedTable; 
	hash_map<int,ROW_LIST>::iterator hashTableItr;
	

	for(int i=0;i<newTable->getSize();i++){
		ROW currentRow = newTable->getRow(i);
		//construct the key
		int key =  currentRow.at(secondColNum2);
		
		//find if the key exists
		hashTableItr = hashedTable.find(key);

		if(hashTableItr!=hashedTable.end()){ //key exists,append the row to the rowList
				hashTableItr->second.push_back(currentRow);
			}else{	//key does not exist, create a new row list
				ROW_LIST newList;
				newList.push_back(currentRow);
				hashedTable[key] = newList;
			}
	}
	//------------------------STEP 1b: convert the newEntires to a hash table------------------------------
	hash_map<int,DATA_LIST> * entryMap = LinkedDataTable::convertToHash(1,newEntries);
	hash_map<int,DATA_LIST>::iterator entryMapItr;
	hash_map<int,DATA_LIST>::iterator endEntryMap = entryMap->end();
	hash_map<int,ROW_LIST>::iterator endHashTable = hashedTable.end();
	//------------------------STEP 2:marge two tables, and store the merged table into current LinkedDataTable-----------------	
	//------------the newEntries and newTable are hashed
	for(int i=0;i<rowList.size();i++){	
		ROW curRow= rowList[i];
		
		//build the key
		int key1 =  curRow.at(firstColNum1);
		
		//find if the first key exists in the hashed entry
		entryMapItr= entryMap->find(key1);

		if(entryMapItr!=endEntryMap){ //the first key is found in entry map
			DATA_LIST secondKeyList= entryMapItr->second;
			DATA_LIST::iterator key2Itr;

			//find if the second key is in the hashed table
			for(key2Itr=secondKeyList.begin();key2Itr!=secondKeyList.end();key2Itr++){
				hashTableItr = hashedTable.find(*key2Itr);

				if(hashTableItr!=endHashTable){ //second key is found, merge entry
					ROW_LIST listTobeMerged = hashTableItr->second;

					//loop through the list to be merged and expand the current database
					for(int k =0;k<listTobeMerged.size();k++){
						ROW tempRow = rowList[i]; //the row in the current database
						ROW curList = listTobeMerged.at(k); // the list in the hashed table
						//append the mached entry one by one
						for(int j=0;j<curList.size();j++){
							if(j!=secondColNum2) tempRow.push_back(curList.at(j));
						}
						//push the merged entry into a temp row  list
						tempRowList->push_back(tempRow);
					}
				}
			}						
		}
	}
	delete entryMap;
	//------------------STEP 3:after merging, replace the old row list and update the qrVarList------------------------
	if(tempRowList->size()>0){
		rowList = *tempRowList;
		delete tempRowList;

		//update the qrVarList
		ROW mergeQrVarList = newTable->getQrVarList();
		for(int i =0;i<mergeQrVarList.size();i++){
			if(i!=secondColNum2)
				qrVarList.push_back(mergeQrVarList.at(i));
		}
		return true;
	}

	//failed to merge two tables
	delete tempRowList;
	return false;

	
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

	ROW_LIST::iterator currentRow;

	//loop through the row list 
	for(int i =0;i<rowList.size();i++){
		returnList->push_back(rowList[i][colNum]);
	}
	
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

	return rowList[rowNum];
}
int LinkedDataTable::getEntryAt(INDEX row,INDEX col){
	return rowList[row][col];

}