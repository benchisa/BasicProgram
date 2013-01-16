#pragma once
#include "Global_Data_Abstraction.h"
#include "QTREE.h"
#include "Pattern.h"
#include <hash_map>


class QueryEvaluator{
public:

	//methods
	QueryEvaluator(PKB* pkb);
	~QueryEvaluator(void);

	void evaluate(QTREE* qrTree,QUERYTABLE* qrTable);
	RAWDATA* getResult();
//	ERROR_MSG getLastError();
	

private:
	QUERYTABLE * qrTable;
	QTREE* qrTree;
	QTREE* resultNode;
	PKB * pkb;
	TYPE resultType;
	INDEX resultIndex;
	Pattern patProcessor;

    INDEX_LIST * returnList;
	RAWDATA* rawData;
	
	RELATION_LIST* suchThat(QTREE* suchThatTree);
	INDEX_LIST* patternRel(QTREE* suchThatTree);
	INDEX_LIST* patternNode(QTREE* suchThatTree);

	void iterateAndStore(RELATION_LIST *result, RELATION_LIST list){
		if(list.empty()){
			result=NULL;
		}else{
			    RELATION_LIST::iterator itr = list.begin();
				while(itr!=list.end()){
					result->push_back(*itr);
					itr++;
				}	
		};
	}
};