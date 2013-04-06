#pragma once
#include "QueryPreprocessor.h"



QueryPreprocessor::QueryPreprocessor(PKB* pkb){
	tokens			= new vector<TOKEN>;

	paramTable		= new hash_map<int,string>;
	qVarTable		= new hash_map<int,TYPE>;
	dVarTable		= new hash_map<string,qVar>;

	this->pkb		= pkb;

	compulsoryOne	= "+";
	optional		= "*";
	or				= "|";		
	underscore		= "_";
	hash			= "#";
	invComma		= "\"";
	letter			= "a-zA-Z";
	digit			= "0-9";
	plus			= "\\+";
	minus			= "\\-";
	times			= "\\*";
	op				= "["+plus+minus+times+"]";
	integer			= "["+digit+"]"+compulsoryOne;
	ident			= "["+letter+"]"+compulsoryOne+"["+letter+digit+hash+"]"+optional;
	synonym			= ident;
	rel				= "Uses|Modifies|Follows\\*|Follows|Parent\\*|Parent|Affects\\*|Affects|Calls\\*|Calls|Next\\*|Next|Contains\\*|Contains|Sibling";
	ref				= synonym+or+underscore+or+integer+or+invComma+ident+invComma;
	attrName		= "procName|varName|value|stmt#";
	attrRef			= synonym+"\\s*\\.\\s*("+attrName+")";
	attrCompare		= "("+attrRef+"\\s*=\\s*("+attrRef+or+"\""+ident+"\""+or+integer+")"+or+synonym+"\\s*=\\s*("+attrRef+or+integer+"))";
	designEnt		= "(procedure|stmtLst|stmt|assign|call|while|if|variable|constant|prog_line)";
	elem			= synonym+or+attrRef;
	tuple			= elem+or+"\\s*<("+elem+")(\\s*,\\s*"+synonym+"|\\s*,\\s*"+attrRef+")"+compulsoryOne+"\\s*>";
	declare			= designEnt+"\\s+"+synonym+"(\\s*,\\s*"+synonym+")*";
	result_cl		= "Select\\s+(BOOLEAN"+or+tuple+")";
	suchthat_cl		= "such that\\s+("+rel+")\\s*\\(\\s*("+ref+")\\s*,\\s*("+ref+")\\s*\\)(\\s+and\\s+("+rel+")\\s*\\(\\s*("+ref+")\\s*,\\s*("+ref+")\\s*\\))"+optional;
	with_cl			= "with\\s+("+attrCompare+")(\\s+and\\s+("+attrCompare+"))"+optional;
	pattern_cl		= "pattern\\s+"+synonym+"\\s*\\(.+,.+,*\\)(\\s+and\\s+"+synonym+"\\s*\\(.+,.+,*\\))"+optional;

	/*
	cout<<"result_cl==============="<<endl;
	cout<<result_cl<<endl;
	cout<<"suchthat_cl==============="<<endl;
	cout<<suchthat_cl<<endl;
	cout<<"with_cl==============="<<endl;
	cout<<with_cl<<endl;
	cout<<"pattern_cl==============="<<endl;
	cout<<pattern_cl<<endl;
	*/
}

QueryPreprocessor::~QueryPreprocessor(void){
	delete qVarTable;
	delete dVarTable;
}

//--------------PUBLIC-----------------

bool QueryPreprocessor::preProcess(){

	//both declarations and main query must be valid
	//then will build qrTree & qrVarTable.
	*tokens = parse(this->query);	

	createGrammarTables();

	/*
	*inside validate()
	*first validate the declarations:
	*check syntax, build up the dVarTable and qrVarTable at the same time
	*then validate main query:
	*check syntax, check if variables exists (declared or in VarTable)
	*/

	bool validated = validate();
	cleanUp();

	return validated;
}

void QueryPreprocessor::setQuery(QUERY query){
	this->query = query;
}

hash_map<int,TYPE> *QueryPreprocessor::getQVarTable(){
	return qVarTable;
}

hash_map<int,string> *QueryPreprocessor::getParamTable(){
	return paramTable;
}

QTREE* QueryPreprocessor::getQTree(){
	return firstNode;
}

bool QueryPreprocessor::getSelectBool(){
	return selectBool;
}

ERROR_MSG QueryPreprocessor::getLastError(){
	return this->errorMsg;
}


//--------------PRIVATE-----------------

void QueryPreprocessor::createGrammarTables(){
	grammarTable.createEntTable();
	grammarTable.createRelTable();
	grammarTable.createPattTable();	
	grammarTable.createArgTable();
	grammarTable.createAttrTable();
	grammarTable.createContainSTInvNodeTable();
	grammarTable.createContainsValNodeTable();
}

bool QueryPreprocessor::validate(){

	TOKEN currToken;	
	selectBool=false;	
	clauseCount=0;
	groupCount=0;

	bool selectOK = false;
	bool conditionOK = false;

	for(int i=0;i<(*tokens).size();i++){
		currToken = (*tokens).at(i);

		//cout<<"i= "<< i<< endl;
		//cout<<currToken<<endl;


		if (regex_match(currToken,regex(declare))){		
			if (!verifyDeclaration(currToken)){
				return false;
			}
		}
		else if (regex_match(currToken,regex(result_cl))){
			if (!verifySelect(currToken)){
				return false;
			}
			selectOK = true;
		}
		else if (selectOK && regex_match(currToken,regex(suchthat_cl+or+with_cl+or+pattern_cl))){
			if (!verifyCondition(currToken)){
				return false;
			}
			conditionOK = true;
		}

	}	

	if(selectOK){
		setQTree();
		return true;
	}
	else{
		return false;
	}


}

