#include "QueryEvaluator.h"
#include "SuchThat.h"
#include <iostream>

//methods
QueryEvaluator::QueryEvaluator(PKB* pkb){
	this->pkb = pkb;
	extractor = new DesignExtractor(pkb);
	this->rawData = new RAWDATA();
}
QueryEvaluator::~QueryEvaluator(void){

}	

bool QueryEvaluator::evaluate(QTREE* qrTree,QUERYTABLE* qrTable,QUERYPARAM* qrParam){
	this->qrTree = qrTree;
	this->qrTable = qrTable;
	this->qrParam = qrParam;
	bool nonEmptyResult;

	//set the result node
	QTREE* resultNode;
	resultNode = qrTree->getFirstDescendant();

	//Compute the intermediate result
	QTREE* relationTree;
	relationTree = qrTree->getRightSibling(); 

	//eg select s
	if(relationTree==NULL){
		generateRaw(resultNode);
		return true;
	}
	//compute the intermediate result and store in a database
	IntermediateResultTable * resultTable ;
	resultTable = QueryEvaluator::computeIntermediateResult(relationTree);

	//can not find the result in the table,include the following cases:
	//a. one of the condition is false, select nonBoolean value
	//b. after taking intersection, the table is empty, select nonBoolean value
	if(!QueryEvaluator::findResult(resultNode, resultTable)){
		return false; 
	}
	//result is found in the table, include the following cases:
	//a. one of the condition is false, select Boolean value
	//b. after taking intersection, the table is empty, select Boolean value
	//c. the resultTable is not empty, return the corresponding results
	return true;
}	
void QueryEvaluator::generateRaw(QTREE* resultNode){
		//create the rawData
		rawData = new RAWDATA();

		//select bool
		if(resultNode->getType()==BOOL){
			DATA_LIST result;
		
			result.push_back(-1);
			result.push_back(1);
			rawData->push_back(result);
		}else{
			//get the type of the selected var
			TYPE resultVarType;
			QUERYTABLE::iterator qrTableItr;

			qrTableItr = qrTable->find(resultNode->getData());
			resultVarType = qrTableItr->second;
			rawData->push_back(*extractor->getStmtListOf(resultVarType));
		
			//add the col name, which is the qrVar index
			DATA_LIST::iterator rawItr;
			rawItr = rawData->at(0).begin();
			rawData->at(0).insert(rawItr,resultNode->getData());
		}
		
}
IntermediateResultTable * QueryEvaluator::computeIntermediateResult(QTREE* relationTree){
	
	IntermediateResultTable * resultTable;
	QTREE* currentClause = relationTree;
	
	resultTable = new IntermediateResultTable(qrTable->size(),pkb,qrTable,extractor);
	bool startClause = false;
	int currentFlag = currentClause->getData();
	int prevFlag = currentFlag;

	do{	
		if(prevFlag!=1&&currentFlag ==1){
			//flag the starting point of the clause, where selected var can be found
			resultTable = new IntermediateResultTable(qrTable->size(),pkb,qrTable,extractor);
		}

		resultTable = QueryEvaluator::evaluateClause(resultTable, currentClause);
		if(resultTable==NULL) return NULL;
		
		currentClause = currentClause->getRightSibling();
		//set the flag of the clause
		prevFlag = currentFlag;
		if(currentClause!=NULL)currentFlag = currentClause->getData();
	}while(currentClause!=NULL);

	return resultTable;
}
IntermediateResultTable * QueryEvaluator::evaluateClause(IntermediateResultTable * resultTable, QTREE* clause){
	
	TYPE clauseType = clause->getType();

	if(clauseType == SUCHTHAT){
		if(!QueryEvaluator::executeSuchThat(resultTable, clause))
			return NULL;
	}
	if(clauseType == PATTERN){
	/*	if(!QueryEvaluator::executePattern(resultTable,clause))
			return NULL;*/
	}
	if(clauseType == WITH){
		if(!QueryEvaluator::executeWith(resultTable,clause))
			return NULL;
	}

	return resultTable;
}
bool QueryEvaluator::executeSuchThat(IntermediateResultTable * resultTable, QTREE* suchThatTree){
	QTREE* firstRel;
	QTREE* secondRel;
	INDEX firstQrVar,secondQrVar;
	RELATION_LIST* currentResultList = new RELATION_LIST();
	SuchThatClause suchThatClause(pkb, qrTable,extractor);
	JOIN_ATTR joinAttr = 0;

	firstRel = suchThatTree->getFirstDescendant()->getFirstDescendant();
	secondRel = firstRel->getRightSibling();
	//original information of the two relations
	//RELATION_PAIR firstRelPair (firstRel->getType(),firstRel->getData());
	//RELATION_PAIR secondRelPair (secondRel->getType(),secondRel->getData());

	firstQrVar = firstRel->getData();
	secondQrVar = secondRel->getData();
	
	if(firstRel->getType()==QUERYVAR){
		int qrVarIndex = firstQrVar;
		joinAttr = qrVarIndex;

		//test to see if secondrRel is known
		if(secondRel->getType()!=QUERYVAR){
			secondQrVar = -1;
		}

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			firstRel is computed in previous steps,second Rel is to be discovered
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//find the type of the relation
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(firstRel->getData());
			TYPE relRealType = itr1->second;
			//replace the firstRel type with its real type
			firstRel->setType(relRealType);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){	
				//replace the firstRel with probe data
				firstRel->setData(*itr);

				RELATION_LIST * tempList;
				tempList = suchThatClause.evaluateSuchThatTree(suchThatTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else if(secondRel->getType()==QUERYVAR){
		int qrVarIndex = secondQrVar;
		joinAttr = qrVarIndex;

		//firstQrVar must be known
		firstQrVar = -1;

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			secondRel is computed in previous steps,first Rel is to be discovered
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//replace the firstRel with its real type
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(secondRel->getData());
			TYPE relRealType = itr1->second;
			secondRel->setType(relRealType);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				
				//replace the firstRel with probe data
				secondRel->setData(*itr);

				RELATION_LIST * tempList;
				tempList = suchThatClause.evaluateSuchThatTree(suchThatTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else{
		//both relations are known
		currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
	}

	//there is no result found with all the probes, this relation is not satisfied
	if(currentResultList==NULL){
		return false; 
	}else{
	//result is found, add into resultTable
		return resultTable->joinList(joinAttr, firstQrVar,secondQrVar,currentResultList);
	}
}
bool QueryEvaluator::executeWith(IntermediateResultTable * resultTable,QTREE* withTree){
	QTREE* firstRel;
	QTREE* secondRel;
	QTREE* firstAttr;
	QTREE* secondAttr;
	INDEX firstQrVar,secondQrVar;
	RELATION_LIST* currentResultList = new RELATION_LIST();
	WithClause withClause(pkb,extractor,qrTable,qrParam);
	JOIN_ATTR joinAttr = 0;

	firstRel = withTree->getFirstDescendant();
	firstAttr = firstRel->getRightSibling();
	secondRel = firstAttr->getRightSibling();
	secondAttr = secondRel->getRightSibling();
	firstQrVar = firstRel->getData();
	secondQrVar = secondRel->getData();
	
	if(firstRel->getType()==QUERYVAR){
		int qrVarIndex = firstQrVar;
		joinAttr = qrVarIndex;

		//test to see if secondrRel is known
		if(secondRel->getType()!=QUERYVAR){
			secondQrVar = -1;
		}

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = withClause.evaluateWithTree(withTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			firstRel is computed in previous steps,second Rel is to be discovered
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//replace the firstRel with probe data
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(firstRel->getData());
			TYPE relRealType = itr1->second;
			firstRel->setType(ANY);
			firstAttr->setType(relRealType);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				
				firstAttr->setData(*itr);

				RELATION_LIST * tempList;
				tempList = withClause.evaluateWithTree(withTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else if(secondRel->getType()==QUERYVAR){
		int qrVarIndex = secondQrVar;
		joinAttr = qrVarIndex;

		//firstQrVar must be known
		firstQrVar = -1;

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = withClause.evaluateWithTree(withTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			secondRel is computed in previous steps,first Rel is to be discovered
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//replace the firstRel with probe data
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(secondRel->getData());
			TYPE relRealType = itr1->second;
			secondRel->setType(ANY);
			secondAttr->setType(relRealType);
		
			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				
				secondAttr->setData(*itr);

				RELATION_LIST * tempList;
				tempList = withClause.evaluateWithTree(withTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else{
		//both relations are known
		currentResultList = withClause.evaluateWithTree(withTree);
	}

	//there is no result found with all the probes, this relation is not satisfied
	if(currentResultList==NULL){
		return false; 
	}else{
	//result is found, add into resultTable
		return resultTable->joinList(joinAttr, firstQrVar,secondQrVar,currentResultList);
	}
}
/*bool QueryEvaluator::executePattern(IntermediateResultTable * resultTable,QTREE* withTree){
	QTREE* firstRel;
	QTREE* secondRel;
	QTREE* firstAttr;
	QTREE* secondAttr;
	INDEX firstQrVar,secondQrVar;
	RELATION_LIST* currentResultList = new RELATION_LIST();
	WithClause withClause(pkb,extractor,qrTable,qrParam);
	JOIN_ATTR joinAttr = 0;

	firstRel = withTree->getFirstDescendant();
	firstAttr = firstRel->getRightSibling();
	secondRel = firstAttr->getRightSibling();
	secondAttr = secondRel->getRightSibling();
	firstQrVar = firstRel->getData();
	secondQrVar = secondRel->getData();
	
	if(firstRel->getType()==QUERYVAR){
		int qrVarIndex = firstQrVar;
		joinAttr = qrVarIndex;

		//test to see if secondrRel is known
		if(secondRel->getType()!=QUERYVAR){
			secondQrVar = -1;
		}

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = withClause.evaluateWithTree(withTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			firstRel is computed in previous steps,second Rel is to be discovered
		*//*
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//replace the firstRel with probe data
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(firstRel->getData());
			TYPE relRealType = itr1->second;
			firstRel->setType(ANY);
			firstAttr->setType(relRealType);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				
				firstAttr->setData(*itr);

				RELATION_LIST * tempList;
				tempList = withClause.evaluateWithTree(withTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else if(secondRel->getType()==QUERYVAR){
		int qrVarIndex = secondQrVar;
		joinAttr = qrVarIndex;

		//firstQrVar must be known
		firstQrVar = -1;

		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = withClause.evaluateWithTree(withTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			secondRel is computed in previous steps,first Rel is to be discovered
		*/
		/*	INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			//replace the firstRel with probe data
			QUERYTABLE::iterator itr1;
			itr1 = qrTable->find(secondRel->getData());
			TYPE relRealType = itr1->second;
			secondRel->setType(ANY);
			secondAttr->setType(relRealType);
		
			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				
				secondAttr->setData(*itr);

				RELATION_LIST * tempList;
				tempList = withClause.evaluateWithTree(withTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else{
		//both relations are known
		currentResultList = withClause.evaluateWithTree(withTree);
	}

	//there is no result found with all the probes, this relation is not satisfied
	if(currentResultList==NULL){
		return false; 
	}else{
	//result is found, add into resultTable
		return resultTable->joinList(joinAttr, firstQrVar,secondQrVar,currentResultList);
	}
}
*/
RELATION_LIST * QueryEvaluator::appendRelationLists(RELATION_LIST* list1,RELATION_LIST* list2){
	if (list2==NULL) return list1;

	RELATION_LIST::iterator itr;

	for(itr = list2->begin();itr!=list2->end();itr++){
		list1->push_back(*itr);
	}

	return list1;
}
bool QueryEvaluator::findResult(QTREE* resultNode,IntermediateResultTable* resultTable){
	
	//the table is empty
	//if(resultTable==NULL) return false;

	int nodeNum = 0;

	QTREE* headNode;
	TYPE headNodeType;
	
	headNode = resultNode; // first node of the resultNode chain
	
	//translate the qrVar to its real type
	QUERYTABLE::iterator itr;
	if(headNode->getType()!=BOOL){
		itr = qrTable->find(headNode->getData());
		headNodeType = itr->second;
	}else{
		headNodeType = headNode->getType();
	}
		
	if((headNodeType!=BOOL) && (resultTable==NULL)){ //empty intersection/one of the condition is false
		/*DATA_LIST resultType;
		DATA_LIST content;
		
		resultType.push_back(0);
		content.push_back(0);
		rawData->push_back(resultType);
		rawData->push_back(content);*/

		return false;
	}else if((headNodeType==BOOL) && (resultTable==NULL)){//special case:--return type is boolean
		DATA_LIST result;

		result.push_back(-1);
		result.push_back(0);
		rawData->push_back(result);

		return true;
	}else if((headNodeType==BOOL) && (resultTable!=NULL)){
		DATA_LIST result;
		
		result.push_back(-1);
		result.push_back(1);
		rawData->push_back(result);
	
		return true;
	}

	//main: find the result combination
	QTREE* currentNode;
	currentNode = headNode;
	DATA_LIST resultVarList;
	do{
		resultVarList.push_back(currentNode->getData());
		currentNode = currentNode->getRightSibling();
	}while(currentNode!=NULL);

	//there must be something found in this stage
	rawData = resultTable->findResultOf(resultVarList);

	//if(rawData==NULL) return false;
	return true;
}
RAWDATA* QueryEvaluator::getRawResult(){
	return rawData;
}
