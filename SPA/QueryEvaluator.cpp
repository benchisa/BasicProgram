#include "QueryEvaluator.h"
#include "SuchThat.h"
#include <iostream>

//methods
QueryEvaluator::QueryEvaluator(PKB* pkb){
	this->pkb = pkb;
}
QueryEvaluator::~QueryEvaluator(void){

}	

bool QueryEvaluator::evaluate(QTREE* qrTree,QUERYTABLE* qrTable,QUERYPARAM* qrParam){
	this->qrTree = qrTree;
	this->qrTable = qrTable;
	this->qrParam = qrParam;
	bool nonEmptyResult;

	//Compute the intermediate result
	QTREE* relationTree;
	relationTree = qrTree->getFirstDescendant()->getRightSibling();
	IntermediateResultTable * resultTable;

	resultTable = QueryEvaluator::computeIntermediateResult(relationTree);

	//find the result from resultTable
	QTREE* resultNode;
	resultNode = qrTree->getFirstDescendant();

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

IntermediateResultTable * QueryEvaluator::computeIntermediateResult(QTREE* relationTree){
	
	QTREE* currentClause = relationTree;
	IntermediateResultTable * resultTable;
	resultTable = new IntermediateResultTable(qrTable->size());
	int selectPoint = 1;

	do{	
		if(selectPoint ==1){
			//flag the starting point of the clause, where selected var can be found
			resultTable = new IntermediateResultTable(qrTable->size());
		}

		resultTable = QueryEvaluator::evaluateClause(resultTable, currentClause);
		if(resultTable==NULL) return NULL;
		
		currentClause = currentClause->getRightSibling();
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
	}
	if(clauseType == WITH){
	}
}
bool QueryEvaluator::executeSuchThat(IntermediateResultTable * resultTable, QTREE* suchThatTree){
	QTREE* firstRel;
	QTREE* secondRel;
	RELATION_LIST* currentResultList;
	SuchThatClause suchThatClause(pkb, qrTable);
	JOIN_ATTR joinAttr = 0;

	firstRel = suchThatTree->getFirstDescendant()->getFirstDescendant();
	secondRel = firstRel->getRightSibling();
	//original information of the two relations
	RELATION_PAIR firstRelPair (firstRel->getType(),firstRel->getData());
	RELATION_PAIR secondRelPair (secondRel->getType(),secondRel->getData());

	if(firstRel->getType()==QUERYVAR){
		int qrVarIndex = firstRel->getData();
		joinAttr = qrVarIndex;
		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			firstRel is computed in previous steps,second Rel is specified in the query
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				//replace the firstRel with probe data
				QUERYTABLE::iterator itr1;
				itr1 = qrTable->find(firstRel->getData());
				TYPE relRealType = itr1->second;

				firstRel->setType(relRealType);
				firstRel->setData(*itr);

				RELATION_LIST * tempList;
				tempList = suchThatClause.evaluateSuchThatTree(suchThatTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else if(secondRel->getType()==QUERYVAR){
		int qrVarIndex = secondRel->getData();
		joinAttr = qrVarIndex;
		//test to see if firstRelVar is already probed in previous steps
		if(!resultTable->isQrVarDiscovered(qrVarIndex)){
			//evaluate the tree
			currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
		}else{
		// current qrVar is evaluated in the previous steps
		/*algo:
			get the evaluated resultList of this qrVar from result table
			use the list as probe to get the correct result
			firstRel is computed in previous steps,second Rel is specified in the query
		*/
			INDEX_LIST * currentVarResultList;
			currentVarResultList = resultTable->getResultListOf(qrVarIndex);

			INDEX_LIST::iterator itr;
			for(itr = currentVarResultList->begin();itr!=currentVarResultList->end();itr++){
				//replace the firstRel with probe data
				QUERYTABLE::iterator itr1;
				itr1 = qrTable->find(secondRel->getData());
				TYPE relRealType = itr1->second;

				secondRel->setType(relRealType);
				secondRel->setData(*itr);

				RELATION_LIST * tempList;
				tempList = suchThatClause.evaluateSuchThatTree(suchThatTree);

				currentResultList = QueryEvaluator::appendRelationLists(currentResultList,tempList);		
			}
		}
	}else{
		currentResultList = suchThatClause.evaluateSuchThatTree(suchThatTree);
	}

	//there is no result found with all the probes, this relation is not satisfied
	if(currentResultList==NULL){
		return false; 
	}else{
	//result is found, add into resultTable
		resultTable->joinList(joinAttr, firstRelPair,secondRelPair,currentResultList);
	}
	return true;
}
bool QueryEvaluator::findResult(QTREE* resultNode,IntermediateResultTable* resultTable){
	
	//the table is empty
	if(resultTable==NULL) return false;

	int nodeNum = 0;

	QTREE* currentNode;
	currentNode = resultNode->getFirstDescendant();

	while(currentNode!=NULL){
		nodeNum++;
		currentNode = currentNode->getRightSibling();
	}
	
	switch(nodeNum){
	case 1:
		{//translate the qrVar to its real type
			currentNode = resultNode->getFirstDescendant();
			QUERYTABLE::iterator itr;
			itr = qrTable->find(currentNode->getData());
			TYPE currentRealType = itr->second;
		
			if((currentRealType!=BOOL) && (resultTable==NULL)){
				return false;
			}else if((currentRealType==BOOL) && (resultTable==NULL)){
				returnList->push_back(0);
				rawData = new RAWDATA(BOOL,returnList);
				return true;
			}else{
			
			}
		}
		break;
	case 2:
		break;
	case 3:
		break;
	}
}
RAWDATA* QueryEvaluator::getResult(){
	
}
/*
PKB * SuchThat::pkb;
QueryEvaluator::QueryEvaluator(PKB * p){
	this->pkb = p;
	this->patProcessor.setPKB(pkb);
	this->returnList = new INDEX_LIST;
	SuchThat::pkb = pkb;
}
QueryEvaluator::~QueryEvaluator(void){
	delete returnList;
	delete pkb;
}
void QueryEvaluator::evaluate(QTREE* qrTree,QUERYTABLE* qTable){
	// always clear resultList
	if(returnList != 0) 
	{
		returnList->clear();
	}
	else
	{
		this->returnList = new INDEX_LIST;
		//cout << returnList << "\n";
	}

	this->qrTree= qrTree;
	this->qrTable = qTable;
	hash_map<int, TYPE>::iterator itr;

	for(itr = qTable->begin(); itr != qTable->end(); itr++){
		this->qrTable->insert(pair<int, TYPE>(itr->first, itr->second));
	}

	// root pointer of the query tree
	resultNode = qrTree->getFirstDescendant();
	//	cout<<"resultNode Type:"<< resultNode->getType();

	//	QUERYTABLE::iterator itr;
	//	itr = qrTable->find(resultNode->getData());

	//get the type of result variable
	resultType = resultNode->getFirstDescendant()->getType();
	//get the index of result varaible
	resultIndex = resultNode->getFirstDescendant()->getData();
	//cout<<"resultType: "<<resultType;
	//cout<<"resultIndex: "<<resultIndex;
	//traverse the query tree
	QTREE* currentNode;
	currentNode = resultNode;

	//store intemediate results
	INDEX_LIST * patternListNode;
	patternListNode = NULL;

	INDEX_LIST * patternListRel;
	patternListRel = NULL;

	RELATION_LIST * relList;
	relList = NULL;
	bool patternCond = true;
	bool relationCond = true;

	//compute result
	while(currentNode->getRightSibling()!=NULL){

		currentNode = currentNode->getRightSibling();
		TYPE nodeType = currentNode->getType();

		if(nodeType == SUCHTHAT){
			relList = QueryEvaluator::suchThat(currentNode);
		//	cout<<"relList Size: "<<relList->size();
			if(relList==NULL) {
				relationCond = false;
			}else{
				if(relList->size()==0) relationCond = false;
			}

		}else if(nodeType == PATTERN){
			patternListNode = QueryEvaluator::patternNode(currentNode);
			patternListRel =  QueryEvaluator::patternRel(currentNode);
			if(patternListNode==NULL||patternListRel==NULL) 
				patternCond= false;
		}
	}


	//traverse the query tree to take the intersection
	if(relationCond == true && patternCond ==true) {

		QTREE* currentNode;
		currentNode = resultNode;
		QTREE * firstRel;
		QTREE * secondRel;

		//take intersection of intermediate result, if there are results in both lists
		if(relList!=NULL&&patternListNode!=NULL&&patternListRel!=NULL){

			QTREE* patternVar;

			while(currentNode->getRightSibling()!=NULL){

				currentNode = currentNode->getRightSibling();
				TYPE nodeType = currentNode->getType();

				if(nodeType==SUCHTHAT){
					firstRel = currentNode->getFirstDescendant()->getFirstDescendant();
					secondRel = firstRel->getRightSibling();
				}
				if(nodeType==PATTERN){
					patternVar = currentNode->getFirstDescendant();
				}
			}

			//pattern entity overlap with first rel entity
			INDEX_LIST tempPatternList;
		//	tempPatternList = NULL;
			RELATION_LIST tempRelList;
	//		tempRelList = NULL;
			RELATION_LIST::iterator itr1;
			INDEX_LIST::iterator itr2;
			bool intersect1 =false;
			bool intersect2 =false;

			//pattervar = firstRel
			if((patternVar->getType()==firstRel->getType())&&(patternVar->getData()==firstRel->getData())){
				intersect1 = true;
				for(itr1 = relList->begin();itr1 != relList->end();itr1++){
					for(itr2=patternListNode->begin();itr2!=patternListNode->end();itr2++){
						if(itr1->first==*itr2){
							int patternIndex = *itr2;
							tempPatternList.push_back(patternIndex);

							pair<int,int> relPair = *itr1;
							tempRelList.push_back(relPair);
						}
					}
				}
			}
			//patternvar = secondRel
			if((patternVar->getType()==secondRel->getType())&&(patternVar->getData()==secondRel->getData())){
				intersect2 = true;
				for(itr1 = relList->begin();itr1 != relList->end();itr1++){
					for(itr2=patternListNode->begin();itr2!=patternListNode->end();itr2++){
						if(itr1->second==*itr2){
							int patternIndex = *itr2;
							tempPatternList.push_back(patternIndex);

							pair<int,int> relPair = *itr1;
							tempRelList.push_back(relPair);
						}
					}
				}
			}
				// the intermediate result is narrowed down
			if(tempPatternList.size()>0){
				patternListNode = new INDEX_LIST(tempPatternList);
				relList = new RELATION_LIST(tempRelList);
				//cout<<"patternList size:"<<patternList->size();
				//cout<<"relList size1:"<<relList->size();
				//set bool result to true
				if((intersect1==true||intersect2==true)&&resultType==BOOL){
					returnList->push_back(1);
				}
				//reset temList
				tempRelList.clear();
				tempPatternList.clear();
			}else{
				//same variable appears in two clauses but no intersection found,
				//set bool result to false and returnList to null
				if(intersect1==true||intersect2==true){
					if(resultType==BOOL){
						returnList->push_back(0);
					}else{
						returnList = NULL;
					}
					return;
				}
			}
			
	/*
			//patternFirstRel = firstRel
			if((patternVar->getFirstDescendant()->getType()==firstRel->getType())&&(patternVar->getFirstDescendant()->getData()==firstRel->getData())){
				intersect1 = true;
				for(itr1 = relList->begin();itr1 != relList->end();itr1++){
					for(itr2=patternListRel->begin();itr2!=patternListRel->end();itr2++){
						if(itr1->first==*itr2){
							int patternIndex = *itr2;
							tempPatternList.push_back(patternIndex);

							pair<int,int> relPair = *itr1;
							tempRelList.push_back(relPair);
						}
					}
				}
			
			}
			//patternFirstRel = secondRel
			if((patternVar->getFirstDescendant()->getType()==secondRel->getType())&&(patternVar->getFirstDescendant()->getData()==secondRel->getData())){
				intersect2 = true;
				for(itr1 = relList->begin();itr1 != relList->end();itr1++){
					for(itr2=patternListRel->begin();itr2!=patternListRel->end();itr2++){
						if(itr1->second==*itr2){
							int patternIndex = *itr2;
							tempPatternList.push_back(patternIndex);

							pair<int,int> relPair = *itr1;
							tempRelList.push_back(relPair);
						}
					}
				}
			}
			// the intermediate result is narrowed down
			if(tempPatternList.size()>0){
				patternListRel = new INDEX_LIST(tempPatternList);
				relList = new RELATION_LIST(tempRelList);
				//cout<<"patternList size:"<<patternList->size();
				//cout<<"relList size1:"<<relList->size();
				//set bool result to true
				if((intersect1==true||intersect2==true)&&resultType==BOOL){
					returnList->push_back(1);
				}
			}else{
				//same variable appears in two clauses but no intersection found,
				//set bool result to false and returnList to null
				if(intersect1==true||intersect2==true){
					if(resultType==BOOL){
						returnList->push_back(0);
					}else{
						returnList = NULL;
					}
					return;
				}
			}

		}

		currentNode = resultNode;
		//based on result return type, add valid results in returnList

		if(resultType !=BOOL){
		
			while(currentNode->getRightSibling()!=NULL){

				currentNode = currentNode->getRightSibling();
				TYPE nodeType = currentNode->getType();
			
				if(nodeType == SUCHTHAT){
					firstRel = currentNode->getFirstDescendant()->getFirstDescendant();
					secondRel = firstRel->getRightSibling();
					RELATION_LIST::iterator itr;
					//desired result is found in relationList, in first field
					if(resultType== firstRel->getType()&&resultIndex ==firstRel->getData()){
						//cout<<"relList size: "<<relList->size();
						for(itr = relList->begin();itr!=relList->end();itr++){
							int addStmt = itr->first;
							returnList->push_back(addStmt);
						}
					}
					//desired result is found in relationList, in first field
					if(resultType== secondRel->getType()&&resultIndex ==secondRel->getData()){
						for(itr = (*relList).begin();itr!=(*relList).end();itr++){
							returnList->push_back(itr->second);
						}
					}
				}else if(nodeType == PATTERN){
					//get the node of assign/while in pattern tree
					firstRel = currentNode->getFirstDescendant();
					//cout<<"patternList size: "<<(*patternList).size();
					if(resultType==firstRel->getType()&&resultIndex==firstRel->getData()){
						INDEX_LIST::iterator itr;
						for(itr = patternListNode->begin();itr!=patternListNode->end();itr++){
							returnList->push_back(*itr);
						}
					}
					/*
					//TODO:: add variables
					if(resultType== firstRel->getFirstDescendant()->getType()&&resultIndex== firstRel->getFirstDescendant()->getData()){
						INDEX_LIST::iterator itr;
						for(itr = patternListRel->begin();itr!=patternListRel->end();itr++){
							//cout<<"returnInex:"<<*itr;
							returnList->push_back(*itr);
						}
					} comment
 				//	cout<<"returnList size: "<<(*returnList).size();
				}

			}
		}

		if((*returnList).size()>0){
			(*returnList).sort();
			(*returnList).unique();
		}else{
			//returnList is NULL, where the desired result variable is not in the intemediate results 
			//or no clauses appear in the query
			//return all the valid results of the resultType
			if(resultType ==BOOL){
				returnList->push_back(1);
			}else{
				TYPE resultVarType = qrTable->find(resultIndex)->second;
				//get the max stmtNo first
				int stmtNo = pkb->getProcedure(1)->getEndProgLine();

				if(resultVarType ==STATEMENT){
					for(int i=1; i<=stmtNo;i++){
						returnList->push_back(i);
					}
				} 
				if(resultVarType== ASSIGNMENT){
					for(int i=1; i<=stmtNo;i++){
						AST_LIST * stmtTree;
						stmtTree = pkb->getASTBy(i);
						AST_LIST::iterator itr;

						for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
							if((*itr)->getRootType()==ASSIGNMENT){
								returnList->push_back(i);
							} 
						}
					}
				}

				if(resultVarType == WHILE){
					for(int i=1; i<=stmtNo;i++){
						AST_LIST * stmtTree;
						stmtTree = pkb->getASTBy(i);
						AST_LIST::iterator itr;

						for(itr=stmtTree->begin();itr!=stmtTree->end();itr++){
							if((*itr)->getRootType()==WHILE){
								returnList->push_back(i);
							} 
						}
					}
				} 
				if(resultVarType== VARIABLE){
					int varSize = pkb->getVarTableSize();
					for(int i =1;i<=varSize;i++){
						returnList->push_back(i);
					}
				}
				if(resultVarType==PROCEDURE){
					int procSize = pkb->getProceTableSize();
					for(int i =1;i<=procSize;i++){
						returnList->push_back(i);
					}
				}
				/*	if(resultVarType ==PROG_LINE){
				for(int i=1; i<=stmtNo;i++){
				returnList->push_back(i);
				}
				}comment
			}
		}
	}else{
		if(resultType == BOOL){ 
			returnList->push_back(0);
		}else{
			returnList = NULL;
		}
	}
}


RAWDATA* QueryEvaluator::getResult(){
	//take care of bool case
	if(resultType ==BOOL) {
		rawData = new RAWDATA(BOOL,returnList);
		return rawData;
	}

	TYPE returnType;
	QUERYTABLE::iterator itr;
	//	cout << this->qrTable->size() << "\n";

	if(this->qrTable->size() != 0) 
	{
		itr= qrTable->find(resultIndex);

		//	cout<< "returnType: "<< itr->second<<"\n";
		returnType = itr->second;
		//cout << "SIZE OF RESULT (EVALUATOR): " << returnList->size() << "\n";
		rawData = new RAWDATA(returnType,returnList);
		return rawData;
	}
}

RELATION_LIST * QueryEvaluator::suchThat(QTREE* suchThatTree){

	QTREE * currentNode;
	currentNode = suchThatTree->getFirstDescendant();
	TYPE relType;
	QTREE * firstRel;
	QTREE * secondRel;
	RELATION_LIST * relList = new RELATION_LIST;
	RELATION_LIST tmpList;

	//at the level of relationships
	while(currentNode!=NULL){
		relType= currentNode->getType();
		firstRel = currentNode->getFirstDescendant();
		secondRel = firstRel->getRightSibling();

		//cout << "reltype: " << relType << "\n";
		//cout << "firstrel: " << firstRel->getType() << "\n";
		//cout << "secondrel: " << secondRel->getData() << "\n";

		//both rel vars are known
		if((firstRel->getType()!=QUERYVAR&&firstRel->getType()!=ANY)&&(secondRel->getType()!=QUERYVAR&&secondRel->getType()!=ANY)){
			if(relType == PARENT){

				if(pkb->isParent(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType == PARENTST){

				if(SuchThat::getIsParentStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==FOLLOWS){
				if(SuchThat::getIsFollowsResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==FOLLOWST){
				if(SuchThat::getIsFollowsStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==MODIFIES){
				if(SuchThat::getIsModifiesResult(STATEMENT,firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==USES){
				if(SuchThat::getIsUsesResult(STATEMENT,firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}

		}
		//first is known ,second is unknown
		if((firstRel->getType()!=QUERYVAR&&firstRel->getType()!=ANY)&&(secondRel->getType()==QUERYVAR||secondRel->getType()==ANY)){
			//check if the unknown is from qrVarTable
			TYPE secondType;
			if(secondRel->getType()==QUERYVAR){
				QUERYTABLE::iterator itr;
				itr = qrTable->find(secondRel->getData());
				secondType = itr->second;
			}else{
				secondType = secondRel->getType();
			}

			//parent, narrow down second
			if(relType == PARENT){

				tmpList = SuchThat::getParentResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//parent*, narrow down second

			if(relType == PARENTST){
				tmpList = SuchThat::getParentStarResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//follow, narrow down second

			if(relType==FOLLOWS){
				tmpList = SuchThat::getFollowsResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//follow* narrow down

			if(relType==FOLLOWST){
				tmpList = SuchThat::getFollowsStarResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//modifies, no need to narrow down

			if(relType==MODIFIES){
				//cout<<"First Rel: "<< firstRel->getType();
				//cout<<"First Data: "<<firstRel->getData();
				tmpList = SuchThat::getModifiesResult(firstRel->getType(),firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//uses, now need to narrow down
			if(relType==USES){
				tmpList = SuchThat::getUsesResult(firstRel->getType(),firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			//	RELATION_LIST::iterator itr;
		}
		//first is unknown, second is known
		if((firstRel->getType()==QUERYVAR||firstRel->getType()==ANY)&&(secondRel->getType()!=QUERYVAR&&secondRel->getType()!=ANY)){
			TYPE firstType;
			if(firstRel->getType()==QUERYVAR){
				QUERYTABLE::iterator itr;
				itr = qrTable->find(firstRel->getData());
				firstType = itr->second;
			}else{
				firstType = firstRel->getType();
			}

			//parent, narrow down
			if(relType == PARENT){
				tmpList = SuchThat::getParentResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			//parent*, narrow down
			if(relType == PARENTST){
				tmpList = SuchThat::getParentStarResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			//follow, narrow down
			if(relType==FOLLOWS){ 
				//cout<<"firstType: "<< firstType;
				//cout<<"secondRel:"<< secondRel->getType();
				//cout<<"secondIndexL: "<<secondRel->getData();

				tmpList = SuchThat::getFollowsResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);

				//relList = &SuchThat::getFollowsResult(firstType,0,secondRel->getData());
				//cout << "\nsize: " << relList->size() << "\n";
			}
			//follow* narrow down
			if(relType==FOLLOWST){
				//	cout<<"First Rel: "<< firstType;
				//		cout<<"Second Data: "<<secondRel->getData();
				tmpList = SuchThat::getFollowsStarResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			//modifies, narrow down
			if(relType==MODIFIES){
			//	cout<<"First Rel: "<< firstType;
			//	cout<<"Second Data: "<<secondRel->getData();
				tmpList = SuchThat::getModifiesResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			//uses, narrow down
			if(relType==USES){
				//cout<<"First Rel: "<< firstType;
				//cout<<"Second Data: "<<secondRel->getData();
				tmpList = SuchThat::getUsesResult(firstType,0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
		}
		//first is unkown, second is unknown
		// Seok Min: I think you make a copy-paste mistake here previously. let me know if i interpret wrongly
		// Original line: ...(secondRel->getType()!=QUERYVAR..
		if((firstRel->getType()==QUERYVAR||firstRel->getType()==ANY)
			&&(secondRel->getType()==QUERYVAR||secondRel->getType()==ANY)){
				//convert to the correct type first
				TYPE firstType;
				if(firstRel->getType()==QUERYVAR){
					QUERYTABLE::iterator itr;
					itr = qrTable->find(firstRel->getData());
					firstType = itr->second;

				//	cout<<"firstType: "<<firstType;
				}else{
					firstType = firstRel->getType();
				}

				TYPE secondType;
				if(secondRel->getType()==QUERYVAR){
					QUERYTABLE::iterator itr;
					itr = qrTable->find(secondRel->getData());
					secondType = itr->second;
				//	cout<<"secondType: "<<secondType;
				}else{
					secondType = secondRel->getType();
				}

				//parent, narrow down both
				if(relType == PARENT){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = SuchThat::getParentResult(firstType,secondType);
						iterateAndStore(relList, tmpList);
					}
				}
				//parent*, narrow down both
				if(relType == PARENTST){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&(firstRel->getData()==secondRel->getData())){
						relList = NULL;
					}else{
						tmpList = SuchThat::getParentStarResult(firstType,secondType);
						iterateAndStore(relList, tmpList);
					}

				}
				//follow, narrow down both
				if(relType==FOLLOWS){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = SuchThat::getFollowsResult(firstType,secondType);
						iterateAndStore(relList, tmpList);
					}

				}
				//follow* narrow down both
				if(relType==FOLLOWST){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = SuchThat::getFollowsStarResult(firstType,secondType);
						iterateAndStore(relList, tmpList);
					}

				}
				//modifies, narrow down first rel
				if(relType==MODIFIES){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;

					tmpList = SuchThat::getModifiesResult(firstType,0,0);
					iterateAndStore(relList, tmpList);
				}
				//uses, narrow down first rel
				if(relType==USES){
					tmpList = SuchThat::getUsesResult(firstType,0,0);
					iterateAndStore(relList, tmpList);
				}
		}

		//increment the currentNode pointer
		currentNode = currentNode->getRightSibling();

	}

		//cout<<relList;
		return relList;
	
}
INDEX_LIST* QueryEvaluator::patternRel(QTREE* patternTree){

	QTREE * qrVarNode = patternTree->getFirstDescendant();
	TYPE qrVarType;
	qrVarType = qrTable->find(qrVarNode->getData())->second;

	//cout<<"qrVarType: "<<qrVarType;
	//cout<<"qrVarIndex: "<<qrVarNode->getData();

	QTREE * firstNode; 
	QTREE * secondNode; 
	INDEX_LIST* patternList = new INDEX_LIST;

	if(qrVarType==ASSIGNMENT){
		firstNode = qrVarNode->getFirstDescendant();
		secondNode = firstNode->getRightSibling();
		//pattern (v,...)
		if(firstNode->getType()==QUERYVAR){
			//get pattern a(_,...)
			if(secondNode->getType()==ANY){
			//	cout<<firstNode->getData();
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,NULL);
			}else{
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,secondNode);
			}
			//get all v with the pattern "..."
			if(patternList!=NULL){
				INDEX_LIST* temList;
				temList = new INDEX_LIST;

				INDEX_LIST::iterator itr;
				for(itr=patternList->begin();itr!=patternList->end();itr++){
					MODIFIES_LIST modList = pkb->getModifies(ASSIGNMENT,*itr,NULL);
					pair<int,int> modPair = *modList.begin();

					int varIndex = modPair.second;
				//	cout<<"varIndex: "<<varIndex;
					temList->push_back(varIndex);
				}
				//replace patternList with temList
				if(temList->size()>0){
					patternList = temList;
				}else{
					patternList = NULL;
				}
			}


		}else{
		//pattern("x",..)
		if(firstNode->getType()==VARIABLE){
			if(secondNode->getType()==ANY){
			//	cout<<firstNode->getData();
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,firstNode->getData(),NULL);
			}else{
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,firstNode->getData(),secondNode);
			}
		}else{//pattern(_,...)
			if(secondNode->getType()==ANY){
				
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,NULL);
			}else{
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,secondNode);
			}

		}
		
		}
	}
	if(qrVarType==WHILE){
		firstNode = qrVarNode->getFirstDescendant();
		if(firstNode->getType()==VARIABLE){
			patternList = patProcessor.getPatternStmt(WHILE,firstNode->getData(),NULL);
		}else{
			patternList = patProcessor.getPatternStmt(WHILE,NULL,NULL);
		}
	}

	//if((*patternList).size()>0)
	return patternList;

	//return NULL;

}

INDEX_LIST* QueryEvaluator::patternNode(QTREE* patternTree){

	QTREE * qrVarNode = patternTree->getFirstDescendant();
	TYPE qrVarType;
	qrVarType = qrTable->find(qrVarNode->getData())->second;

	//cout<<"qrVarType: "<<qrVarType;
	//cout<<"qrVarIndex: "<<qrVarNode->getData();

	QTREE * firstNode; 
	QTREE * secondNode; 
	INDEX_LIST* patternList = new INDEX_LIST;

	if(qrVarType==ASSIGNMENT){
		firstNode = qrVarNode->getFirstDescendant();
		secondNode = firstNode->getRightSibling();
		
		//pattern("x",..)
		if(firstNode->getType()==VARIABLE){
			if(secondNode->getType()==ANY){
			//	cout<<firstNode->getData();
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,firstNode->getData(),NULL);
			}else{
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,firstNode->getData(),secondNode);
			}
		}else{//pattern(_,...)
			if(secondNode->getType()==ANY){
				
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,NULL);
			}else{
				patternList = patProcessor.getPatternStmt(ASSIGNMENT,NULL,secondNode);
			}

		}
		
		
	}
	if(qrVarType==WHILE){
		firstNode = qrVarNode->getFirstDescendant();
		if(firstNode->getType()==VARIABLE){
			patternList = patProcessor.getPatternStmt(WHILE,firstNode->getData(),NULL);
		}else{
			patternList = patProcessor.getPatternStmt(WHILE,NULL,NULL);
		}
	}

	//if((*patternList).size()>0)
	return patternList;

	//return NULL;

}

*/