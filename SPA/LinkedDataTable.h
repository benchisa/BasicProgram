#pragma once
#include "Global_Data_Abstraction.h"
class LinkedDataTable
{
public:

	LinkedDataTable(void);
	~LinkedDataTable(void);

	//pos=0, creat a new table
	//pos=1, mergeAttr is the first, 
	//pos=2, mergeAttr is the second 
	bool addEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries);

	//both exists in the table
	bool mergeEntries(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * mergeList); 
	bool mergeTable(INDEX firstQrVar,INDEX secondQrVar,LinkedDataTable * newTable);
	void deleteRowBy(INDEX QrVar,int data);
	INDEX_LIST * getDataListOf(INDEX qrVar);
	INDEX getColNumOf(INDEX qrVar);
	bool isExists(INDEX qrVar);
	ROW getRow(INDEX rowNum);
	ROW getQrVarList();
	SIZE getSize();
	int getEntryAt(INDEX row,INDEX col);

private:
	
	ROW qrVarList;
	ROW_LIST rowList;

	bool addOneEntry(int mergeAttrPos,INDEX oldQrVar,INDEX newQrVar,RELATION_LIST * newEntries);
	void createEntry(INDEX firstQrVar,INDEX secondQrVar,RELATION_LIST * newEntries);
};