void QueryPreprocessor::setQTree(){
	//idea: check selected variable/s is/are from which group/s
	int grpNum;
	vector<int> clauseNums;
	vector<vector<int>> groupings;
	dVarPair dpair;

	firstWithPatt = createQTREENode(ANY);
	firstFollows= createQTREENode(ANY); 
	firstFollowsSt= createQTREENode(ANY);
	firstUsesMod_Proc= createQTREENode(ANY);
	firstUsesMod_Call= createQTREENode(ANY);
	firstUsesMod_Stmt= createQTREENode(ANY);
	firstUsesMod_Assign= createQTREENode(ANY);
	firstUsesMod_Container= createQTREENode(ANY);
	firstUsesMod= createQTREENode(ANY);
	firstCall= createQTREENode(ANY);
	firstCallSt= createQTREENode(ANY);
	firstParent_KU= createQTREENode(ANY);
	firstParent_UK= createQTREENode(ANY);
	firstParent= createQTREENode(ANY);
	firstParentSt_KU= createQTREENode(ANY);
	firstParentSt_UK= createQTREENode(ANY);
	firstParentSt= createQTREENode(ANY);
	firstAffect= createQTREENode(ANY);
	firstAffectSt= createQTREENode(ANY);
	firstNext= createQTREENode(ANY);
	firstNextSt= createQTREENode(ANY);
	firstContainSibling= createQTREENode(ANY);

	//prepare sort
	//sortQVarTable();

	//initialise grouping
	for (int i=0;i<groupCount;i++){
		groupings.push_back(vector<int>());
	}


	//flag groups of selected variable
	//for every variable
	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
		//check if its selected
		qVar var = dVarIter->second;
		string varName = dVarIter->first;
		//get groupNum
		grpNum = var.groupNum;
		if(var.selected){
			//check if the group is already flagged
			if (!isFlaggedGroup(grpNum)){
				//add flag 
				flagGroups.push_back(grpNum);
			}	
		}

		//get groupings right
		vector<int> cN = var.clauseNum;
		for (int v = 0; v<cN.size(); v++){
			if(std::find(groupings[grpNum].begin(),groupings[grpNum].end(),cN[v])==groupings[grpNum].end()){
				groupings[grpNum].push_back(cN[v]);
			}
		}

	}

	//want to further rearrange the clauses in the groups


	//insert the clauses NOT participating in results

	//1. add clauses with two constants
	for (int i=0; i<twoConstantClauses.size(); i++){
		currNode = clauses.at(twoConstantClauses.at(i));
		if (currNode!=NULL){
			arrangeClauseByRel(currNode);
			clauses.at(twoConstantClauses.at(i)) = NULL;
		}
	}

	joinClauses();

	//2. add clauses with one constant	
	for (int i=0; i<groupings.size(); i++){
		if (!groupings[i].empty() && !isFlaggedGroup(i)){
			for (int k = 0; k<groupings[i].size(); k++){
				if(hasConstantVar(groupings[i][k])){
					currNode = clauses.at(groupings[i][k]);
					if (currNode!=NULL){
						arrangeClauseByRel(currNode);//-------------Apply filters here
						clauses.at(groupings[i][k]) = NULL;
					}
				}
			}

		}
	}
	joinClauses();

	//3. add clauses with two unknowns
	for (int i=0; i<groupings.size(); i++){
		if (!groupings[i].empty() && !isFlaggedGroup(i)){
			//throw the whole list of clauses to be processed
			processClauses(groupings[i],0);
		}
	}

	/*
	//3. add clause with two unknowns
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those not flagged for selected
	if(!isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (!hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);//-------------Apply filters here
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}

	}
	}
	}
	joinClauses();
	*/

	//4. add clause with wildcard		
	for (int i=0; i<wildClauses.size(); i++){
		currNode = clauses.at(wildClauses.at(i));
		if (currNode!=NULL){
			arrangeClauseByRel(currNode);
			clauses.at(wildClauses.at(i)) = NULL;
		}
	}		
	joinClauses();

	//insert the clauses participating in results

	//5. add clauses with one constant
	for (int i=0; i<groupings.size(); i++){
		if (!groupings[i].empty() && isFlaggedGroup(i)){
			for (int k = 0; k<groupings[i].size(); k++){
				if(hasConstantVar(groupings[i][k])){
					currNode = clauses.at(groupings[i][k]);
					if (currNode!=NULL){						
						currNode->setData(1);
						arrangeClauseByRel(currNode);//-------------Apply filters here
						clauses.at(groupings[i][k]) = NULL;
					}
				}
			}

		}
	}
	joinClauses();

	/*
	//6. add clause with two unknowns
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those flagged for selected
	if(isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (!hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);//-------------Apply filters here
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}

	}
	}
	}
	joinClauses();
	*/

	//6. add clauses with two unknowns
	for (int i=0; i<groupings.size(); i++){
		if (!groupings[i].empty() && isFlaggedGroup(i)){
			//throw the whole list of clauses to be processed
			processClauses(groupings[i],1);
		}
	}

	/*
	//7. add everything else (make sure everything is added)
	for (int i=0; i<clauses.size(); i++){
	currNode = clauses.at(i);
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(i) = NULL;
	}
	}

	joinClauses();	

	*/

	//old function
	/*for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){

	if(!isFlaggedGroup(dVarIter->second.groupNum)){
	//retrieve the clause number
	clauseNums = dVarIter->second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}		
	}


	//insert into QTREE the clauses with the variables NOT from those flagged groups 
	//insert those with 1 constant first
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those not flagged for selected
	if(!isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}
	}
	}

	joinClauses();

	//insert those with two unknowns
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those not flagged for selected
	if(!isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (!hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}
	}
	}

	joinClauses();

	//insert those clauses with wildcards
	for (int i=0; i<wildClauses.size(); i++){
	currNode = clauses.at(wildClauses.at(i));
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(wildClauses.at(i)) = NULL;
	}
	}

	joinClauses();

	//then insert the rest (participating in result)
	//with 1 constant
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those flagged for selected
	if(isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	currNode->setData(1);
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}
	}
	}

	joinClauses();

	//with two unknowns
	for (int i=0; i<sorted_qVarTable.size(); i++){
	dpair = getQVar(sorted_qVarTable.at(i).first);
	//choose those not flagged for selected
	if(isFlaggedGroup(dpair.second.groupNum)){
	//retrieve the clause number
	clauseNums = dpair.second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	if (!hasConstantVar(clauseNums.at(i))){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	currNode->setData(1);
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}
	}
	}

	joinClauses();

	//old function
	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
	if(isFlaggedGroup(dVarIter->second.groupNum)){
	clauseNums = dVarIter->second.clauseNum;
	for (int i=0; i<clauseNums.size(); i++){
	currNode = clauses.at(clauseNums.at(i));
	if (currNode!=NULL){
	currNode->setData(1);
	arrangeClauseByRel(currNode);
	clauses.at(clauseNums.at(i)) = NULL;
	}
	}
	}		
	}



	for (int i=0; i<clauses.size(); i++){
	currNode = clauses.at(i);
	if (currNode!=NULL){
	arrangeClauseByRel(currNode);
	clauses.at(i) = NULL;
	}
	}

	joinClauses();
	*/
}

vector<QTREE*> QueryPreprocessor::addToProbe(QTREE* currClause){
	vector<QTREE*> queue;
	if (currClause->getType() == SUCHTHAT){
		//found in probed, add to queue
		int first = currClause->getFirstDescendant()->getFirstDescendant()->getData();
		int second = currClause->getFirstDescendant()->getLastDescendant()->getData();
		vector<int>::iterator posfirst = std::find(trackProbes.begin(),trackProbes.end(),first);
		vector<int>::iterator possecond = std::find(trackProbes.begin(),trackProbes.end(),second);
		//both arguments probed
		if(posfirst!=trackProbes.end() && possecond!=trackProbes.end()){
			queue.push_back(currClause);
		}
		//first argument probed
		else if (posfirst!=trackProbes.end()){
			queue.push_back(currClause);
			trackProbes.push_back(second);
		}
		//second argument probed
		else if (possecond!=trackProbes.end()){
			queue.push_back(currClause);
			trackProbes.push_back(first);
		}
	}
	else if (currClause->getType() == PATTERN){
		int first = currClause->getFirstDescendant()->getData();
		int second = currClause->getFirstDescendant()->getLastDescendant()->getData();
		vector<int>::iterator posfirst = std::find(trackProbes.begin(),trackProbes.end(),first);
		vector<int>::iterator possecond = std::find(trackProbes.begin(),trackProbes.end(),second);
		//both arguments probed
		if(posfirst!=trackProbes.end() && possecond!=trackProbes.end()){
			queue.push_back(currClause);
		}
		//first argument probed
		else if (posfirst!=trackProbes.end()){
			queue.push_back(currClause);
			trackProbes.push_back(second);
		}
		//second argument probed
		else if (possecond!=trackProbes.end()){
			queue.push_back(currClause);
			trackProbes.push_back(first);
		}
	}
	else if (currClause->getType() == WITH){
		int first = currClause->getFirstDescendant()->getData();
		int second = currClause->getFirstDescendant()->getRightSibling()->getRightSibling()->getData();
		vector<int>::iterator posfirst = std::find(trackProbes.begin(),trackProbes.end(),first);
		vector<int>::iterator possecond = std::find(trackProbes.begin(),trackProbes.end(),second);
		if (second<0){
			//is a constant on right side, ignore
			if (posfirst!=trackProbes.end()){
				queue.push_back(currClause);
			}
		}
		else{
			//both arguments probed
			if(posfirst!=trackProbes.end() && possecond!=trackProbes.end()){
				queue.push_back(currClause);
			}
			//first argument probed
			else if (posfirst!=trackProbes.end()){
				queue.push_back(currClause);
				trackProbes.push_back(second);
			}
			//second argument probed
			else if (possecond!=trackProbes.end()){
				queue.push_back(currClause);
				trackProbes.push_back(first);
			}
		}
	}
	return queue;
}

