#pragma once
#include "Global_Data_Abstraction.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Procedure.h"
#include "PKB.h"

class QueryFormatter{
public:
	QueryFormatter(PKB* pkb);
	~QueryFormatter(void);
	void setQrTable(QUERYTABLE * qrTable);
	list<string> formatString(RAWDATA * data);
private:
	PKB * pkb;
	QUERYTABLE * qrTable;

};