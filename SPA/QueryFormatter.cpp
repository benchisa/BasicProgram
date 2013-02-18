#pragma once
#include "QueryFormatter.h"

void QueryFormatter::setPKB(PKB* pkb){
	this->pkb = pkb;

}

list<string> QueryFormatter::formatString(RAWDATA * data) {
	list<string>  result;
	int type, value;

	//checks the type of result at the 1st row and subsequent columns eg. Vector[cols][0]
	for(int i = 1; i < (data->at(0)).size(); i++) {
		//type = data-
		//type = (data[cols])[0].data;
		//translates the result eg. Vector[cols][rows]
		for(int j = 0; j < data->size(); j++) {
			type = (data->at(j)).at(0);
			value = (data->at(j)).at(i);
			if(type == VARIABLE) {
				VAR_NAME varName = pkb->getVarName(value);
				result.push_back(varName);
			}else if(type == BOOL) {
				if(value == 1) {
					result.push_back("true");
				}else {
					result.push_back("false");
				}
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

