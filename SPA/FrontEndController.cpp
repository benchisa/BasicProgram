#include "FrontEndController.h"

FrontEndController::FrontEndController(void)
{	//holds pointer of both parser and design extractor
	parser = new Parser;
}


FrontEndController::~FrontEndController(void)
{	
	delete parser;
}
int FrontEndController::analyzeProgram(SOURCE source){
	parser->setSource(source);
	int returnVal = parser->startParse();

	return returnVal;
}
PKB* FrontEndController::getPKB(){
	//Get PKB pointer from parser, TODO
	return parser->getPKB();
}
ERROR_MSG FrontEndController::getParserError(){
	return parser->getLastError();
}