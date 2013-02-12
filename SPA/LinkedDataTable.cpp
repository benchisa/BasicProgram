#include "LinkedDataTable.h"


LinkedDataTable::LinkedDataTable(void)
{
}


LinkedDataTable::~LinkedDataTable(void)
{
}

void LinkedDataTable::addEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries){
	if(mergeAttrPos==0){
		LinkedDataTable::createEntry(oldQrVar,newQrVar,newEntries);
	}else{
		LinkedDataTable::addOneEntry(mergeAttrPos,oldQrVar,newQrVar,newEntries);
	}
}
void LinkedDataTable::addOneEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries){
	//create new column
	qrVarList.push_back(newQrVar);

	//create a temporary row list
	ROW_LIST tempRowList;
	INDEX oldColNum = LinkedDataTable::getColNumOf(oldQrVar);
	//INDEX newColNum = LinkedDataTable::getColNumOf(newQrVar);

	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	
	//run until reach the last row
	while(currentRow!=rowList.end()){
		//get the pointer of the old entry in the table
		ROW::iterator currentEntry;
		currentEntry = currentRow->begin();
		advance(currentEntry,oldColNum);

		//compare the oldEntry and newEntry value
		RELATION_LIST::iterator newEntry;
		for(newEntry= newEntries->begin();newEntry!=newEntries->end();newEntry++){
			int mergeAttr;
			//merge attribute is at the first field
			if( mergeAttrPos==1){
				mergeAttr = newEntry->first;
				//matched entry is found, add to temp table
				if(mergeAttr==*currentEntry){
					ROW tempRow = *currentRow;
					tempRow.push_back(newEntry->second);
					tempRowList.push_back(tempRow);
				}
			}else{
				mergeAttr = newEntry->second;
				//matched entry is found, add to temp table
				if(mergeAttr==*currentEntry){
					ROW tempRow = *currentRow;
					tempRow.push_back(newEntry->first);
					tempRowList.push_back(tempRow);
				}
			}
		}

		currentRow++;
	}
	//replace the old row list
	rowList = tempRowList;
}
void LinkedDataTable::createEntry(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newEntries){
	//create 2 new columns
	qrVarList.push_back(firstQrVar);
	qrVarList.push_back(secondQrVar);

	RELATION_LIST::iterator currentEntry;
	for(currentEntry = newEntries->begin();currentEntry!=newEntries->end();currentEntry++){
		ROW newRow;
		newRow.push_back(currentEntry->first);
		newRow.push_back(currentEntry->second);
		rowList.push_back(newRow);
	}
}
bool LinkedDataTable::mergeEntries(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * mergeList){
	//create a temporary row list
	ROW_LIST tempRowList;
	INDEX firstColNum = LinkedDataTable::getColNumOf(firstQrVar);
	INDEX secondColNum = LinkedDataTable::getColNumOf(secondQrVar);

	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	
	//run until reach the last row
	while(currentRow!=rowList.end()){
		//get the value of the two old entries in the table
		ROW::iterator currentEntry;
		currentEntry = currentRow->begin();
		advance(currentEntry,firstColNum);
		int firstOldEntry = *currentEntry;

		currentEntry = currentRow->begin();
		advance(currentEntry,secondColNum);
		int secondOldEntry = *currentEntry;

		//compare the oldEntry and newEntry value
		RELATION_LIST::iterator mergeEntry;
		for(mergeEntry= mergeList->begin();mergeEntry!=mergeList->end();mergeEntry++){
			int firstMergeAttr = mergeEntry->first;
			int secondMergeAttr = mergeEntry->second;
			
			//matched entry is found, add to temp table
			if(firstOldEntry==firstMergeAttr&&secondOldEntry==secondMergeAttr){
				ROW tempRow = *currentRow;
				tempRowList.push_back(tempRow);
			}
		}
		currentRow++;
	}
	//replace the old row list
	if(tempRowList.size()>0){
		rowList = tempRowList;
		return true;
	}
	return false;
}
bool LinkedDataTable::mergeTable(INDEX firstQrVar,INDEX secondQrVar,LinkedDataTable * newTable){
	//create a temporary row list
	ROW_LIST tempRowList;
	//find the location of the two qrVar in two tables
	INDEX firstColNum1 = LinkedDataTable::getColNumOf(firstQrVar);
	INDEX secondColNum1 = LinkedDataTable::getColNumOf(secondQrVar);

	INDEX firstColNum2 = newTable->getColNumOf(firstQrVar);
	INDEX secondColNum2 = newTable->getColNumOf(secondQrVar);

	ROW_LIST::iterator currentRow1;
	currentRow1 = rowList.begin();
	
	bool newVarAdded = false;
	//run until reach the last row
	while(currentRow1!=rowList.end()){
		//get the value of the two old entries in the table
		ROW::iterator currentEntry;
		currentEntry = currentRow1->begin();
		advance(currentEntry,firstColNum1);
		int firstEntry1 = *currentEntry;

		currentEntry = currentRow1->begin();
		advance(currentEntry,secondColNum1);
		int secondEntry1 = *currentEntry;

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
				tempRowList.push_back(tempRow);
			}
		}
		currentRow1++;
	}
	//replace the old row list
	if(tempRowList.size()>0){
		rowList = tempRowList;
		return true;
	}
	return false;
	
}
void LinkedDataTable::deleteRowBy(INDEX qrVar,int data){
	INDEX colNum = LinkedDataTable::getColNumOf(qrVar);
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
	}
	
}
INDEX_LIST LinkedDataTable::getDataListOf(INDEX qrVar){
	INDEX_LIST returnList;
	INDEX colNum = LinkedDataTable::getColNumOf(qrVar);
	SIZE rowNum = rowList.size();

	ROW_LIST::iterator currentRow;
	//loop through the row list 
	for(currentRow = rowList.begin();currentRow!=rowList.end();currentRow++){
		ROW::iterator currentEntry;
		//locate the position of entry[row][col]
		currentEntry = currentRow->begin();
		advance(currentEntry,colNum);
		//add the value in return list
		returnList.push_back(*currentEntry);
	}
	
	returnList.unique();
	returnList.sort();

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
	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	advance(currentRow, rowNum);

	return *currentRow;
}
int LinkedDataTable::getEntryAt(INDEX row,INDEX col){
	ROW_LIST::iterator currentRow;
	currentRow = rowList.begin();
	advance(currentRow,row);

	ROW::iterator currentEntry;
	currentEntry = currentRow->begin();
	advance(currentEntry,col);

	return *currentEntry;
}