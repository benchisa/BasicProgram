#pragma once
#include "Global_Data_Abstraction.h"
class ConstantTable
{
public:
	ConstantTable(void);
	~ConstantTable(void);

	CONSTANT_INDEX insertConst(int constantValue);
	SIZE getConstantTableSize();
	int getConstantValue(CONSTANT_INDEX index);
	CONSTANT_INDEX getConstantIndex(int constantValue);
	bool isExists(int constantValue);
	CONSTANT_LIST *  getAllConstant();

private:
	list<int> constantList;
	int counter;
	typedef list<int>::iterator listItr;
};

