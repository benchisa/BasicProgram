#pragma once
#include "Global_Data_Abstraction.h"


class ProcTable
{
	public:
		ProcTable(void);
		~ProcTable(void);
		bool inRange(int num,int start,int end);
		PROC_INDEX insertProc(Procedure * proc);
		SIZE getProceTableSize();
		Procedure * getProcedure(PROC_INDEX index);
		PROC_INDEX getProcIndex(PROC_NAME procName);
		bool isExists(PROC_NAME procName);
		bool isInSameProc(STATEMENT_NUM stmt1, STATEMENT_NUM stmt2);
		PROC_LIST * getAllProc();
        
};

