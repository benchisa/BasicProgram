#pragma once
#include "QueryEvaluator.h"
#include "QueryPreprocessor.h"
#include "QueryFormatter.h"
#include "DesignExtractor.h"

class PKB;
class PQLController
{
public:
	PQLController(void);
	~PQLController(void);

	FINAL_RESULT getQueryResult(QUERY newQuery);
	void setPKB(PKB* pkb);
	
private:
	QueryEvaluator* queryEval;
	QueryPreprocessor* queryProcessor;
	QueryFormatter* queryFormatter;
	DesignExtractor* extractor;
};