void QueryPreprocessor::processClauses(vector<int> cl,int mode){
	QTREE* currClause;
	vector<QTREE*> queue;
	vector<QTREE*> patternwith;
	vector<QTREE*> follows;	
	vector<QTREE*> parent;	
	vector<QTREE*> calls;
	vector<QTREE*> modifies_assign;
	vector<QTREE*> modifies_container;	
	vector<QTREE*> modifies_proc;
	vector<QTREE*> modifies_call;
	vector<QTREE*> modifies_stmt;
	vector<QTREE*> uses_assign;
	vector<QTREE*> uses_container;	
	vector<QTREE*> uses_proc;
	vector<QTREE*> uses_call;
	vector<QTREE*> uses_stmt;
	vector<QTREE*> callst;
	vector<QTREE*> parentst;
	vector<QTREE*> followst;
	vector<QTREE*> next;
	vector<QTREE*> nextst;
	vector<QTREE*> affects;
	vector<QTREE*> affectst;
	vector<QTREE*> consib;

	TYPE relType;

	for (int w=0; w< cl.size() ; w++){
		currClause = clauses[cl[w]];
		if(currClause!=NULL){
			if (mode == 1)
				currClause->setData(1);
			relType = currClause->getFirstDescendant()->getType();
			//pattern,with
			if (currClause->getType() == PATTERN || currClause->getType() == WITH){
				patternwith.push_back(currClause);
			}
			//follows
			else if (relType==FOLLOWS){
				follows.push_back(currClause);
			}
			//follows*
			else if (relType==FOLLOWST){
				followst.push_back(currClause);
			}
			//parent
			else if (relType==PARENT){			
				parent.push_back(currClause);
			}
			//parent*
			else if (relType==PARENTST){
				parentst.push_back(currClause);
			}
			//calls
			else if (relType==CALL){
				calls.push_back(currClause);
			}
			//calls*
			else if (relType==CALLST){
				callst.push_back(currClause);
			}
			//next
			else if (relType==NEXT){
				next.push_back(currClause);
			}
			//next*
			else if (relType==NEXTST){
				nextst.push_back(currClause);
			}
			//affect
			else if (relType==AFFECTS){
				affects.push_back(currClause);
			}
			//affect*
			else if (relType==AFFECTST){
				affectst.push_back(currClause);
			}
			//contains,contains*,sibling
			else if (relType==CONTAINS||relType==CONTAINST||relType==SIBLING){
				consib.push_back(currClause);
			}
			//modifies
			else if (relType==MODIFIES){
				//assign
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==ASSIGNMENT){
					modifies_assign.push_back(currClause);
				}
				//proc
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==PROCEDURE){
					modifies_proc.push_back(currClause);
				}
				//call
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==CALL){
					modifies_call.push_back(currClause);
				}
				//container
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==WHILE || getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==IF){
					modifies_container.push_back(currClause);
				}
				//stmt
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==STATEMENT){
					modifies_stmt.push_back(currClause);
				}
			}
			//uses
			else if (relType==USES){
				//assign
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==ASSIGNMENT){
					uses_assign.push_back(currClause);
				}
				//proc
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==PROCEDURE){
					uses_proc.push_back(currClause);
				}
				//call
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==CALL){
					uses_call.push_back(currClause);
				}
				//container
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==WHILE || getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==IF){
					uses_container.push_back(currClause);
				}
				//stmt
				if (getQVarType(currClause->getFirstDescendant()->getFirstDescendant()->getData())==STATEMENT){
					uses_stmt.push_back(currClause);
				}
			}
			clauses[cl[w]] = NULL;
		}
	}

	//check for probes
	for(int a = 0; a< patternwith.size(); a++){
		currClause = patternwith[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< follows.size(); a++){
		currClause = follows[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< parent.size(); a++){
		currClause = parent[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< calls.size(); a++){
		currClause = calls[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< modifies_assign.size(); a++){
		currClause = modifies_assign[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< uses_assign.size(); a++){
		currClause = uses_assign[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< callst.size(); a++){
		currClause = callst[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< parentst.size(); a++){
		currClause = parentst[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< followst.size(); a++){
		currClause = followst[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< modifies_container.size(); a++){
		currClause = modifies_container[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< modifies_proc.size(); a++){
		currClause = modifies_proc[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< modifies_call.size(); a++){
		currClause = modifies_call[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< modifies_stmt.size(); a++){
		currClause = modifies_stmt[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< uses_container.size(); a++){
		currClause = uses_container[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< uses_proc.size(); a++){
		currClause = uses_proc[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< uses_call.size(); a++){
		currClause = uses_call[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< uses_stmt.size(); a++){
		currClause = uses_stmt[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< next.size(); a++){
		currClause = next[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< nextst.size(); a++){
		currClause = nextst[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< affects.size(); a++){
		currClause = affects[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< affectst.size(); a++){
		currClause = affectst[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}
	for(int a = 0; a< consib.size(); a++){
		currClause = consib[a];
		vector<QTREE*> temp = addToProbe(currClause);
		queue.insert(queue.end(),temp.begin(),temp.end());
	}

	//chainup
	for(int i=0;i< patternwith.size();i++){
		if(std::find(queue.begin(),queue.end(),patternwith[i])==queue.end())
			queue.push_back(patternwith[i]);
	}
	for(int i=0;i< follows.size();i++){
		if(std::find(queue.begin(),queue.end(),follows[i])==queue.end())
			queue.push_back(follows[i]);
	}
	for(int i=0;i< parent.size();i++){
		if(std::find(queue.begin(),queue.end(),parent[i])==queue.end())
			queue.push_back(parent[i]);
	}	
	for(int i=0;i< calls.size();i++){
		if(std::find(queue.begin(),queue.end(),calls[i])==queue.end())
			queue.push_back(calls[i]);
	}
	for(int i=0;i< modifies_assign.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_assign[i])==queue.end())
			queue.push_back(modifies_assign[i]);
	}
	for(int i=0;i< uses_assign.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_assign[i])==queue.end())
			queue.push_back(uses_assign[i]);
	}
	for(int i=0;i< callst.size();i++){
		if(std::find(queue.begin(),queue.end(),callst[i])==queue.end())
			queue.push_back(callst[i]);
	}
	for(int i=0;i< parentst.size();i++){
		if(std::find(queue.begin(),queue.end(),parentst[i])==queue.end())
			queue.push_back(parentst[i]);
	}
	for(int i=0;i< followst.size();i++){
		if(std::find(queue.begin(),queue.end(),followst[i])==queue.end())
			queue.push_back(followst[i]);
	}
	for(int i=0;i< modifies_container.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_container[i])==queue.end())
			queue.push_back(modifies_container[i]);
	}	
	for(int i=0;i< modifies_proc.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_proc[i])==queue.end())
			queue.push_back(modifies_proc[i]);
	}
	for(int i=0;i< modifies_assign.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_assign[i])==queue.end())
			queue.push_back(modifies_assign[i]);
	}
	for(int i=0;i< modifies_call.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_call[i])==queue.end())
			queue.push_back(modifies_call[i]);
	}
	for(int i=0;i< modifies_stmt.size();i++){
		if(std::find(queue.begin(),queue.end(),modifies_stmt[i])==queue.end())
			queue.push_back(modifies_stmt[i]);
	}
	for(int i=0;i< uses_container.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_container[i])==queue.end())
			queue.push_back(uses_container[i]);
	}	
	for(int i=0;i< uses_proc.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_proc[i])==queue.end())
			queue.push_back(uses_proc[i]);
	}
	for(int i=0;i< uses_assign.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_assign[i])==queue.end())
			queue.push_back(uses_assign[i]);
	}
	for(int i=0;i< uses_call.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_call[i])==queue.end())
			queue.push_back(uses_call[i]);
	}
	for(int i=0;i< uses_stmt.size();i++){
		if(std::find(queue.begin(),queue.end(),uses_stmt[i])==queue.end())
			queue.push_back(uses_stmt[i]);
	}
	for(int i=0;i< next.size();i++){
		if(std::find(queue.begin(),queue.end(),next[i])==queue.end())
			queue.push_back(next[i]);
	}	
	for(int i=0;i< nextst.size();i++){
		if(std::find(queue.begin(),queue.end(),nextst[i])==queue.end())
			queue.push_back(nextst[i]);
	}
	for(int i=0;i< affects.size();i++){
		if(std::find(queue.begin(),queue.end(),affects[i])==queue.end())
			queue.push_back(affects[i]);
	}	
	for(int i=0;i< affectst.size();i++){
		if(std::find(queue.begin(),queue.end(),affectst[i])==queue.end())
			queue.push_back(affectst[i]);
	}
	for(int i=0;i< consib.size();i++){
		if(std::find(queue.begin(),queue.end(),consib[i])==queue.end())
			queue.push_back(consib[i]);
	}

	//finally can add to final node
	for(int w=0;w<queue.size();w++){
		firstNode->getLastSibling()->setSibling(queue[w]);
		firstNode->setLastSibling(queue[w]);
	}
}

