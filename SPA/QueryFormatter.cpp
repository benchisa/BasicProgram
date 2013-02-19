#pragma once
#include "QueryFormatter.h"

QueryFormatter::QueryFormatter(PKB * pkb){
	this->pkb = pkb;
}
QueryFormatter::~QueryFormatter(void){

}
void QueryFormatter::setQrTable(QUERYTABLE * qrTable){
	this->qrTable = qrTable;
}
list<string> QueryFormatter::formatString(RAWDATA * data) {
	list<string>  result;
	TYPE type;
	int value;

	//check if the type is boolean (special case)
	int firstVarIndex = data->at(0).at(0);
	if(firstVarIndex == -1) {
		value = data->at(0).at(1);
		if(value == 1) {
			result.push_back("true");
		}else {
			result.push_back("false");
		}
	}
	//checks the type of result at the 1st row and subsequent columns eg. Vector[cols][0]
	for(int i = 1; i < (data->at(0)).size(); i++) {
		//type = data-
		//type = (data[cols])[0].data;
		//translates the result eg. Vector[cols][rows]
		for(int j = 0; j < data->size(); j++) {

			//find the actul type of current qrVar
			int varIndex = (data->at(j)).at(0);
			QUERYTABLE::iterator itr;
			itr = qrTable->find(varIndex);
			type = itr->second;

			//get the data of curren qrVar
			value = (data->at(j)).at(i);

			if(type ==CONSTANT){
				int constValue = pkb->getConstantValue(value);
				result.push_back(static_cast<ostringstream*>( &(ostringstream() << constValue) )->str());
			}else if(type == VARIABLE) {
				VAR_NAME varName = pkb->getVarName(value);
				result.push_back(varName);
			}else if(type==PROCEDURE||type==CALL) { 
				Procedure * proc = pkb->getProcedure(value);
				result.push_back(proc->getProcName());
			}else {
				result.push_back(static_cast<ostringstream*>( &(ostringstream() << value) )->str());
			}
		}
	}

	return result;

}

