#pragma once
#include "Global_Data_Abstraction.h"


class PKB;
class Parser;

class FrontEndController
{
public:
	FrontEndController(void);
	~FrontEndController(void);

	int analyzeProgram(SOURCE source);
	ERROR_MSG getParserError();
	PKB* getPKB();
private:
	Parser* parser;
};

