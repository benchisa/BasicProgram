#include "IntermediateResultTable.h"


IntermediateResultTable::IntermediateResultTable(SIZE tableSize)
{
	this->tableSize = tableSize;
	indexTable = new INDEX_LIST[tableSize];
}


IntermediateResultTable::~IntermediateResultTable(void)
{
}
