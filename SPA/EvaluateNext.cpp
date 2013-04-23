#include "EvaluateNext.h"


PKB * EvaluateNext::pkb = NULL;
CFG * EvaluateNext::cfg = NULL;
vector<PROG_LINE> EvaluateNext::cfgWhileKeepers;


EvaluateNext::EvaluateNext(void)
{
	
}


EvaluateNext::~EvaluateNext(void)
{
}
NEXT_LIST EvaluateNext::getAllPaths(PROG_LINE p1, PROG_LINE p2)
{
	return cfg->getAllPaths(p1,p2);
}
void EvaluateNext::createCFG(){
	int size = pkb->getMaxProgLine();
	cfg = new CFG(size);

	// traverse AST and create the CFG
	AST* cAST = pkb->getRootAST();
	AST* tmp = cAST;
	buildCFG(tmp);
}


NEXT_LIST EvaluateNext::getNextResult(PROG_LINE p1, PROG_LINE p2){
	int size = pkb->getMaxProgLine();
	NEXT_LIST result;
	if(p1 > size || p2 > size || p1 < 0 || p2 < 0)
	{
		return result;
	}else
		return cfg->getNext(p1, p2);
}

bool EvaluateNext::isNextResult(PROG_LINE p1, PROG_LINE p2){
	int size = pkb->getMaxProgLine();

	if(p1 > size || p2 > size || p1 < 0 || p2 < 0)
		return false;
	else
		return cfg->isNext(p1, p2);
}

bool EvaluateNext::isNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	int size = pkb->getMaxProgLine();

	if(p1 > size || p2 > size || p1 <= 0 || p2 <=0)
		return false;

	list<PROG_LINE> tmp = cfg->getAllProgLines(p1, p2);
	list<PROG_LINE>::const_iterator tmp_end=tmp.cend();
	if(tmp.size() != 0){
		list<PROG_LINE>::const_iterator itr = tmp.cbegin();
		while(itr!=tmp_end){
			if(*itr==p2)
				return true;

			itr++;
		}
	}
	return false;
}

NEXT_LIST EvaluateNext::getNextStarResult(PROG_LINE p1, PROG_LINE p2)
{
	 
	NEXT_LIST result;
	int size = pkb->getMaxProgLine();

	// Next*(n1, n2) --> findAll
	if(p1 == 0 && p2 == 0){
		for(int i = 1; i <=size; i++){
			
			list<PROG_LINE> tmp = cfg->getAllProgLines(i, i);
			list<PROG_LINE>::const_iterator tmp_end=tmp.cend();

			list<PROG_LINE>::const_iterator itr = tmp.cbegin();
			while(itr!=tmp_end)
			{
				pair<PROG_LINE, PROG_LINE> tPair;
				tPair.first = i;
				tPair.second = *itr;
				
				result.push_back(tPair);
				
				itr++;
			}
		}
	}
	//Next*(n, n), Next*(1,n1), Next*(n1,2) --> findAll
	else if(p1 == p2 || (p1 != 0 && p2 == 0) || (p1 == 0 && p2 != 0) || (p1 != 0 && p2 != 0)){
		
		list<PROG_LINE> tmp = cfg->getAllProgLines(p1, p2);
		
		list<PROG_LINE>::const_iterator itr = tmp.cbegin();
		list<PROG_LINE>::const_iterator tmp_end=tmp.cend();
		while(itr!=tmp_end)
		{
			pair<PROG_LINE, PROG_LINE> tPair;
			if(p1 != 0) {
				tPair.first = p1;
				tPair.second = *itr;
			}
			else if(p1 == 0) {
				tPair.first = *itr;
				tPair.second = p2;
			}
				
			result.push_back(tPair);
				
			itr++;
		}
	}
	return result;
}

AST* EvaluateNext::buildCFG(AST * node)
{
	AST* tmp = node;
	AST* containerTmp, *lastNode, *grandparent;
	vector<PROG_LINE>::iterator itr2;

	while(tmp)
	{
		switch(pkb->getType(tmp)){

		case ASSIGNMENT:
			if(tmp->getRightSibling()){
				EvaluateNext::cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}
			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;

		case WHILE:
			EvaluateNext::cfgWhileKeepers.push_back(pkb->getProgLine(tmp));

			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}

			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());

			if(pkb->getType(lastNode) != IF)
				cfg->addEdge(pkb->getProgLine(lastNode), EvaluateNext::cfgWhileKeepers.back());


			// change to the correct last node
			if(lastNode->getAncestor() && pkb->getType(lastNode->getAncestor()->getAncestor()) != IF)
				lastNode =lastNode->getAncestor()->getAncestor();


			EvaluateNext::cfgWhileKeepers.pop_back();

			tmp = tmp->getRightSibling();
			break;
		case IF:

			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling());
			if(EvaluateNext::cfgWhileKeepers.size()!= 0 && !tmp->getRightSibling())
				cfg->addEdge(pkb->getProgLine(lastNode), EvaluateNext::cfgWhileKeepers.back());

			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}

			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getFirstDescendant()));


			// else
			lastNode = buildCFG(tmp->getFirstDescendant()->getRightSibling()->getRightSibling());
			if(EvaluateNext::cfgWhileKeepers.size() != 0 && !tmp->getRightSibling())
				cfg->addEdge(pkb->getProgLine(lastNode), EvaluateNext::cfgWhileKeepers.back());


			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(lastNode), pkb->getProgLine(tmp->getRightSibling()));
			}


			cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getFirstDescendant()->getRightSibling()->getRightSibling()->getFirstDescendant()));

			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case CALL:
			if(tmp->getRightSibling()){
				cfg->addEdge(pkb->getProgLine(tmp), pkb->getProgLine(tmp->getRightSibling()));
			}

			lastNode = tmp;
			tmp = tmp->getRightSibling();
			break;
		case PROCEDURE:
			lastNode = buildCFG(tmp->getFirstDescendant());
			tmp = tmp->getRightSibling();
			break;
		case STMT_LIST:
			tmp = tmp->getFirstDescendant();
			break;
		}
	}

	return lastNode;
}
