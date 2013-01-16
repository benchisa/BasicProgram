#include "Procedure.h"

//Procedure::Procedure(void){}

Procedure::~Procedure(void){}


Procedure::Procedure(PROC_NAME name,PROG_LINE startProgLine){
	
	this->name=name;
	this->startProgLine=startProgLine;
	endProgLine = NULL;

}

void Procedure::setEndProgLine(PROG_LINE endProgLine){
	this->endProgLine = endProgLine;
}
PROC_NAME Procedure::getProcName(){
	return name;
}

PROG_LINE Procedure::getStartProgLine(){
	return startProgLine;
}
PROG_LINE Procedure::getEndProgLine(){
	return endProgLine;
}