void QueryPreprocessor::arrangeClauseByRel(QTREE* node){
	TYPE relType;	


	relType = node->getFirstDescendant()->getType();

	//pattern, with
	if (node->getType()==PATTERN || node->getType()==WITH){
		if (firstWithPatt->getType()==ANY){
			firstWithPatt = node;
		}
		else{
			lastWithPatt->setSibling(node);
		}
		lastWithPatt = node;
	}
	//calls
	else if (relType==CALL){
		if (firstCall->getType()==ANY){
			firstCall = node;
		}
		else{
			lastCall->setSibling(node);
		}
		lastCall = node;
	}
	//calls*
	else if (relType==CALLST){
		if (firstCallSt->getType()==ANY){
			firstCallSt = node;
		}
		else{
			lastCallSt->setSibling(node);
		}
		lastCallSt = node;
	}
	//parent
	else if (relType==PARENT){
		if (node->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR){
			//(unknown,known)
			if (firstParent_UK->getType()==ANY){
				firstParent_UK = node;
			}
			else{
				lastParent_UK->setSibling(node);
			}
			lastParent_UK = node;
		}
		else{
			//(known,unknown)			
			if (firstParent_KU->getType()==ANY){
				firstParent_KU = node;
			}
			else{
				lastParent_KU->setSibling(node);
			}
			lastParent_KU = node;
		}			
	}
	//parent*
	else if (relType==PARENTST){
		if (node->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR){
			//(unknown,known)
			if (firstParentSt_UK->getType()==ANY){
				firstParentSt_UK = node;
			}
			else{
				lastParentSt_UK->setSibling(node);
			}
			lastParentSt_UK = node;
		}
		else{
			//(known,unknown)
			if (firstParentSt_KU->getType()==ANY){
				firstParentSt_KU = node;
			}
			else{
				lastParentSt_KU->setSibling(node);
			}
			lastParentSt_KU = node;
		}
	}
	//follows
	else if (relType==FOLLOWS){
		if (firstFollows->getType()==ANY){
			firstFollows = node;
		}
		else{
			lastFollows->setSibling(node);
		}
		lastFollows = node;
	}
	//follows*
	else if (relType==FOLLOWST){
		if (firstFollowsSt->getType()==ANY){
			firstFollowsSt = node;
		}
		else{
			lastFollowsSt->setSibling(node);
		}
		lastFollowsSt = node;
	}
	//modifies,uses
	else if (relType==MODIFIES||relType==USES){
		if (node->getFirstDescendant()->getFirstDescendant()->getType()==QUERYVAR){
			//(unknown,known)
			//find out what is the first unknown: stmt,while/if,assign,call,proc?
			if(getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==STATEMENT){
				if (firstUsesMod_Stmt->getType()==ANY){
					firstUsesMod_Stmt = node;
				}
				else{
					lastUsesMod_Stmt->setSibling(node);
				}
				lastUsesMod_Stmt = node;
			}
			else if(getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==ASSIGNMENT){
				if (firstUsesMod_Assign->getType()==ANY){
					firstUsesMod_Assign = node;
				}
				else{
					lastUsesMod_Assign->setSibling(node);
				}
				lastUsesMod_Assign = node;
			}
			else if(getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==PROCEDURE){
				if (firstUsesMod_Proc->getType()==ANY){
					firstUsesMod_Proc = node;
				}
				else{
					lastUsesMod_Proc->setSibling(node);
				}
				lastUsesMod_Proc = node;
			}
			else if(getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==CALL){
				if (firstUsesMod_Call->getType()==ANY){
					firstUsesMod_Call = node;
				}
				else{
					lastUsesMod_Call->setSibling(node);
				}
				lastUsesMod_Call = node;
			}
			else if(getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==WHILE || getQVarType(node->getFirstDescendant()->getFirstDescendant()->getData())==IF){
				if (firstUsesMod_Container->getType()==ANY){
					firstUsesMod_Container = node;
				}
				else{
					lastUsesMod_Container->setSibling(node);
				}
				lastUsesMod_Container = node;
			}

		}
		else{
			//(known,unknown)
			if (node->getFirstDescendant()->getLastDescendant()->getType()==QUERYVAR){
				if (node->getFirstDescendant()->getFirstDescendant()->getType()==STATEMENT){
					if (firstUsesMod_Stmt->getType()==ANY){
						firstUsesMod_Stmt = node;
					}
					else{
						lastUsesMod_Stmt->setSibling(node);
					}
					lastUsesMod_Stmt = node;
				}
				else if (node->getFirstDescendant()->getFirstDescendant()->getType()==PROCEDURE){
					if (firstUsesMod_Proc->getType()==ANY){
						firstUsesMod_Proc = node;
					}
					else{
						lastUsesMod_Proc->setSibling(node);
					}
					lastUsesMod_Proc = node;
				}
			}
			else{
				if (firstUsesMod_Stmt->getType()==ANY){
					firstUsesMod_Stmt = node;
				}
				else{
					lastUsesMod_Stmt->setSibling(node);
				}
				lastUsesMod_Stmt = node;
			}
		}

	}
	//next
	else if (relType==NEXT){
		if (firstNext->getType()==ANY){
			firstNext = node;
		}
		else{
			lastNext->setSibling(node);
		}
		lastNext = node;
	}
	//next*
	else if (relType==NEXTST){
		if (firstNextSt->getType()==ANY){
			firstNextSt = node;
		}
		else{
			lastNextSt->setSibling(node);
		}
		lastNextSt = node;
	}
	//affects
	else if (relType==AFFECTS){
		if (firstAffect->getType()==ANY){
			firstAffect = node;
		}
		else{
			lastAffect->setSibling(node);
		}
		lastAffect = node;
	}
	//affects*
	else if (relType==AFFECTST){
		if (firstAffectSt->getType()==ANY){
			firstAffectSt = node;
		}
		else{
			lastAffectSt->setSibling(node);
		}
		lastAffectSt = node;
	}
	//Contains,Contains*,Sibling
	else if (relType==CONTAINS||relType==CONTAINST||relType==SIBLING){
		if (firstContainSibling->getType()==ANY){
			firstContainSibling = node;
		}
		else{
			lastContainSibling->setSibling(node);
		}
		lastContainSibling = node;
	}

}

void QueryPreprocessor::joinClauses(){

	//with,pattern
	if (firstWithPatt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstWithPatt);
		firstNode->setLastSibling(lastWithPatt);		
	}
	//follows
	if (firstFollows->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstFollows);
		firstNode->setLastSibling(lastFollows);		
	}
	//parent(unknown,known)
	if (firstParent_UK->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParent_UK);
		firstNode->setLastSibling(lastParent_UK);		
	}
	//parent(known,unknown)
	if (firstParent_KU->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParent_KU);
		firstNode->setLastSibling(lastParent_KU);		
	}
	//parent
	if (firstParent->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParent);
		firstNode->setLastSibling(lastParent);		
	}
	//calls
	if (firstCall->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstCall);
		firstNode->setLastSibling(lastCall);		
	}
	//modifies(unknown_assign,unknown/known)
	if (firstUsesMod_Assign->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod_Assign);
		firstNode->setLastSibling(lastUsesMod_Assign);		
	}
	//calls*
	if (firstCallSt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstCallSt);
		firstNode->setLastSibling(lastCallSt);		
	}
	//parent*(unknown,known)
	if (firstParentSt_UK->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParentSt_UK);
		firstNode->setLastSibling(lastParentSt_UK);		
	}	
	//follows*
	if (firstFollowsSt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstFollowsSt);
		firstNode->setLastSibling(lastFollowsSt);		
	}
	//parent*(known,unknown)
	if (firstParentSt_KU->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParentSt_KU);
		firstNode->setLastSibling(lastParentSt_KU);		
	}
	//parent*
	if (firstParentSt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstParentSt);
		firstNode->setLastSibling(lastParentSt);		
	}
	//uses/modifies(unknown_container,unknown/known)
	if (firstUsesMod_Container->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod_Container);
		firstNode->setLastSibling(lastUsesMod_Container);		
	}
	//uses/modifies(unknown_proc,unknown/known)
	if (firstUsesMod_Proc->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod_Proc);
		firstNode->setLastSibling(lastUsesMod_Proc);		
	}
	//uses/modifies(unknown_call,unknown/known)
	if (firstUsesMod_Call->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod_Call);
		firstNode->setLastSibling(lastUsesMod_Call);		
	}
	//uses/modifies(unknown_stmt,unknown/known)
	if (firstUsesMod_Stmt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod_Stmt);
		firstNode->setLastSibling(lastUsesMod_Stmt);		
	}
	//uses/modifies
	if (firstUsesMod->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod);
		firstNode->setLastSibling(lastUsesMod);		
	}
	//next
	if (firstNext->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstNext);
		firstNode->setLastSibling(lastNext);		
	}
	//next*
	if (firstNextSt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstNextSt);
		firstNode->setLastSibling(lastNextSt);		
	}
	//affect
	if (firstAffect->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstAffect);
		firstNode->setLastSibling(lastAffect);		
	}
	//affect*
	if (firstAffectSt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstAffectSt);
		firstNode->setLastSibling(lastAffectSt);		
	}
	//contains,contains*,sibling
	if (firstContainSibling->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstContainSibling);
		firstNode->setLastSibling(lastContainSibling);		
	}

	firstWithPatt = createQTREENode(ANY);
	firstFollows= createQTREENode(ANY); 
	firstFollowsSt= createQTREENode(ANY);
	firstUsesMod_Proc= createQTREENode(ANY);
	firstUsesMod_Call= createQTREENode(ANY);
	firstUsesMod_Stmt= createQTREENode(ANY);
	firstUsesMod_Assign= createQTREENode(ANY);
	firstUsesMod_Container= createQTREENode(ANY);
	firstUsesMod= createQTREENode(ANY);
	firstCall= createQTREENode(ANY);
	firstCallSt= createQTREENode(ANY);
	firstParent_KU= createQTREENode(ANY);
	firstParent_UK= createQTREENode(ANY);
	firstParent= createQTREENode(ANY);
	firstParentSt_KU= createQTREENode(ANY);
	firstParentSt_UK= createQTREENode(ANY);
	firstParentSt= createQTREENode(ANY);
	firstAffect= createQTREENode(ANY);
	firstAffectSt= createQTREENode(ANY);
	firstNext= createQTREENode(ANY);
	firstNextSt= createQTREENode(ANY);
	firstContainSibling= createQTREENode(ANY);
}

