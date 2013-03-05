#pragma once
#include "Global_Data_Abstraction.h"
#include "FrontEndController.h"
#include "PQLController.h"

class FrontEndController;
class PQLController;

class SPAController
{
public:
	SPAController(void);
	~SPAController(void);
	
	REPORT_MSG enterProgram(SOURCE source);
	FINAL_RESULT getQueryResult(QUERY newQuery);
private:
	FrontEndController* frontEnd;
	PQLController* pql;
	bool isParsed;
};

