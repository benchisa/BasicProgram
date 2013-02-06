#pragma once
#include "QueryFormatter.h"

void QueryFormatter::setPKB(PKB* pkb){
	this->pkb = pkb;

}

list<string> QueryFormatter::formatString(RAWDATA * data) {
	TYPE type = data->first;
	list<int> * dataList;
	dataList = data->second;
	list<int>::iterator i;
	list<string> result;

	if(dataList == NULL){
		//	result = "No matched result found!";
		return result;
	}

	for(i=dataList->begin(); i != dataList->end(); ++i) {

		if(type==VARIABLE) {
			VAR_NAME varName = pkb->getVarName(*i);

			
				//result.append("Variable: ");
				result.push_back(varName);
		

		}else if(type==BOOL) { 
			if(*i == 1) {
				result.push_back("true");
			}else {
				result.push_back("false");
			}
		}else if(type==PROCEDURE) { 
			Procedure * proc = pkb->getProcedure(*i);
			//result.append("Procedure Name: ");
			result.push_back(proc->getProcName());
		}else {
			//result.append("Statement No: ");
			result.push_back(static_cast<ostringstream*>( &(ostringstream() << (*i)) )->str());


		}

		//		result.append("\n");

	}
	//cout << result.size() << "\n";
	return result;

}