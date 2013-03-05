#pragma once
#include "Global_Data_Abstraction.h"
#include <hash_map>
#include <iostream>
class VarTable
{
	public:
		VarTable(void);
		~VarTable(void);
		VAR_INDEX insertVar(VAR_NAME varName);
		SIZE getVarTableSize();
		VAR_NAME  getVarName(VAR_INDEX index);
		VAR_INDEX getVarIndex(VAR_NAME varName);
		bool isExists(VAR_NAME varName);
		VAR_LIST *  getAllVar();
};

