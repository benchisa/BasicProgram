#include "PQLController.h"


PQLController::PQLController(void)
{
}


PQLController::~PQLController(void)
{
}

FINAL_RESULT PQLController::getQueryResult(QUERY newQuery){
	
	FINAL_RESULT returnResult;
	
	//set Query to preprocessor
	queryProcessor->setQuery(newQuery);
		
	//process query & evaluate the result
	if(queryProcessor->preProcess()==true){
		//get query tree and query table from preprocessor, pass them to query evaluator
		
		if(queryEval->evaluate(queryProcessor->getQTree(),queryProcessor->getQVarTable(),queryProcessor->getParamTable())){
			//call formatter
			queryFormatter->setQrTable(queryProcessor->getQVarTable());
			returnResult= queryFormatter->formatString(queryEval->getRawResult());
		}else{
			//failed to evaluate the result, return nothing
			returnResult.push_back("");
		}

		//cout << "returnResult: " << returnResult.size() << "\n";
	}else if(queryProcessor->getSelectBool()==true){
		returnResult.push_back("true");
		
	}else if(queryProcessor->getSelectBool()==false){
		returnResult.push_back("false");
	}

	return returnResult;
	
}
void PQLController::setPKB(PKB* pkb){
	queryProcessor = new QueryPreprocessor(pkb);
	queryEval = new QueryEvaluator(pkb);
	queryFormatter= new QueryFormatter(pkb);
}
