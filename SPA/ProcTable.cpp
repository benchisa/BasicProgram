
#include "ProcTable.h"


typedef pair<Procedure*,int> procPair;
typedef pair<PROC_NAME,procPair> tablePair;
hash_map<PROC_NAME,procPair> *procTable;
hash_map<PROC_NAME,procPair>::const_iterator itr;

ProcTable::ProcTable(void){
	procTable = new hash_map<PROC_NAME,procPair>;

}
ProcTable::~ProcTable(void){
	delete procTable;
}
PROC_INDEX ProcTable::insertProc(Procedure * proc){
	itr=(*procTable).find(proc->getProcName());
	//if index is not empty, insert procedure and its index as a pair into the map
	if(itr!=(*procTable).end()){
		return itr->second.second;
	}else{
		procPair newPair(proc,(*procTable).size()+1);
		(*procTable).insert(tablePair(proc->getProcName(),newPair));
		return ((*procTable).size());
	}

}
SIZE ProcTable::getProceTableSize(){
	return (*procTable).size();
}
Procedure * ProcTable::getProcedure(PROC_INDEX index){
	//if invalid index, return NULL
	//else return the variable name with index "index"
	if(index<1||index>(*procTable).size()){
		return NULL;
	}else{

		for(itr=(*procTable).begin();itr!=(*procTable).end();itr++){
			if(itr->second.second == index){
				return itr->second.first;
			}
		}

	}
}

PROC_INDEX ProcTable::getProcIndex(PROC_NAME procName){
	//find the index with procedure name "name"
	itr= (*procTable).find(procName);
	int index = itr->second.second;

	if(index > 0){
		return index;
	}
	return NULL;

}
bool ProcTable::isExists(PROC_NAME procName){
	itr = (*procTable).find(procName);

	if(itr->second.second<1) return false;

	return true;
}
PROC_LIST * ProcTable::getAllProc(){
	list<Procedure> returnList;

	if((*procTable).size()==0) return NULL;
	for(itr=(*procTable).begin( );itr!= (*procTable).end( );++itr){
		returnList.push_back(*(itr->second.first));
		//returnList.push_back(new Procedure(itr->second.first));
	}
	return new list<Procedure>(returnList);
}
bool ProcTable::isInSameProc(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2){

	for(itr=(*procTable).begin();itr!=(*procTable).end();++itr){
		Procedure curProc = *(itr->second.first);
		int start = curProc.getStartProgLine();
		int end = curProc.getEndProgLine();

		if(end==NULL) return false;

		//if two statements in the same procedure return true
		if(ProcTable::inRange(stmt1,start,end)&&ProcTable::inRange(stmt2,start,end)){
			return true;
		}	
	}
	//endFor, return false
	return false;
}
bool ProcTable::inRange(int num, int start,int end){
	if((start<=num)&&(num<=end)) {
		return true;
	}
	return false;
}

