#include "IntermediateResultTable.h"


IntermediateResultTable::IntermediateResultTable(SIZE tableSize)
{
	this->tableSize = tableSize;
	qrVarTable = new int[tableSize];
}


IntermediateResultTable::~IntermediateResultTable(void)
{
}
bool IntermediateResultTable::isQrVarDiscovered(INDEX qrVarIndex){
	if (qrVarTable[qrVarIndex]!=1) return false;
	return true;
}