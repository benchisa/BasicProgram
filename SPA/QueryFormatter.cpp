#pragma once
#include "QueryFormatter.h"

void QueryFormatter::setPKB(PKB* pkb){
	this->pkb = pkb;

}
/*
list<string> QueryFormatter::formatString(RAWDATA * data) {
	list<string>  result;
	TYPE type;

	//checks the type of result at the 1st row and subsequent columns eg. Vector[cols][0]
	for(int cols = 0; cols < data->size(); cols++) {
		type = (data[cols])[0].data;
		//translates the result eg. Vector[cols][rows]
		for(int rows = 0; rows < (data[cols])[0].size(); rows++) {
			if(type == VARIABLE) {
				VAR_NAME varName = pkb->getVarName((data[cols])[rows].data);
				result.push_back(varName);
			}else if(type == BOOL) {
				if((data[cols])[rows].data == 1) {
					result.push_back("true");
				}else {
					result.push_back("false");
				}
			}else if(type==PROCEDURE) { 
				Procedure * proc = pkb->getProcedure((data[cols])[rows].data);
				result.push_back(proc->getProcName());
			}else {
				result.push_back(static_cast<ostringstream*>( &(ostringstream() << ((data[cols])[rows].data)) )->str());


			}
		}
	}

	return result;

}
*/
