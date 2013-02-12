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
		
		queryEval->evaluate(queryProcessor->getQTree(),queryProcessor->getQVarTable(),queryProcessor->getParamTable());
		//print the result
		
	//	returnResult= queryFormatter->formatString(queryEval->getResult());
		//cout << "returnResult: " << returnResult.size() << "\n";
		returnResult.push_back("success");
		return returnResult;
	}else if(queryProcessor->getSelectBool()==true){
		returnResult.push_back("true");
		
	}else if(queryProcessor->getSelectBool()==false){
		returnResult.push_back("false");
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
