#include "VarTable.h"


hash_map<string,int> *varTable;
hash_map<string,int>::const_iterator varItr1;

typedef pair<string,int> varPair;

VarTable::VarTable(void){
	varTable = new hash_map<string,int>;
	//cout<<&varTable;
}
VarTable::~VarTable(void){
	delete varTable;
} 
VAR_INDEX VarTable::insertVar(VAR_NAME varName){

	varItr1=varTable->find(varName);

	//if index is not empty, insert varName and its index as a pair into the map
	if(varItr1!=(*varTable).end()){
		return varItr1->second;
	}else{
		(*varTable).insert(varPair(varName,(*varTable).size()+1));
		return ((*varTable).size());
	}
}
SIZE VarTable::getVarTableSize(){
	return (*varTable).size();
}
 VAR_NAME  VarTable::getVarName(VAR_INDEX index){
	//if invalid index, return NULL
	//else return the variable name with index "index"
	if(index<1||index>(*varTable).size()){
		return NULL;
	}else{

		for(varItr1=(*varTable).begin();varItr1!=(*varTable).end();varItr1++){
			if(varItr1->second == index){
				return varItr1->first;
			}
		}
	}
}
VAR_INDEX VarTable::getVarIndex(VAR_NAME varName){
	//find the index with variable name "varName"
	varItr1 = (*varTable).find(varName);
	int index = varItr1->second;

	if(index > 0){
		return index;
	}
	return NULL;
}

bool VarTable::isExists(VAR_NAME varName){
	varItr1 = (*varTable).find(varName);

	if(varItr1->second<1) return false;

	return true;
}
VAR_LIST *  VarTable::getAllVar(){
	list<string> returnList;

	if((*varTable).size()==0) return NULL;
	for(varItr1=(*varTable).begin( );varItr1!= (*varTable).end( );varItr1++){
		returnList.push_back(varItr1->first);
	}
	return new list<string>(returnList);
}