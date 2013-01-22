#include "SuchThatTree.h"
#include "SuchThat.h"

SuchThatTree::SuchThatTree(PKB* pkb)
{
	this->pkb = pkb;
}


SuchThatTree::~SuchThatTree(void)
{
}


void SuchThatTree::iterateAndStore(RELATION_LIST *result, RELATION_LIST list){
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
RELATION_LIST* SuchThatTree::evaluateSuchThatTree(QTREE* suchThatTree,QUERYTABLE* qrTable){
	this->suchThatTree = suchThatTree;
	this->qrTable = qrTable;

	return SuchThatTree::evaluateSuchThat();
}
RELATION_LIST* SuchThatTree::evaluateSuchThat(){
	
	QTREE * currentNode;
	currentNode = suchThatTree->getFirstDescendant();
	TYPE relType;
	QTREE * firstRel;
	QTREE * secondRel;
	RELATION_LIST * relList = new RELATION_LIST;
	RELATION_LIST tmpList;

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

	

		//cout<<relList;
		return relList;

}