bool QueryPreprocessor::isFlaggedGroup(int grpNum){
	for (int i=0; i<flagGroups.size(); i++){
		if (grpNum==flagGroups.at(i)){
			return true;
		}
	}
	return false;
}

bool QueryPreprocessor::verifyDeclaration(TOKEN token){

	vector<TOKEN> declarations;
	TYPE entType;
	TOKEN currToken;
	qVar newVar;

	declarations = tokenize(token,designEnt+or+synonym);
	for(int i=0;i<declarations.size();i++){
		currToken = declarations.at(i);
		if (regex_match(currToken,regex(designEnt))){
			//expect declaration type
			if (grammarTable.isEntExists(currToken)){
				entType = grammarTable.getEntType(currToken);
			}
			else {
				error(INVALID_ENTITY);
				return false;
			}
		}
		else {
			//expect synonym
			if (isDeclaredVar(currToken)){
				//already declared, check if it is a different entity type (conflict)
				if (getQVarType(currToken)!=entType){
					error(CONFLICTED_SYNONYM);
					return false;
				}
			}
			else{
				//if not previously declared, add new entry
				newVar.qVarIndex=(*dVarTable).size();
				newVar.qVarType=entType;
				newVar.selected=false;
				newVar.explored=false;
				(*dVarTable).insert(dVarPair(currToken,newVar));
				(*qVarTable).insert(qVarPair((*qVarTable).size(),entType));
			}
		}
	}
	return true;
}

bool QueryPreprocessor::verifySelect(TOKEN token){
	vector<TOKEN> selections;
	TOKEN currToken;

	firstNode = createQTREENode(RESULT);
	firstNode->setLastSibling(firstNode);

	selections = tokenize(token,elem);
	for(int i=1;i<selections.size();i++){
		currToken = selections.at(i);
		if (regex_match(currToken,regex("BOOLEAN"))){
			currNode = createQTREENode(BOOL);
			selectBool=true;
		}
		else{
			if (isDeclaredVar(currToken)){
				currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
				setAsSelected(currToken);
			}
			else{
				error(INVALID_RESULT);
				return false;
			}
		}
		setChild(firstNode,currNode);
	}

	return true;
}

bool QueryPreprocessor::verifyCondition(TOKEN token){

	vector<TOKEN> conditions;
	if (regex_match(token,regex(suchthat_cl))){
		conditions = tokenize(token,"("+rel+")\\s*\\(\\s*("+ref+")\\s*,\\s*("+ref+")\\s*\\)");
		for(int i=0;i<conditions.size();i++){
			if (processSuchThat(conditions.at(i))){
				clauseCount++;
			}
			else{
				return false;
			}
		}
	}
	else if (regex_match(token,regex(with_cl))){
		conditions = tokenize(token,attrCompare);
		for(int i=0;i<conditions.size();i++){		
			if (processWith(conditions.at(i))){
				clauseCount++;
			}
			else{
				return false;
			}
		}
	}
	else if (regex_match(token,regex(pattern_cl))){
		conditions = tokenize(token,synonym+"\\s*\\(.+,.+,*\\)");
		for(int i=0;i<conditions.size();i++){		
			if (processPattern(conditions.at(i))){
				clauseCount++;
			}
			else{
				return false;
			}
		}
	}
	else{
		error(INVALID_QUERY_SYNTAX);
		return false;
	}

	return true;
}

