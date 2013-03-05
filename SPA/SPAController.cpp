
#include "SPAController.h"
SPAController::SPAController(void)
{
	frontEnd = new FrontEndController;
	pql = new PQLController;
	isParsed = false;
}


SPAController::~SPAController(void)
{
	delete frontEnd;
	delete pql;
}
REPORT_MSG SPAController::enterProgram(SOURCE source){
	//parse the source program
	int returnVal = frontEnd->analyzeProgram(source);

	string returnMsg = "";

	if(returnVal!=-1){
		//parse successfully
		isParsed = true;
		return "Parse the program Successfully!";

	}else{
		//fail to parse the source
		returnMsg = "The program has syntax error,parse the program Fail!";
		returnMsg+= "error:"+frontEnd->getParserError();

		return returnMsg;
	}
}
FINAL_RESULT SPAController::getQueryResult(QUERY newQuery){
	list<string> returnResult;
	
	if(isParsed){
		//pass the pkb pointer to query processer
		pql->setPKB(frontEnd->getPKB());
		//let PQL to evaluate the query, and return the result found
		return pql->getQueryResult(newQuery);
	}
	return returnResult;

	//PKB is not set in PQL, return error MSG
	//return "The pkb has not been initialized.";
}