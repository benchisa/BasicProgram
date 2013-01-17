#pragma once
#include "Global_Data_Abstraction.h"
class ConstantTable
{
public:
	ConstantTable(void);
	~ConstantTable(void);

	CONSTANT_INDEX insertVar(int constantValue);
	SIZE getConstantTableSize();
	int getConstantValue(CONSTANT_INDEX index);
	CONSTANT_INDEX getConstantIndex(int constantValue);
	bool isExists(int constantValue);
	CONSTANT_LIST *  getAllConstant();
};