bool QueryPreprocessor::processSuchThat(TOKEN token){

	vector<TOKEN> relationships;
	vector<TYPE> validArgType;
	vector<TOKEN> syn;
	TOKEN currToken;
	int argCount;
	bool correctArg;
	bool prevArgWild;
	bool prevArgConstant;
	bool firstArg;
	bool firstArgConstant;
	string relName;
	TYPE relType;
	TYPE tokenType;
	TYPE prevTokenType = ANY;

	headNode = createQTREENode(SUCHTHAT);
	insertClause(headNode);

	relationships = tokenize(token,"("+rel+")"+or+"("+ref+")");
	argCount=relationships.size()-1;
	relName = relationships.at(0);

	//do special cases for modifies and uses
	if (relName=="Modifies"){
		//look ahead to first arg
		if (regex_match(relationships.at(1),regex(synonym+or+underscore+or+integer))){
			relName="Modifies_s";
		}
		else{
			relName="Modifies_p";
		}
	}
	else if (relName=="Uses"){
		//look ahead to first arg
		if (regex_match(relationships.at(1),regex(synonym+or+underscore+or+integer))){
			relName="Uses_s";
		}
		else{
			relName="Uses_p";
		}
	}

	//check valid relationship
	if (!grammarTable.isRelExists(relName)){
		error(INVALID_RELATIONSHIP);
		return false;
	}

	//check number of arguments is correct
	if (!grammarTable.getRelArgCount(relName)==argCount){

		error(INVALID_ARGUMENT);
		return false;
	}

	relType = grammarTable.getRelType(relName);
	currNode = createQTREENode(relType);			
	setChild(headNode,currNode);
	prevNode=currNode;
	prevArgWild = false;
	prevArgConstant = false;
	firstArg = true;
	for(int i=1;i<relationships.size();i++){			
		currToken = relationships.at(i);

		//check argument's syntax
		if(!regex_match(currToken,regex(grammarTable.getRelArg(relName,i)))){
			error(INVALID_ARGUMENT);
			return false;			
		}

		//if with inverted commas
		//two scenarios: "<varName>" and "<procName>"
		if(regex_match(currToken,regex(invComma+ident+invComma))){			
			currToken.erase(currToken.begin());	
			currToken.resize(currToken.size()-1);
			if (isConstant(currToken)){
				tokenType = CONSTANT;
				currNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
			}
			if ((i==1&&(relName=="Modifies_p"||relName=="Uses_p"))||relName=="Calls"||relName=="Calls*"){
				if (!pkb->isProcExists(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				tokenType = PROCEDURE;
				currNode = createQTREENode(PROCEDURE,pkb->getProcIndex(currToken));
			}
			else if(i==1&&(relName=="Contains"||relName=="Contains*"||relName=="Sibling")){
				if (pkb->isProcExists(currToken)){
					tokenType = PROCEDURE;
					currNode = createQTREENode(PROCEDURE,pkb->getProcIndex(currToken));
				}
				else{
					//not a procedure, could be a var
					if (!pkb->isVarExists(currToken)){
						error(INVALID_VARIABLE);
						return false;
					}
					tokenType = VARIABLE;
					currNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));		
				}
			}
			else{
				if (!pkb->isVarExists(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				tokenType = VARIABLE;
				currNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));				
			}
			if (prevArgConstant){
				twoConstantClauses.push_back(clauseCount);
			}
			else if(firstArg){					
				prevArgConstant = true;
				firstArg = false;
			}
			else{
				oneConstantClauses.push_back(clauseCount);
				int t = getQVarIndex(syn[0]);
				if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
					trackProbes.push_back(t);
				}
			}
		}
		//can also be wildcards _
		else if(currToken=="_"){
			if(((relName=="Modifies_p"||relName=="Modifies_s"||relName=="Uses_p"||relName=="Uses_s")&&firstArg)||
				(relName=="Contains*"||relName=="Contains"||relName=="Sibling")){
					error(INVALID_ARGUMENT);
					return false;	
			}		
			else{
				tokenType = ANY;
				currNode = createQTREENode(ANY);
				if (prevArgWild){
					wildClauses.push_back(clauseCount);
				}
				else{
					prevArgWild = true;
				}
				if (prevArgConstant){
					oneConstantClauses.push_back(clauseCount);
				}
				else if(firstArg){				
					firstArg = false;
				}	
			}
		}
		//check is the synonym declared
		else if (isDeclaredVar(currToken)){
			tokenType = getQVarType(currToken);			
			syn.push_back(currToken);			
			currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
			if (prevArgConstant){
				oneConstantClauses.push_back(clauseCount);		
				int t = getQVarIndex(syn[0]);		
				if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
					trackProbes.push_back(t);
				}
			}
			else if(firstArg){		
				firstArg = false;
			}			
		}
		//even if not declared,could be a constant statement number
		else if (isConstant(currToken)){			
			if (relName=="Next"||relName=="Next*"){
				tokenType = PROGLINE;
				currNode = createQTREENode(PROGLINE,atoi(currToken.c_str()));
			}
			else if(relName=="Affects"||relName=="Affects*"){
				tokenType = ASSIGNMENT;
				currNode = createQTREENode(ASSIGNMENT,atoi(currToken.c_str()));
			}
			else{
				tokenType = STATEMENT;			
				currNode = createQTREENode(STATEMENT,atoi(currToken.c_str()));
			}
			if (prevArgConstant){
				twoConstantClauses.push_back(clauseCount);
			}
			else{
				//the first arg NOT constant or
				//this is the first arg
				if(firstArg){					
					prevArgConstant = true;
					firstArg = false;
				}
				else{
					oneConstantClauses.push_back(clauseCount);
					int t = getQVarIndex(syn[0]);
					if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
						trackProbes.push_back(t);
					}
				}
			}
		}

		//not declared, not constant
		else{
			error(INVALID_ARGUMENT);
			return false;		
		}

		//check argument's type

		if(relName=="Contains*"){
			if (prevTokenType==ANY){
				prevTokenType = tokenType;
				correctArg = true;
				if (isConstant(currToken)){
					firstArgConstant = true;
				}
			}
			else{
				validArgType = grammarTable.getContainSTInvNodes(prevTokenType);
				correctArg = true;
				for(int j = 0; j < validArgType.size(); j++){
					if (tokenType == validArgType.at(j)){
						correctArg = false;
						break;
					}

				}
				if (!correctArg && firstArgConstant){
					validArgType = grammarTable.getContainSTInvNodes(STMT_LIST);
					correctArg = true;
					for(int j = 0; j < validArgType.size(); j++){
						if (tokenType == validArgType.at(j)){
							correctArg = false;
							break;
						}

					}
				}
				

			}
		}
		else {
			if (relName=="Contains"){
				if (prevTokenType==ANY){
					prevTokenType = tokenType;
					correctArg = true;
					if (isConstant(currToken)){
						firstArgConstant = true;
					}
				}
				else{
					validArgType = grammarTable.getContainsValNodes(tokenType);
					correctArg = false;
					for(int j = 0; j < validArgType.size(); j++){
						if (tokenType == validArgType.at(j)){
							correctArg = true;
							break;
						}
					}
					if (!correctArg && firstArgConstant){
					validArgType = grammarTable.getContainsValNodes(STMT_LIST);
					correctArg = false;
					for(int j = 0; j < validArgType.size(); j++){
						if (tokenType == validArgType.at(j)){
							correctArg = true;
							break;
						}

					}
				}
				}
			}
			else if (relName=="Sibling"){
				if (prevTokenType==ANY){
					prevTokenType = tokenType;
					correctArg = true;
					if (isConstant(currToken)){
						firstArgConstant = true;
					}
				}
				else{
					validArgType = grammarTable.getSiblingValNodes(tokenType);
					correctArg = false;
					for(int j = 0; j < validArgType.size(); j++){
						if (tokenType == validArgType.at(j)){
							correctArg = true;
							break;
						}
					}
					if (!correctArg && firstArgConstant){
					validArgType = grammarTable.getSiblingValNodes(STMT_LIST);
					correctArg = false;
					for(int j = 0; j < validArgType.size(); j++){
						if (tokenType == validArgType.at(j)){
							correctArg = true;
							break;
						}

					}
				}
				}
			}
			else{
				validArgType = grammarTable.getArgument(relType,i);
				correctArg = false;
				for(int j = 0; j < validArgType.size(); j++){
					if (tokenType == validArgType.at(j)){
						correctArg = true;
						break;
					}
				}
			}			

		}
		if (!correctArg){
			error(INVALID_ARGUMENT);
			return false;
		}

		setChild(prevNode,currNode);		

	}

	if (!syn.empty()){
		setQVarGroup(syn);
	}
	return true;
}

bool QueryPreprocessor::processWith(TOKEN token){

	vector<TOKEN> comparisons;
	vector<TOKEN> syn;
	vector<string> attrAllowed;
	TOKEN currToken;
	TYPE synType;
	bool isSyn;
	bool isCorrectAttr;
	string attrType;

	headNode = createQTREENode(WITH);
	insertClause(headNode);

	comparisons = tokenize(token,"\\."+or+"\"("+ident+")\""+or+"("+attrName+")"+or+integer+or+"("+synonym+")");
	//only 1st and 3rd token can be synonyms
	isSyn = false;

	for(int i=0;i<comparisons.size();i++){
		isCorrectAttr = false;
		currToken = comparisons.at(i);
		if ((i!=comparisons.size()-1) && (comparisons.at(i+1)==".")){
			isSyn=true;
			i=i+2;
		}

		if (isSyn){	
			if (!isDeclaredVar(currToken)){
				error(INVALID_VARIABLE);
				return false;
			}
			syn.push_back(currToken);
			synType = getQVarType(currToken);
			currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));			
			setChild(headNode,currNode);

			attrAllowed = grammarTable.getAttr(synType);
			for (int j = 0; j< attrAllowed.size(); j++){
				if(comparisons.at(i)==attrAllowed.at(j)){
					isCorrectAttr = true;
					if (synType==VARIABLE||synType==PROCEDURE){
						if(attrType!="" && attrType!="string"){
							error(INVALID_QUERY_SYNTAX);
							return false;
						}
						currNode = createQTREENode(NAME);
						attrType = "string";
					}
					else if (synType==CALL){
						if((attrType=="string" || attrType=="") && comparisons.at(i)=="procName"){
							currNode = createQTREENode(NAME);
							attrType = "string";
						}
						else if ((attrType=="number" || attrType=="") && comparisons.at(i)=="stmt#"){
							currNode = createQTREENode(ANY);
							attrType = "number";
						}
						else if (attrType=="string" && comparisons.at(i)=="stmt#"){
							error(INVALID_QUERY_SYNTAX);
							return false;
						}
						else if (attrType=="number" && comparisons.at(i)=="procName"){
							error(INVALID_QUERY_SYNTAX);
							return false;
						}
					}
					else{
						if(attrType!="" && attrType!="number"){
							error(INVALID_QUERY_SYNTAX);
							return false;
						}
						currNode = createQTREENode(ANY);
						attrType = "number";
					}

					isSyn=false;
					break;
				}
			}			
			if (!isCorrectAttr){
				error(INVALID_VARIABLE);
				return false;
			}
		}
		else if (regex_match(currToken,regex("\".+\""))){
			//not syn: is string
			//want to remove the inverted commas
			currToken.erase(currToken.begin());	
			currToken.resize(currToken.size()-1);
			(*paramTable).insert(paramPair((*paramTable).size(),currToken));
			currNode = createQTREENode(ANY);
			setChild(headNode,currNode);	
			currNode = createQTREENode(PARAM,(*paramTable).size()-1);
			oneConstantClauses.push_back(clauseCount);
			int t = getQVarIndex(syn[0]);
			if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
				trackProbes.push_back(t);
			}
		}
		else if (isConstant(currToken)){		
			//not syn: is constant
			currNode = createQTREENode(ANY);
			setChild(headNode,currNode);			
			currNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
			oneConstantClauses.push_back(clauseCount);
			int t = getQVarIndex(syn[0]);
			if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
				trackProbes.push_back(t);
			}
		}
		setChild(headNode,currNode);
	}

	setQVarGroup(syn);
	return true;
}

