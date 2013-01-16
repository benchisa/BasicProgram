#pragma once
#include "Global_Data_Abstraction.h"

class Procedure
{
	public:
		//Procedure(void);
		~Procedure(void);
		Procedure(PROC_NAME name,PROG_LINE startProgLine);
		void setEndProgLine(PROG_LINE endProgLine);
		PROC_NAME getProcName();
		PROG_LINE getStartProgLine();
		PROG_LINE getEndProgLine();

	private:
		PROC_NAME name;
		PROG_LINE startProgLine;
		PROG_LINE endProgLine;

};