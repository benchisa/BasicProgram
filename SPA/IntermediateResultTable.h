#pragma once
#include "Global_Data_Abstraction.h"

class IntermediateResultTable
{
public:
	IntermediateResultTable(SIZE tableSize);
	~IntermediateResultTable(void);
	
	//if joinAttr is 0, both of the two relations are not in the table
	INDEX joinList(JOIN_ATTR qrVarIndex,RELATION_PAIR firstRel,RELATION_PAIR secondRel,RELATION_LIST * newList);
//	RELATION_PAIR getFirstRel(INDEX recordIndex);
//	RELATION_PAIR getSecondRel(INDEX recordIndex);
//	RELATION_LIST * getRelationListOf(INDEX recordIndex);
	bool isQrVarDiscovered(INDEX qrVarIndex);
	INDEX_LIST* getResultListOf(INDEX qrVarIndex);

private:
	SIZE tableSize;
	//an array to store the qr variables
	int * qrVarTable;
	



};