bool QueryPreprocessor::processPattern(TOKEN token){

	vector<TOKEN> patterns;
	vector<TYPE> validArgType;
	vector<TOKEN> syn;
	TOKEN currToken;
	TYPE synType;
	int argCount;

	string expr			= "\\(*("+synonym+or+integer+")("+op+"("+synonym+or+integer+")"+"\\)*)"+optional;
	string wildexpr		= underscore+invComma+expr+invComma+underscore;

	headNode = createQTREENode(PATTERN);
	insertClause(headNode);

	patterns = tokenize(token,wildexpr+or+invComma+expr+invComma+or+underscore+or+synonym);
	//first token is the synonym
	//check if declared, and get type
	if (!isDeclaredVar(patterns.at(0))){
		error(INVALID_VARIABLE);
		return false;
	}
	else{
		syn.push_back(patterns.at(0));
		synType = getQVarType(patterns.at(0));
		argCount = patterns.size()-1;

		currNode = createQTREENode(QUERYVAR,getQVarIndex(patterns.at(0)));
		setChild(headNode,currNode);
		prevNode = currNode;
		int t = getQVarIndex(syn[0]);
		if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
			trackProbes.push_back(t);
		}
	}

	for(int i=1;i<patterns.size();i++){			
		currToken = patterns.at(i);

		//is the number of arg correct
		if (!grammarTable.getPattArgCount(synType)==argCount){			
			error(INVALID_ARGUMENT);
			return false;
		}

		//is the arg syntax correct
		if (!regex_match(currToken,regex(grammarTable.getPattArg(synType,i)))){
			error(INVALID_ARGUMENT);
			return false;			
		}

		if (i==1){	
			if(regex_match(currToken,regex(underscore))){
				//if underscore
				currNode = createQTREENode(ANY);
			}
			else if(regex_match(currToken,regex(invComma+ident+invComma))){
				//if with inverted commas
				currToken.erase(currToken.begin());	
				currToken.resize(currToken.size()-1);	
				if (!pkb->isVarExists(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				currNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));
				oneConstantClauses.push_back(clauseCount);
			}
			else{
				//if without inverted commas
				if (!isDeclaredVar(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				syn.push_back(currToken);
				currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
				int t = getQVarIndex(syn[1]);
				if(std::find(trackProbes.begin(),trackProbes.end(),t)==trackProbes.end()){
					trackProbes.push_back(t);
				}
			}
			setChild(prevNode,currNode);
		}
		else{		
			(*paramTable).insert(paramPair((*paramTable).size(),currToken));			
			currNode = createQTREENode(PARAM,(*paramTable).size()-1);
			setChild(prevNode,currNode);
			/*
			if(regex_match(currToken,regex(underscore))){
			//if underscore
			currNode = createQTREENode(ANY);
			}
			else if(regex_match(currToken,regex(wildexpr))){
			//if with inverted commas with underscore _"x+y"_

			//remove underscore and inverted commas
			currToken.erase(currToken.begin());
			currToken.erase(currToken.begin());	
			currToken.resize(currToken.size()-2);

			//build expression subtree
			//currNode = createExprTree(currToken);
			}
			else if(regex_match(currToken,regex(invComma+expr+invComma))){				
			//if with inverted commas without underscore "x+y"				
			currNode = createQTREENode(synType);
			setChild(prevNode,currNode);
			prevNode=currNode;
			currNode = createQTREENode(prevNode->getLeftSibling()->getType(),prevNode->getLeftSibling()->getData());
			setChild(prevNode,currNode);

			//remove inverted commas
			currToken.erase(currToken.begin());	
			currToken.resize(currToken.size()-1);	
			//continue with building expression subtree				
			currNode = createExprTree(currToken);
			if (currNode==NULL){
			error(INVALID_ARGUMENT);
			return false;
			}
			}
			setChild(prevNode,currNode);
			*/
		}



	}

	setQVarGroup(syn);
	return true;
}

bool QueryPreprocessor::isDeclaredVar(TOKEN token){
	dVarIter = (*dVarTable).find(token); 

	if(dVarIter==(*dVarTable).end()) 
		return false;

	return true;
}

bool QueryPreprocessor::isResultVar(TOKEN token){
	dVarIter = (*dVarTable).find(token); 

	return dVarIter->second.selected;
}

bool QueryPreprocessor::isExploredVar(TOKEN token){
	dVarIter = (*dVarTable).find(token); 

	return dVarIter->second.explored;
}

bool QueryPreprocessor::isConstant(TOKEN token){
	return regex_match(token,regex(integer));
}

bool QueryPreprocessor::hasConstantVar(int clauseNum){
	for (int i=0; i<oneConstantClauses.size(); i++){
		if (clauseNum == oneConstantClauses.at(i)){
			return true;
		}
	}
	return false;
}

void QueryPreprocessor::setAsSelected(TOKEN token){
	qVar changeVar;
	dVarIter = (*dVarTable).find(token);
	changeVar = dVarIter->second;
	changeVar.selected=true;
	(*dVarTable).erase(dVarIter);
	(*dVarTable).insert(dVarPair(token,changeVar));
}

void QueryPreprocessor::setQVarGroup(vector<TOKEN> arguments){
	int groupNum;
	string arg;
	bool prevExplored = false; //flag that the previous arguments in the same clause has been explored
	for(int i=0;i<arguments.size();i++){
		arg = arguments.at(i);
		//is explored
		if (isExploredVar(arg)){
			if (prevExplored){
				//prev arg also explored
				updateRelatedVar(arg,arguments.at(i-1));
				if (groupNum!= getQVarGroup(arg)){
					//two exists in diff groups, should merge the groups
					mergeGroup(getQVarGroup(arg),groupNum);
				}
			}
			else{
				//no prev arg
				groupNum = getQVarGroup(arg);
			}
		}

		//is not explored
		else{
			if (!prevExplored){
				//no prev arg
				groupNum=groupCount;
				groupCount++;
			}
			else{				
				updateRelatedVar(arg,arguments.at(i-1));
			}
			updateQVarGroup(arg,groupNum);
		}

		prevExplored=true;
		updateQVarClause(arg);
	}
}

void QueryPreprocessor::updateRelatedVar(TOKEN rightVar,TOKEN leftVar){
	qVar changeVar;
	dVarIter = (*dVarTable).find(rightVar);
	changeVar = dVarIter->second;
	(*dVarTable).erase(dVarIter);
	if(std::find(changeVar.leftVar.begin(),changeVar.leftVar.end(),getQVarIndex(leftVar))==changeVar.leftVar.end()){
		changeVar.leftVar.push_back(getQVarIndex(leftVar));
	}
	(*dVarTable).insert(dVarPair(rightVar,changeVar));

	dVarIter = (*dVarTable).find(leftVar);
	changeVar = dVarIter->second;
	(*dVarTable).erase(dVarIter);
	if(std::find(changeVar.rightVar.begin(),changeVar.rightVar.end(),getQVarIndex(rightVar))==changeVar.rightVar.end()){
		changeVar.rightVar.push_back(getQVarIndex(rightVar));
	}
	(*dVarTable).insert(dVarPair(leftVar,changeVar));
}

void QueryPreprocessor::updateQVarGroup(TOKEN token,int groupNum){
	qVar changeVar;
	dVarIter = (*dVarTable).find(token);
	changeVar = dVarIter->second;
	changeVar.groupNum = groupNum;
	changeVar.explored = true;	
	(*dVarTable).erase(dVarIter);
	(*dVarTable).insert(dVarPair(token,changeVar));
}

void QueryPreprocessor::updateQVarClause(TOKEN token){
	qVar changeVar;
	vector<int> clauses;
	dVarIter = (*dVarTable).find(token);
	changeVar = dVarIter->second;
	clauses = dVarIter->second.clauseNum;
	clauses.push_back(clauseCount);
	changeVar.clauseNum=clauses;
	(*dVarTable).erase(dVarIter);
	(*dVarTable).insert(dVarPair(token,changeVar));
}

void QueryPreprocessor::updateQVarClause(TOKEN token,vector<int> newClauses){
	qVar changeVar;
	vector<int> clauses;
	dVarIter = (*dVarTable).find(token);
	changeVar = dVarIter->second;
	changeVar.clauseNum = newClauses;
	(*dVarTable).erase(dVarIter);
	(*dVarTable).insert(dVarPair(token,changeVar));
}

void QueryPreprocessor::mergeGroup(int grp1, int grp2){
	//goal: merge grp2 into grp1
	vector<dVarPair> updatedVar;
	vector<hash_map<string,qVar>::const_iterator> marker;
	qVar changeVar;
	TOKEN varName;

	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
		if(dVarIter->second.groupNum==grp2){			
			changeVar = dVarIter->second;
			varName = dVarIter->first;
			changeVar.groupNum=grp1;
			updatedVar.push_back(dVarPair(varName,changeVar));
			marker.push_back(dVarIter);
		}
	}
	for(int i=0;i<marker.size();i++){
		(*dVarTable).erase(marker.at(i));
		(*dVarTable).insert(updatedVar.at(i));
	}	
}

