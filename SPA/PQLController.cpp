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
		
		queryEval->evaluate(queryProcessor->getQTree(),queryProcessor->getQVarTable());
		//print the result
		
		returnResult= queryFormatter->formatString(queryEval->getResult());
		//cout << "returnResult: " << returnResult.size() << "\n";
		return returnResult;
	}

	//fail to preprocess the query and return the result
//	returnResult.push_back(queryProcessor->getLastError());
	return returnResult;
	
}
void PQLController::setPKB(PKB* pkb){
	queryProcessor = new QueryPreprocessor(pkb);
	queryEval = new QueryEvaluator(pkb);
	queryFormatter = new QueryFormatter();
	queryFormatter->setPKB(pkb);

}
