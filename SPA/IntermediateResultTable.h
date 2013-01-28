#pragma once
#include "Global_Data_Abstraction.h"

class IntermediateResultTable
{
public:
	IntermediateResultTable(SIZE tableSize);
	~IntermediateResultTable(void);
	
	INDEX addResultList(RELATION_PAIR firtRel,RELATION_PAIR secondRel,RELATION_LIST * newList);
	RELATION_PAIR getFirstRel(INDEX recordIndex);
	RELATION_PAIR getSecondRel(INDEX recordIndex);
	RELATION_LIST * getResultLlist(INDEX recordIndex);
	SIZE getSize();
	bool modifyResultList(INDEX recordIndex, RELATION_LIST * newList);
	bool isQrVarExists(INDEX qrVarIndex);
	INDEX_LIST* getResultListOf(INDEX qrVarIndex);

private:
	SIZE tableSize;
	INDEX_LIST * indexTable;
	RELATION_PAIR **pairTable;
	RELATION_LIST * resultListTable;



};