QueryPreprocessor::dVarPair QueryPreprocessor::getQVar(int index){
	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
		if(dVarIter->second.qVarIndex == index){
			return dVarPair(dVarIter->first,dVarIter->second);
		}
	}
}

int QueryPreprocessor::getQVarGroup(TOKEN token){
	dVarIter = (*dVarTable).find(token);

	return dVarIter->second.groupNum;
}

TYPE QueryPreprocessor::getQVarType(TOKEN token){
	dVarIter = (*dVarTable).find(token);

	return dVarIter->second.qVarType;
}

TYPE QueryPreprocessor::getQVarType(int index){
	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
		if(dVarIter->second.qVarIndex == index){
			return dVarIter->second.qVarType;
		}
	}
}

int QueryPreprocessor::getQVarIndex(TOKEN token){
	//index number in qVarTable and dVarTable is the same
	dVarIter = (*dVarTable).find(token);

	return dVarIter->second.qVarIndex;
}

void QueryPreprocessor::sortQVarTable(){
	for(qVarIter=(*qVarTable).begin();qVarIter!=(*qVarTable).end();qVarIter++){
		sorted_qVarTable.push_back(qVarPair(qVarIter->first,qVarIter->second));
	}

	std::sort(sorted_qVarTable.begin(),sorted_qVarTable.end(), sortBySecond());
}

void QueryPreprocessor::insertClause(QTREE* headNode){
	clauses.push_back(headNode);
}

QTREE* QueryPreprocessor::createQTREENode(TYPE type){

	QTREE* node = new QTREE();
	node = node->createQTREENode(type,-1);

	return node;
}

QTREE* QueryPreprocessor::createQTREENode(TYPE type,int data){

	QTREE* node = new QTREE();
	node = node->createQTREENode(type,data);

	return node;
}

void QueryPreprocessor::setChild(QTREE* parent, QTREE* child){

	if (parent->getFirstDescendant()!=NULL){
		parent->getLastDescendant()->setSibling(child);
		parent->setLastDescendant(child);
	}
	else{
		parent->setFirstDescendant(child);
		parent->setLastDescendant(child);
	}
}

//Used in Old Implementation Pattern Subtree
/*
QTREE* QueryPreprocessor::createExprTree(TOKEN expr){
QTREE* exprTree;
exprPieces = tokenize(expr,"\\("+or+"\\)"+or+plus+or+minus+or+times+or+synonym+or+integer);
next = exprPieces.begin();
end = exprPieces.end();

if (next!=end)
exprTree=extractPlusMinus();

return exprTree;
}

QTREE* QueryPreprocessor::extractPlusMinus(){
QTREE* exprtree;
QTREE* subtree;
QTREE* node;	
if (next!=end)
exprtree = extractTimes();

if (exprtree == NULL){
return NULL;
}

while (next!=end && (*next=="+"||*next=="-")){
if (*next=="+"){
node = createQTREENode(PLUS);
}
else{
node = createQTREENode(MINUS);
}
next++;
subtree = extractTimes();

if (subtree == NULL){
return NULL;
}

exprtree = createSubTree(node,exprtree,subtree);
}

return exprtree;
}

QTREE* QueryPreprocessor::extractTimes(){
QTREE* exprtree;
QTREE* subtree;
QTREE* node;
if (next!=end)
exprtree = extractAll();

if (exprtree == NULL){
return NULL;
}

while (next!=end && *next=="*"){		
node = createQTREENode(MULTIPLY);
next++;
subtree = extractAll();

if (subtree == NULL){
return NULL;
}

exprtree = createSubTree(node,exprtree,subtree);
}
return exprtree;
}

QTREE* QueryPreprocessor::extractAll(){
QTREE* node;
if (regex_match(*next,regex(synonym))){
if (!pkb->isVarExists(*next)){
error(INVALID_VARIABLE);
return false;
}
node = createQTREENode(VARIABLE,pkb->getVarIndex(*next));
next++;
}
else if (regex_match(*next,regex(integer))){
node = createQTREENode(CONSTANT,atoi((*next).c_str()));
next++;
}
else if (regex_match(*next,regex("\\("))){
next++;
node = extractPlusMinus();
if (!regex_match(*next,regex("\\)"))){
error(INVALID_ARGUMENT);
return NULL;
}
next++;
}
else{
return NULL;
}

return node;
}
*/

QTREE* QueryPreprocessor::createSubTree(QTREE* head,QTREE* left, QTREE* right){
	setChild(head, left);
	setChild(head, right);

	return head;
}

vector<TOKEN> QueryPreprocessor::parse(QUERY qr){
	string expression = declare+or+result_cl+or+suchthat_cl+or+with_cl+or+pattern_cl;

	return tokenize(qr,expression);

}

vector<TOKEN> QueryPreprocessor::tokenize(TOKEN tk,string expression){		
	vector<TOKEN> tokens;
	std::regex rgx(expression);		
	std::sregex_iterator rgxIter(tk.begin(), tk.end(), rgx), rgxend;

	for (rgxIter; rgxIter != rgxend; ++rgxIter)
	{
		tokens.push_back(rgxIter->str());
	}
	return tokens;

}

void QueryPreprocessor::cleanUp(){	
	clauses.clear();
	wildClauses.clear();
	oneConstantClauses.clear();
	twoConstantClauses.clear();
	flagGroups.clear();
	//exprPieces.clear();
	sorted_qVarTable.clear();
}

void QueryPreprocessor::error(int errorCode){
	if(errorMsg.empty()){
		switch(errorCode){
		case INVALID_SYNONYM:
			errorMsg = "Query Error(): invalid synonym(s)";
			break;
		case INVALID_VARIABLE:
			errorMsg = "Query Error(): invalid variable(s)";
			break;
		case INVALID_RELATIONSHIP:
			errorMsg = "Query Error(): invalid relationship(s)";
			break;
		case INVALID_ENTITY:
			errorMsg = "Query Error(): invalid entity(s)";
			break;
		case INVALID_ARGUMENT:
			errorMsg = "Query Error(): invalid argument(s)";
			break;
		case INVALID_RESULT:
			errorMsg = "Query Error(): invalid result type(s)";
			break;
		case INVALID_QUERY_SYNTAX:
			errorMsg = "Query Error(): invalid syntax";
			break;
		default:
			errorMsg  = "Query Error(): unknown error";


		}
	}
}