#include "SuchThatClause.h"


SuchThatClause::SuchThatClause(PKB* pkb,QUERYTABLE* qrTable,DesignExtractor * extractor)
{
	this->pkb = pkb;
	this->qrTable = qrTable;
	this->extractor = extractor;
}


SuchThatClause::~SuchThatClause(void)
{
}


void SuchThatClause::iterateAndStore(RELATION_LIST *result, RELATION_LIST list){
	if(list.empty()){
			result=NULL;
		}else{
			    RELATION_LIST::iterator itr = list.begin();
				while(itr!=list.end()){
					result->push_back(*itr);
					itr++;
				}	
		}
	
}
void SuchThatClause::filterResult(RELATION_LIST * result,RELATION_LIST list,TYPE cond1,TYPE cond2){
	if(list.empty()){
			result=NULL;
		}else{
			    RELATION_LIST::iterator itr = list.begin();
				while(itr!=list.end()){
					bool firstCond = cond1==ANY||cond1==STATEMENT||cond1==STMT_LIST||cond1==PROGLINE||extractor->isStatementTypeOf(cond1,itr->first);
					bool secondCond = cond2==ANY||cond2 ==STATEMENT||cond2==STMT_LIST||cond2==PROGLINE||extractor->isStatementTypeOf(cond2,itr->second);
					if(firstCond&&secondCond){
						result->push_back(*itr);
					}
					itr++;
				}	
		}
}
RELATION_LIST* SuchThatClause::evaluateSuchThatTree(QTREE* suchThatTree){
	this->suchThatTree = suchThatTree;
	

	return SuchThatClause::evaluateSuchThat();
}
RELATION_LIST* SuchThatClause::evaluateSuchThat(){
	
	QTREE * currentNode;
	currentNode = suchThatTree->getFirstDescendant();
	TYPE relType;
	QTREE * firstRel;
	QTREE * secondRel;
	RELATION_LIST * relList = new RELATION_LIST;
	RELATION_LIST tmpList;
	int unknownValue;

	//get the relation type, firstRel and secondRel
	relType= currentNode->getType();
	firstRel = currentNode->getFirstDescendant();
	secondRel = firstRel->getRightSibling();

		//cout << "reltype: " << relType << "\n";
		//cout << "firstrel: " << firstRel->getType() << "\n";
		//cout << "secondrel: " << secondRel->getData() << "\n";

	//both rel vars are known, QUERYVAR: unknown declared in query; Any: underscore "_"
	if((firstRel->getType()!=QUERYVAR&&firstRel->getType()!=ANY)&&(secondRel->getType()!=QUERYVAR&&secondRel->getType()!=ANY)){
		if(relType == PARENT){
			if(pkb->isParent(firstRel->getData(),secondRel->getData())){
				relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType == PARENTST){

				if(extractor->getIsParentStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==FOLLOWS){
				if(extractor->getIsFollowsResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==FOLLOWST){
				if(extractor->getIsFollowsStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==MODIFIES){
				TYPE modType;
				if(firstRel->getType()==PROCEDURE){
					modType = PROCEDURE;
				}else{
					modType = STATEMENT;
				}
				if(extractor->getIsModifiesResult(modType,firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==USES){
				TYPE useType;
				if(firstRel->getType()==PROCEDURE){
					useType = PROCEDURE;
				}else{
					useType = STATEMENT;
				}
				if(extractor->getIsUsesResult(useType,firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==NEXT){
				if(extractor->isNextResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==NEXTST){
				if(extractor->isNextStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==CALL){
				PROC_NAME callerName = pkb->getProcedure(firstRel->getData())->getProcName();
				PROC_NAME calleeName = pkb->getProcedure(secondRel->getData())->getProcName();
				if(extractor->getIsCallResult(callerName,calleeName)){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==CALLST){
				PROC_NAME callerName = pkb->getProcedure(firstRel->getData())->getProcName();
				PROC_NAME calleeName = pkb->getProcedure(secondRel->getData())->getProcName();
				if(extractor->getIsCallStarResult(callerName,calleeName)){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==AFFECTS){
				if(extractor->getIsAffectResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==AFFECTST){
				if(extractor->getIsAffectStarResult(firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==SIBLING){
				if(extractor->getIsSiblingResult(firstRel->getType(),secondRel->getType(),firstRel->getData(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==CONTAINS){
				if(extractor->getIsContainResult(firstRel->getType(),firstRel->getData(),secondRel->getType(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}
			if(relType==CONTAINST){
				if(extractor->getIsContainStarResult(firstRel->getType(),firstRel->getData(),secondRel->getType(),secondRel->getData())){
					relList->push_back(pair<int,int>(firstRel->getData(),secondRel->getData()));
				}
			}

		}
		//first is known ,second is unknown
		if((firstRel->getType()!=QUERYVAR&&firstRel->getType()!=ANY)&&(secondRel->getType()==QUERYVAR||secondRel->getType()==ANY)){
			//check if the unknown is from qrVarTable
			TYPE firstType = firstRel->getType();
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

				tmpList = extractor->getParentResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList,tmpList);
			}
			//parent*, narrow down second

			if(relType == PARENTST){
				tmpList = extractor->getParentStarResult(secondType,firstRel->getData(),0);
				iterateAndStore(relList,tmpList);
			}
			//follow, narrow down second

			if(relType==FOLLOWS){
				tmpList = extractor->getFollowsResult(secondType,firstRel->getData(),0);
				filterResult(relList,tmpList,firstType,secondType);
			}
			//follow* narrow down

			if(relType==FOLLOWST){
				tmpList = extractor->getFollowsStarResult(secondType,firstRel->getData(),0);
				filterResult(relList,tmpList,firstType,secondType);
			}
			//modifies, no need to narrow down

			if(relType==MODIFIES){
				//cout<<"First Rel: "<< firstRel->getType();
				//cout<<"First Data: "<<firstRel->getData();
				tmpList = extractor->getModifiesResult(firstRel->getType(),firstRel->getData(),0);
				iterateAndStore(relList,tmpList);
			}
			//uses, now need to narrow down
			if(relType==USES){
				tmpList = extractor->getUsesResult(firstRel->getType(),firstRel->getData(),0);
				iterateAndStore(relList,tmpList);
			}
			if(relType==NEXT){
				tmpList = extractor->getNextResult(firstRel->getData(),0);
				filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==NEXTST){
				tmpList = extractor->getNextStarResult(firstRel->getData(),0);
				filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==CALL){
				PROC_NAME callerName = pkb->getProcedure(firstRel->getData())->getProcName();
				INDEX callerIndex,calleeIndex;
				CALL_LIST callResult=extractor->getCallResult(callerName," ");

				CALL_LIST::iterator callPair;
				for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
					callerIndex = pkb->getProcIndex(callPair->first);
					calleeIndex = pkb->getProcIndex(callPair->second);
					tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
				}
				iterateAndStore(relList, tmpList);
			}
			if(relType==CALLST){
				PROC_NAME callerName = pkb->getProcedure(firstRel->getData())->getProcName();
				INDEX callerIndex,calleeIndex;
				CALL_LIST callResult=extractor->getCallStarResult(callerName," ");

				CALL_LIST::iterator callPair;
				for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
					callerIndex = pkb->getProcIndex(callPair->first);
					calleeIndex = pkb->getProcIndex(callPair->second);
					tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
				}
				iterateAndStore(relList, tmpList);
			}
			if(relType==AFFECTS){
				tmpList = extractor->getAffectResult(firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			if(relType==AFFECTST){
				tmpList = extractor->getAffectStarResult(firstRel->getData(),0);
				iterateAndStore(relList, tmpList);
			}
			if(relType==SIBLING){
				tmpList = extractor->getSiblingResult(firstType,secondType,firstRel->getData(),-2);
				//filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==CONTAINS){
				if(secondType==PLUS||secondType==MINUS||secondType==MULTIPLY||secondType==STMT_LIST){
					unknownValue = -1;
				}else{
					unknownValue = 0;
				}
				tmpList = extractor->getContainResult(firstType,firstRel->getData(),secondType,unknownValue);
				iterateAndStore(relList,tmpList);
			}
			if(relType==CONTAINST){
				if(secondType==PLUS||secondType==MINUS||secondType==MULTIPLY||secondType==STMT_LIST){
					unknownValue = -1;
				}else{
					unknownValue = 0;
				}
				tmpList = extractor->getContainStarResult(firstType,firstRel->getData(),secondType,unknownValue);
				iterateAndStore(relList,tmpList);
			}
			
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

			TYPE secondType = secondRel->getType();

			//parent, narrow down
			if(relType == PARENT){
				tmpList = extractor->getParentResult(firstType,0,secondRel->getData());
				iterateAndStore(relList,tmpList);
			}
			//parent*, narrow down
			if(relType == PARENTST){
				tmpList = extractor->getParentStarResult(firstType,0,secondRel->getData());
				iterateAndStore(relList,tmpList);
			}
			//follow, narrow down
			if(relType==FOLLOWS){ 
				tmpList = extractor->getFollowsResult(firstType,0,secondRel->getData());
				filterResult(relList,tmpList,firstType,secondType);
			}
			//follow* narrow down
			if(relType==FOLLOWST){
				tmpList = extractor->getFollowsStarResult(firstType,0,secondRel->getData());
				filterResult(relList,tmpList,firstType,secondType);
			}
			//modifies, narrow down
			if(relType==MODIFIES){
			//	cout<<"First Rel: "<< firstType;
			//	cout<<"Second Data: "<<secondRel->getData();
				tmpList = extractor->getModifiesResult(firstType,0,secondRel->getData());
				iterateAndStore(relList,tmpList);
			}
			//uses, narrow down
			if(relType==USES){
				//cout<<"First Rel: "<< firstType;
				//cout<<"Second Data: "<<secondRel->getData();
				tmpList = extractor->getUsesResult(firstType,0,secondRel->getData());
				iterateAndStore(relList,tmpList);
			}
			if(relType==NEXT){
				tmpList = extractor->getNextResult(0,secondRel->getData());
				filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==NEXTST){
				tmpList = extractor->getNextStarResult(0,secondRel->getData());
				filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==CALL){
				PROC_NAME calleeName = pkb->getProcedure(secondRel->getData())->getProcName();
				INDEX callerIndex,calleeIndex;
				CALL_LIST callResult=extractor->getCallResult(" ",calleeName);

				CALL_LIST::iterator callPair;
				for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
					callerIndex = pkb->getProcIndex(callPair->first);
					calleeIndex = pkb->getProcIndex(callPair->second);
					tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
				}
				iterateAndStore(relList, tmpList);
			}
			if(relType==CALLST){
				PROC_NAME calleeName = pkb->getProcedure(secondRel->getData())->getProcName();
				INDEX callerIndex,calleeIndex;
				CALL_LIST callResult=extractor->getCallStarResult(" ",calleeName);

				CALL_LIST::iterator callPair;
				for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
					callerIndex = pkb->getProcIndex(callPair->first);
					calleeIndex = pkb->getProcIndex(callPair->second);
					tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
				}
				iterateAndStore(relList, tmpList);
			}
			if(relType==AFFECTS){
				tmpList = extractor->getAffectResult(0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			if(relType==AFFECTST){
				tmpList = extractor->getAffectStarResult(0,secondRel->getData());
				iterateAndStore(relList, tmpList);
			}
			if(relType==SIBLING){
				tmpList = extractor->getSiblingResult(firstType,secondType,-2,secondRel->getData());
				filterResult(relList,tmpList,firstType,secondType);
			}
			if(relType==CONTAINS){
				if(firstType==PLUS||firstType==MINUS||firstType==MULTIPLY||firstType==STMT_LIST){
					unknownValue = -1;
				}else{
					unknownValue = 0;
				}
				tmpList = extractor->getContainResult(firstType,unknownValue,secondType,secondRel->getData());
				iterateAndStore(relList,tmpList);
			}
			if(relType==CONTAINST){
				if(firstType==PLUS||firstType==MINUS||firstType==MULTIPLY||firstType==STMT_LIST){
					unknownValue = -1;
				}else{
					unknownValue = 0;
				}
				tmpList = extractor->getContainStarResult(firstType,unknownValue,secondType,secondRel->getData());
				iterateAndStore(relList,tmpList);
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
						tmpList = extractor->getParentResult(firstType,secondType);
						iterateAndStore(relList,tmpList);
					}
				}
				//parent*, narrow down both
				if(relType == PARENTST){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&(firstRel->getData()==secondRel->getData())){
						relList = NULL;
					}else{
						tmpList = extractor->getParentStarResult(firstType,secondType);
						iterateAndStore(relList,tmpList);
					}

				}
				//follow, narrow down both
				if(relType==FOLLOWS){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = extractor->getFollowsResult(firstType,secondType);
						filterResult(relList,tmpList,firstType,secondType);
					}

				}
				//follow* narrow down both
				if(relType==FOLLOWST){
					//cout<<"firstType: "<<firstType;
					//cout<<"secondType: "<<secondType;
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = extractor->getFollowsStarResult(firstType,secondType);
						filterResult(relList,tmpList,firstType,secondType);
					}

				}
				//modifies, narrow down first rel
				if(relType==MODIFIES){
					tmpList = extractor->getModifiesResult(firstType,0,0);
					iterateAndStore(relList,tmpList);
				}
				//uses, narrow down first rel
				if(relType==USES){
					tmpList = extractor->getUsesResult(firstType,0,0);
					iterateAndStore(relList,tmpList);
				}
				if(relType==NEXT){
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						relList = NULL;
					}else{
						tmpList = extractor->getNextResult(0,0);
						filterResult(relList,tmpList,firstType,secondType);
					}
				}
				if(relType==NEXTST){
					tmpList = extractor->getNextStarResult(0,0);
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						for(RELATION_LIST::iterator tmpItr = tmpList.begin();tmpItr!=tmpList.end();tmpItr++){
							if(tmpItr->first==tmpItr->second) relList->push_back(pair<int,int>(tmpItr->first,tmpItr->second));
						}
					}else{
						filterResult(relList,tmpList,firstType,secondType);
					}
				}
				if(relType==CALL){
				
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						/*for(RELATION_LIST::iterator tmpItr = tmpList.begin();tmpItr!=tmpList.end();tmpItr++){
							if(tmpItr->first==tmpItr->second) relList->push_back(pair<int,int>(tmpItr->first,tmpItr->second));
						}*/
						relList = NULL;
					}else{
						INDEX callerIndex,calleeIndex;
						CALL_LIST callResult=extractor->getCallResult(" "," ");

						CALL_LIST::iterator callPair;
						for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
							callerIndex = pkb->getProcIndex(callPair->first);
							calleeIndex = pkb->getProcIndex(callPair->second);
							tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
						}
						iterateAndStore(relList, tmpList);
					}
				}
				if(relType==CALLST){
					INDEX callerIndex,calleeIndex;
					CALL_LIST callResult=extractor->getCallStarResult(" "," ");

					CALL_LIST::iterator callPair;
					for(callPair = callResult.begin();callPair!=callResult.end();callPair++){
						callerIndex = pkb->getProcIndex(callPair->first);
						calleeIndex = pkb->getProcIndex(callPair->second);
						tmpList.push_back(pair<int,int>(callerIndex,calleeIndex));
					}
					if((firstRel->getType()==QUERYVAR&&secondRel->getType()==QUERYVAR)&&firstRel->getData()==secondRel->getData()){
						for(RELATION_LIST::iterator tmpItr = tmpList.begin();tmpItr!=tmpList.end();tmpItr++){
							if(tmpItr->first==tmpItr->second) relList->push_back(pair<int,int>(tmpItr->first,tmpItr->second));
						}
					}else{
						iterateAndStore(relList, tmpList);
					}
				}
				if(relType==AFFECTS){
					tmpList = extractor->getAffectResult(0,0);
					iterateAndStore(relList, tmpList);
				}
				if(relType==AFFECTST){
					tmpList = extractor->getAffectStarResult(0,0);
					iterateAndStore(relList, tmpList);
				}
				if(relType==SIBLING){
					tmpList = extractor->getSiblingResult(firstType,secondType,-2,-2);
					filterResult(relList,tmpList,firstType,secondType);
				}
				if(relType==CONTAINS){
					int unknownValue1,unknownValue2;
					if(firstType==PLUS||firstType==MINUS||firstType==MULTIPLY||firstType==STMT_LIST){
						unknownValue1 = -1;
					}else{
						unknownValue1 = 0;
					}
					if(secondType==PLUS||secondType==MINUS||secondType==MULTIPLY||secondType==STMT_LIST){
						unknownValue2 = -1;
					}else{
						unknownValue2 = 0;
					}
					tmpList = extractor->getContainResult(firstType,unknownValue1,secondType,unknownValue2);
					iterateAndStore(relList,tmpList);
				}
				if(relType==CONTAINST){
					int unknownValue1,unknownValue2;
					if(firstType==PLUS||firstType==MINUS||firstType==MULTIPLY||firstType==STMT_LIST){
						unknownValue1 = -1;
					}else{
						unknownValue1 = 0;
					}
					if(secondType==PLUS||secondType==MINUS||secondType==MULTIPLY||secondType==STMT_LIST){
						unknownValue2 = -1;
					}else{
						unknownValue2 = 0;
					}
					tmpList = extractor->getContainStarResult(firstType,unknownValue1,secondType,unknownValue2);
					iterateAndStore(relList,tmpList);
				}

		}

	

		//cout<<relList;
		return relList;

}