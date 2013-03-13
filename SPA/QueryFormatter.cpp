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

FINAL_RESULT QueryFormatter::formatString(FINALRAW * data,DATA_LIST * selectedVars){

	TYPE type;
	int value;
	FINAL_RESULT  result;
	//check if the type is boolean (special case)
	if(selectedVars==NULL||selectedVars->size()==0){
		if(data->begin()->second[0]==1){
			result.push_back("true");
		}else{
			result.push_back("false");
		}

		return result;
	}
	
	//check if there is empty result
	if(data==NULL||data->size()<1) return result;

	//check the type of result at the 1st row and subsequent columns eg. Vector[cols][0]
	FINALRAW::iterator itr;
	for(itr = data->begin();itr!=data->end();itr++){
		//find current set of result
		DATA_LIST currentValue = itr->second;
		string currentEntry;  //entry in string result

		for(int i=0;i<selectedVars->size();i++){
			//find current selected variable type
			TYPE currentVarType = qrTable->find(selectedVars->at(i))->second;
			//find current data in the set of final raw
			int currData = currentValue[i];
			string currentValue;

			if(currentVarType == VARIABLE) {
				VAR_NAME varName = pkb->getVarName(currData);
				currentValue = varName;
			}else if(currentVarType==PROCEDURE) { 
				Procedure * proc = pkb->getProcedure(currData);
				currentValue = proc->getProcName();
			}else if(currentVarType==CONSTANT){
				int constValue = pkb->getConstantValue(currData);
				currentValue = static_cast<ostringstream*>( &(ostringstream() << constValue) )->str();
			}else{
				currentValue = static_cast<ostringstream*>( &(ostringstream() << currData) )->str();
			}
			currentEntry.append(currentValue);
			if(i!=selectedVars->size()-1) currentEntry.append(" ");
		}
		result.push_back(currentEntry);
	}
	return result;
}

/*FINAL_RESULT QueryFormatter::formatString(RAWDATA * data) {
	
	TYPE type;
	int value;

	//check if the type is boolean (special case)
	int firstVarIndex = data->at(0).at(0);
	
	if(firstVarIndex == -1) {
		value = data->at(0).at(1);
		if(value == 1) {			
			result.push_back("true");
		}else {				
			cout << "false!" << endl;
			result.push_back("false");
		}

		return result;
	}
	//check if there is empty result
	if(data->at(0).size()==1) return result;

	//check the type of result at the 1st row and subsequent columns eg. Vector[cols][0]
	for(int i = 1; i < (data->at(0)).size(); i++) {
		//type = data-
		//type = (data[cols])[0].data;
		//translates the result eg. Vector[cols][rows]
		string currentEntry;
		for(int j = 0; j < data->size(); j++) {
			string currentValue;

			//find the actul type of current qrVar
			int varIndex = (data->at(j)).at(0);
			QUERYTABLE::iterator itr;
			itr = qrTable->find(varIndex);
			type = itr->second;

			//get the data of curren qrVar
			value = (data->at(j)).at(i);

			if(type == VARIABLE) {
				VAR_NAME varName = pkb->getVarName(value);
				currentValue = varName;
			}else if(type==PROCEDURE) { 
				Procedure * proc = pkb->getProcedure(value);
				currentValue = proc->getProcName();
			}else if(type==CONSTANT){
				int constValue = pkb->getConstantValue(value);
				currentValue = static_cast<ostringstream*>( &(ostringstream() << constValue) )->str();
			}else{
				currentValue = static_cast<ostringstream*>( &(ostringstream() << value) )->str();
			}
			currentEntry.append(currentValue);
			if(j!=data->size()-1) currentEntry.append(" ");
		}
		result.push_back(currentEntry);
	}

	return result;

}*/