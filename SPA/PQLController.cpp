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
	//cout << "the query: " << newQuery << endl;	
	//process query & evaluate the result
	if(queryProcessor->preProcess()==true){
		//get query tree and query table from preprocessor, pass them to query evaluator
		
		//cout << "the query is parsed!" << endl;
		if(queryEval->evaluate(queryProcessor->getQTree(),queryProcessor->getQVarTable(),queryProcessor->getParamTable())){
			
			//cout << "the evaluator is OK" << endl;	
			//call formatter
			queryFormatter->setQrTable(queryProcessor->getQVarTable());
			//cout << "the formatter is OK" << endl;	
			returnResult= queryFormatter->formatString(queryEval->getRawResult());	
			FINAL_RESULT::iterator itr;
			//cout<<"\n Final result is:";
			//for(itr = returnResult.begin();itr!=returnResult.end();itr++){
			//	cout<< *itr <<" "  ;
			//}

		}else{
			//failed to evaluate the result, return nothing
			returnResult.push_back("");
		}

		//cout << "returnResult: " << returnResult.size() << "\n";
	}else if(queryProcessor->getSelectBool()==true){
		returnResult.push_back("false");
		
	}else if(queryProcessor->getSelectBool()==false){
		returnResult.push_back("");
	}

	return returnResult;
	
}
void PQLController::setPKB(PKB* pkb){
	queryProcessor = new QueryPreprocessor(pkb);
	queryEval = new QueryEvaluator(pkb);
	queryFormatter= new QueryFormatter(pkb);
}
