#pragma once
#include "Global_Data_Abstraction.h"
#include "Parser.h"


class PKB;

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

