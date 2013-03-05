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
	rel				= "Uses|Modifies|Follows\\*|Follows|Parent\\*|Parent|Affects\\*|Affects|Calls\\*|Calls|Next\\*|Next";
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
	delete paramTable;
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
	dVarPair dpair;
	
	firstAffect = createQTREENode(ANY);
	firstFollowsNext = createQTREENode(ANY);
	firstUsesMod = createQTREENode(ANY);
	firstCallPar = createQTREENode(ANY);
	firstWithPatt = createQTREENode(ANY);

	//prepare sort
	sortQVarTable();
		
	//for every variable
	for(dVarIter=(*dVarTable).begin();dVarIter!=(*dVarTable).end();dVarIter++){
		//check if its selected
		if(dVarIter->second.selected){
			//get selected's groupNum
			grpNum = dVarIter->second.groupNum;
			//check if the group is already flagged
			if (!isFlaggedGroup(grpNum)){
				//add flag 
				flagGroups.push_back(grpNum);
			}
		}
	}

	//insert into QTREE the clauses with both constants first
	for (int i=0; i<twoConstantClauses.size(); i++){
		currNode = clauses.at(twoConstantClauses.at(i));
		if (currNode!=NULL){
			arrangeClauseByRel(currNode);
			clauses.at(twoConstantClauses.at(i)) = NULL;
		}
	}

	joinClauses();

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
	*/

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
		//choose those not flagged for selected
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
	/*
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
	*/
	
	for (int i=0; i<clauses.size(); i++){
		currNode = clauses.at(i);
		if (currNode!=NULL){
			arrangeClauseByRel(currNode);
			clauses.at(i) = NULL;
		}
	}

	joinClauses();
	
}

void QueryPreprocessor::arrangeClauseByRel(QTREE* node){
	TYPE relType;	
	

	relType = node->getFirstDescendant()->getType();
	if (node->getType()==PATTERN || node->getType()==WITH){
		if (firstWithPatt->getType()==ANY){
			firstWithPatt = node;
		}
		else{
			lastWithPatt->setSibling(node);
		}
		lastWithPatt = node;
	}
	else if (relType==CALL||relType==PARENT||relType==CALLST||relType==PARENTST){
		if (firstCallPar->getType()==ANY){
			firstCallPar = node;
		}
		else{
			lastCallPar->setSibling(node);
		}
		lastCallPar = node;
	}
	else if (relType==USES||relType==MODIFIES){
		if (firstUsesMod->getType()==ANY){
			firstUsesMod = node;
		}
		else{
			lastUsesMod->setSibling(node);
		}
		lastUsesMod = node;
	}
	else if (relType==FOLLOWS||relType==FOLLOWST||relType==NEXT||relType==NEXTST){
		if (firstFollowsNext->getType()==ANY){
			firstFollowsNext = node;
		}
		else{
			lastFollowsNext->setSibling(node);
		}
		lastFollowsNext = node;
	}
	else if (relType==AFFECTS||relType==AFFECTST){
		if (firstAffect->getType()==ANY){
			firstAffect = node;
		}
		else{
			lastAffect->setSibling(node);
		}
		lastAffect = node;
	}
}

void QueryPreprocessor::joinClauses(){

	if (firstWithPatt->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstWithPatt);
		firstNode->setLastSibling(lastWithPatt);		
	}

	if (firstCallPar->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstCallPar);
		firstNode->setLastSibling(lastCallPar);		
	}

	if (firstUsesMod->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstUsesMod);
		firstNode->setLastSibling(lastUsesMod);		
	}

	if (firstFollowsNext->getType()!=ANY){
	firstNode->getLastSibling()->setSibling(firstFollowsNext);
		firstNode->setLastSibling(lastFollowsNext);		
	}

	if (firstAffect->getType()!=ANY){
		firstNode->getLastSibling()->setSibling(firstAffect);
		firstNode->setLastSibling(lastAffect);		
	}

	
	firstAffect = createQTREENode(ANY);
	firstFollowsNext = createQTREENode(ANY);
	firstUsesMod = createQTREENode(ANY);
	firstCallPar = createQTREENode(ANY);
	firstWithPatt = createQTREENode(ANY);
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
	string relName;
	TYPE relType;
	TYPE tokenType;

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
			if (relName=="Modifies_p"||relName=="Uses_p"||relName=="Calls"||relName=="Calls*"){
				if (!pkb->isProcExists(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				tokenType = PROCEDURE;
				currNode = createQTREENode(PROCEDURE,pkb->getProcIndex(currToken));
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
				oneConstantClauses.push_back(clauseCount);
			}
		}
		//can also be wildcards _
		else if(currToken=="_"){
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
		}
		//check is the synonym declared
		else if (isDeclaredVar(currToken)){
			tokenType = getQVarType(currToken);			
			syn.push_back(currToken);			
			currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
			if (prevArgConstant){
				oneConstantClauses.push_back(clauseCount);
			}
		}
		//even if not declared,could be a constant statement number
		else if (isConstant(currToken)){
			if (relName=="Next"||relName=="Next*"){
				tokenType = PROGLINE;
				currNode = createQTREENode(PROGLINE,atoi(currToken.c_str()));
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
				}
			}
		}
		
		//not declared, not constant
		else{
			error(INVALID_ARGUMENT);
			return false;		
		}

		//check argument's type
		validArgType = grammarTable.getArgument(relType,i);
		correctArg = false;
		for(int j = 0; j < validArgType.size(); j++){
			if (tokenType == validArgType.at(j)){
				correctArg = true;
				break;
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
						if(attrType=="" && comparisons.at(i)=="procName"){
							currNode = createQTREENode(NAME);
							attrType = "string";
						}
						else if (attrType=="" && comparisons.at(i)=="stmt#"){
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
		}
		else if (isConstant(currToken)){		
		//not syn: is constant
			currNode = createQTREENode(ANY);
			setChild(headNode,currNode);			
			currNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
			oneConstantClauses.push_back(clauseCount);
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
			}
			else{
				//if without inverted commas
				if (!isDeclaredVar(currToken)){
					error(INVALID_VARIABLE);
					return false;
				}
				syn.push_back(currToken);
				currNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
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
			updateQVarGroup(arg,groupNum);
		}
		
		prevExplored=true;
		updateQVarClause(arg);
	}
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
	exprPieces.clear();
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