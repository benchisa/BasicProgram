#pragma once
#include "Global_Data_Abstraction.h"
#include "VarTable.h"
#include "ProcTable.h"
#include "Procedure.h"
#include "PKB.h"

class QueryFormatter{
public:
	PKB * pkb;
	void setPKB(PKB* pkb);
	list<string> formatString(RAWDATA * data);

};