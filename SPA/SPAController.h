#pragma once
#include "Global_Data_Abstraction.h"

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
	bool setPKB;
};

