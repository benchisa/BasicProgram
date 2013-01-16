#pragma once
#include "QueryPreprocessor.h"
#include <hash_map>

vector<TOKEN> *tokens;
vector<TOKEN> *declarations;
vector<TOKEN> *mainQuery;	

hash_map<int,TYPE> *qVarTable;
hash_map<int,TYPE>::const_iterator qVarIter;
hash_map<string,int> *dVarTable;
hash_map<string,int>::const_iterator dVarIter;

QueryPreprocessor::QueryPreprocessor(PKB* pkb){
	grammarTable = new GrammarTable;	
	tokens = new vector<TOKEN>;
	declarations = new vector<TOKEN>;
	mainQuery = new vector<TOKEN>;

	qVarTable = new hash_map<int,TYPE>;
	dVarTable = new hash_map<string,int>;
	this->pkb = pkb;
}

QueryPreprocessor::~QueryPreprocessor(void){
	delete grammarTable;
	delete tokens;
	delete declarations;
	delete mainQuery;

	delete qVarTable;
	delete dVarTable;
}


//--------------PUBLIC-----------------

bool QueryPreprocessor::preProcess(){

	//both declarations and main query must be valid
	//then will build qrTree & qrVarTable.
	Tokenizer tkz;
	(*tokens) = tkz.tokenize(this->query);	

	createGrammarTables();

	//split the user-provided query into 2 parts:
	//declarations and mainQuery
	splitQuery();

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
	//set qr to lower case
	std::transform(query.begin(),query.end(),query.begin(), std::bind2nd(std::ptr_fun(&std::tolower<char>), std::locale("")));
	this->query = query;
}

hash_map<int,TYPE> *QueryPreprocessor::getQVarTable(){
	return qVarTable;
}

QTREE* QueryPreprocessor::getQTree(){

	return qTree;
}

ERROR_MSG QueryPreprocessor::getLastError(){
	return this->errorMsg;
}

//--------------PRIVATE-----------------
void QueryPreprocessor::splitQuery(){

	int i = 0;
	bool startOfMainQr = false;

	for(i = 0; i < (*tokens).size(); i++){
		if (!startOfMainQr){
			if ((*tokens).at(i).compare("select") != 0){
				//if not select
				(*declarations).push_back((*tokens).at(i));

			} else {
				//seen a select
				startOfMainQr = true;
				(*mainQuery).push_back((*tokens).at(i));

			}
		} else {
			if ((*tokens).at(i).compare(";") != 0){
				(*mainQuery).push_back((*tokens).at(i));
			}
		}
	}

}

void QueryPreprocessor::createGrammarTables(){
	(*grammarTable).createEntTable();
	(*grammarTable).createRelTable();
	(*grammarTable).createPattTable();	
	(*grammarTable).createArgTable();
}

bool QueryPreprocessor::validate(){

	return (validateDeclarations(declarations) && validateMainQuery(mainQuery));

}

bool QueryPreprocessor::validateDeclarations(vector<TOKEN>* declarations){

	int i;
	TYPE entType;
	bool nextSynonym = false;
	TOKEN currToken;

	//going through each token in declarations
	for(i = 0; i < (*declarations).size(); i++){
		currToken = (*declarations).at(i);
		//not expecting a synonym
		//expecting a type declaration eg assign, while
		if (!nextSynonym){
			if ((*grammarTable).isEntExists(currToken)){
				entType = (*grammarTable).getEntType(currToken);
				nextSynonym = true;				
			}
			else {
				error(INVALID_ENTITY);
				return false;
			}

		} else {			
			//expecting a synonym to come up next
			//ignore commas
			if (!matchToken(currToken,regex(","))){
				if (isSynonym(currToken)){
					//if this synonym is already previous declared (repetition)
					if (isDeclared(currToken)){
						//check if it is a different entity type (conflict)
						if (getQVarType(currToken)!=entType){
							error(CONFLICTED_SYNONYM);
							return false;
						}
					}
					//if not previously declared, add new entry
					else{
						(*dVarTable).insert(dVarPair(currToken,(*dVarTable).size()+1));
						(*qVarTable).insert(qVarPair((*qVarTable).size()+1,entType));
					}

					//done with this synonym
					//look ahead to see if any more synonyms of the same type
					//if meet with ; means no more synonyms so next token anticipate to be a type.
					if (matchToken((*declarations).at(i+1),regex(";"))){
						i++;
						nextSynonym = false;
					}
				}
				//not a valid synonym
				else {
					error(INVALID_SYNONYM);
					return false;
				}
			}
		}

	}
	return true;
}

bool QueryPreprocessor::validateMainQuery(vector<TOKEN>* mainQuery){


	int i = 0;		
	int nextTokenType;

	//used for working with arguments within brackets ()
	numOfArg = 0;
	tokenPosition = 0;
	argPosition = 0;	
	correctArg = false;

	TOKEN currToken;

	qTree = new QTREE();
	qNode = new QTREE();
	prevNode = NULL; //used for the parts with commas
	temNode = NULL;
	resultNode = NULL;
	suchThatNode = NULL;
	patternNode = NULL;
	lastSuchThatChild = NULL;
	lastPatternChild = NULL;


	//first token will always be select
	if (!matchToken((*mainQuery).at(i),regex("select"))){
		error(INVALID_QUERY_SYNTAX);
		return false;
	}
	else{
		//create the first node of qrTree
		qTree = createQTREENode(SELECT);

		//the first child of root 'qTree' will always be result
		qNode = createQTREENode(RESULT);
		qTree->setFirstDescendant(qNode);
		qNode->setAncestor(qTree);
		resultNode = qNode;
		nextTokenType=RES;
	}

	for(i = 1; i < (*mainQuery).size(); i++){			
		currToken = (*mainQuery).at(i);
		//such that
		if(matchToken(currToken,regex("such")) && matchToken((*mainQuery).at(i+1),regex("that"))){
			nextTokenType = SUCH;
		}
		//"pattern"
		else if (matchToken(currToken,regex("pattern"))){
			nextTokenType = PATT;
		}

		switch(nextTokenType){

		case RES:
			//select bool
			if (matchToken(currToken,regex("boolean"))){
				qNode = createQTREENode(BOOL);
				resultNode->setFirstDescendant(qNode);
				qNode->setAncestor(resultNode);
				prevNode = qNode;
			}
			//select synonym
			else if (isDeclared(currToken)){ 
				qNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
				//if this were the first synonym after SELECT, prevNode will be null
				if (prevNode == NULL){
					resultNode->setFirstDescendant(qNode);
				}
				//else there was another synonym before this
				else{
					prevNode->setSibling(qNode);
				}
				qNode->setAncestor(resultNode);
				prevNode = qNode;
			}
			//anything else is wrong
			else{
				error(INVALID_RESULT);
				return false;
			}
			break;

		case SUCH:
			i++;
			nextTokenType = REL;
			//if this is the first such that, suchThatNode will be null
			if (suchThatNode == NULL){							
				qNode = createQTREENode(SUCHTHAT);
				qNode->setAncestor(qTree);
				suchThatNode = qNode;
				//if no pattern has been seen before this, patternNode will be null
				if (patternNode == NULL)
					resultNode->setSibling(qNode);
				else
					patternNode->setSibling(qNode);
			}

			break;

		case PATT:
			nextTokenType = PATT_SYNONYM;
			if (patternNode == NULL){
				qNode = createQTREENode(PATTERN);
				qNode->setAncestor(qTree);
				patternNode = qNode;
				//if no such that has been seen before this, suchThatNode will be null
				if (suchThatNode == NULL)
					resultNode->setSibling(qNode);
				else
					suchThatNode->setSibling(qNode);
			}
			break;

		case REL:
			//check if relationship name is correct
			if ((*grammarTable).isRelExists(currToken)){	
				nextTokenType = REL_ARG;
				getGrammar("rel",currToken);
				qNode = createQTREENode((*grammarTable).getRelType((currToken)));	
				//if this is the first relationship
				if (lastSuchThatChild==NULL){
					suchThatNode->setFirstDescendant(qNode);
				}
				else{
					lastSuchThatChild->setSibling(qNode);
				}					
				qNode->setAncestor(suchThatNode);
				lastSuchThatChild = qNode;
				prevNode = qNode;
			}
			//not a valid relationship
			else {
				error(INVALID_RELATIONSHIP);						
				return false;
			}
			break;

		case PATT_SYNONYM:			
			//pattern synonym must have been declared
			if(isDeclared(currToken)){
				nextTokenType = PATT_ARG;
				getGrammar("patt", getQVarType(currToken));
				qNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
				//if this is the first pattern
				if (lastPatternChild == NULL){
					patternNode->setFirstDescendant(qNode);
				}
				else{
					lastPatternChild->setSibling(qNode);
				}
				qNode->setAncestor(patternNode);
				lastPatternChild = qNode;
				prevNode = qNode;
			}
			//synonym was undeclared
			else{
				error(INVALID_SYNONYM);						
				return false;
			}
			break;		

		case REL_ARG:
			if (tokenPosition<=(numOfArg*2)){
				if (!checkArg(currToken, tokenPosition, "rel"))
					return false;
			}
			break;

		case PATT_ARG:
			if (tokenPosition<=(numOfArg*2)){
				if (!checkArg(currToken, tokenPosition, "patt"))
					return false;
			}
			break;

		default:
			error(INVALID_QUERY_SYNTAX);
			return false;
		}
	}

	return true;
}

bool QueryPreprocessor::matchToken(TOKEN token, regex reg){
	return regex_match(token,reg);

}

bool QueryPreprocessor::isSynonym(TOKEN token){

	return regex_match(token,regex("[a-zA-Z]+[a-zA-Z0-9#]*"));

}

bool QueryPreprocessor::isConstant(TOKEN token){

	return regex_match(token,regex("[0-9]+"));

}

bool QueryPreprocessor::isDeclared(TOKEN token){
	dVarIter = (*dVarTable).find(token); 

	if(dVarIter==(*dVarTable).end()) 
		return false;

	return true;
}

TYPE QueryPreprocessor::getQVarType(TOKEN token){
	dVarIter = (*dVarTable).find(token);
	int index = dVarIter->second;
	for(qVarIter=(*qVarTable).begin();qVarIter!=(*qVarTable).end();qVarIter++){
		if(qVarIter->first == index){
			return qVarIter->second;
		}
	}
}

TYPE QueryPreprocessor::getQVarType(int index){
	for(qVarIter=(*qVarTable).begin();qVarIter!=(*qVarTable).end();qVarIter++){
		if(qVarIter->first == index){
			return qVarIter->second;
		}
	}
}

int QueryPreprocessor::getQVarIndex(TOKEN token){
	//index number in qVarTable and dVarTable is the same
	dVarIter = (*dVarTable).find(token);

	return dVarIter->second;
}

vector<TOKEN> QueryPreprocessor::tokenizeGrammar(string grammar){
	vector<TOKEN> arg;
	std::regex rgx("[0-9a-zA-Z\"\\(\\)\\?\s\\[\\]\\|\\\\\\-\\*\\+_#]+");		
	std::sregex_iterator rgxIter(grammar.begin(), grammar.end(), rgx), rgxend;

	for (rgxIter; rgxIter != rgxend; ++rgxIter)
	{
		arg.push_back(rgxIter->str());
	}

	return arg;
}

void QueryPreprocessor::getGrammar(string tableType, string data){

	string grammar;

	if (tableType.compare("rel")==0){		
		grammar = (*grammarTable).getRelGrammar(data);
		arguments = tokenizeGrammar(grammar);	
		numOfArg = arguments.size();
	} 
	
}

void QueryPreprocessor::getGrammar(string tableType, TYPE data){

	string grammar;

	if (tableType.compare("patt")==0){
		grammar = (*grammarTable).getPattGrammar(data);
		arguments = tokenizeGrammar(grammar);	
		numOfArg = arguments.size();
	} 
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

bool QueryPreprocessor::checkArg(TOKEN currToken, int tokenPosition, string arg_type){

	TYPE relType,tokenType;
	vector<TYPE> validTypes;
	if (tokenPosition==0){
		if (!matchToken(currToken,regex("\\("))){
			error(MISSING_BRACKET);
			return false;
		}
		this->tokenPosition++;
	}
	else if(tokenPosition==numOfArg*2){
		if (!matchToken(currToken,regex("\\)"))){
			error(MISSING_BRACKET);
			return false;
		}	
		//finish all tokens within brackets
		else {
			this->tokenPosition=0;
			this->argPosition=0;
			this->numOfArg=0;
		}
	}
	else if(tokenPosition%2==0){
		if (!matchToken(currToken,regex(","))){
			error(MISSING_ARGUMENT);
			return false;
		}
		this->tokenPosition++;
	}
	else if(tokenPosition%2!=0){
		//check that it match the expected argument syntax
		if (matchToken(currToken,regex(arguments.at(argPosition)))){
			// if its not in inverted comma "" or a underscore
			if (!matchToken(currToken,regex("\"[a-zA-Z0-9]+\"|_|_\"[a-zA-Z0-9]+\"_"))){
				//if its a declared variable
				if (isDeclared(currToken)){
					if (arg_type == "rel"){
						//if its the correct entity type
						tokenType = getQVarType(currToken);
						//if prevNode is lastSuchThatChild means this is first argument
						//if prevNode's ancestor is lastSuchThatChild means this is second argument
						relType = lastSuchThatChild->getType();
						//retrieve allowed argument
						validTypes = (*grammarTable).getArgument(relType,argPosition);
						//check for any match with the allowed argument types
						for(int j = 0; j < validTypes.size(); j++){
							if (tokenType == validTypes.at(j)){
								correctArg = true;
								break;
							}
						}
					}
					else if (arg_type == "patt" && getQVarType(currToken) == VARIABLE){
						correctArg = true;
					}


					//if we found a match with an allowed argument type eg ASSIGNMENT or WHILE
					if (correctArg){
						correctArg = false;
						qNode = createQTREENode(QUERYVAR,getQVarIndex(currToken));
						if (argPosition==0){
							//first child, prevNode will be lastSuchThatChild/lastPatternChild
							prevNode->setFirstDescendant(qNode); 
							qNode->setAncestor(prevNode);
						}
						else{
							//not first child, prevNode is prev argument
							prevNode->setSibling(qNode);
							qNode->setAncestor(prevNode->getAncestor());
						}
						prevNode = qNode;
					}
					//not correctArg
					else{

						error(INVALID_ARGUMENT);
						return false;
					}


				}
				//synonym not declared, could be a constant, regarded as statement
				else if (arg_type == "rel" && isConstant(currToken)){
					tokenType = STATEMENT;

					relType = lastSuchThatChild->getType();
					//retrieve allowed argument
					validTypes = (*grammarTable).getArgument(relType,this->argPosition);
					//check for any match with the allowed argument types
					for(int j = 0; j < validTypes.size(); j++){
						if (tokenType == validTypes.at(j)){
							correctArg = true;
							break;
						}
					}
					//if we found a match with an allowed argument type eg STATEMENT
					if (correctArg){
						correctArg = false;
						qNode = createQTREENode(STATEMENT,atoi(currToken.c_str()));
						if (argPosition==0){
							//first child, prevNode will be lastSuchThatChild
							prevNode->setFirstDescendant(qNode); 
							qNode->setAncestor(prevNode);
						}
						else{
							//not first child, prevNode is prev argument
							prevNode->setSibling(qNode);
							qNode->setAncestor(prevNode->getAncestor());
						}
						prevNode = qNode;
					}
					//not correctArg
					else{
						error(INVALID_ARGUMENT);
						return false;
					}


				}
				//not declared, not constant
				else{
					error(INVALID_SYNONYM);
					return false;
				}
			}
			// if its in inverted comma "" or a underscore
			// if Modifies/uses, must check if valid variable from VarTable
			else if (arg_type == "rel"){
				//if underscore
				if(matchToken(currToken,regex("_"))){
					qNode = createQTREENode(ANY);
					if (argPosition==0){
						//first child, prevNode will be lastSuchThatChild
						prevNode->setFirstDescendant(qNode); 
						qNode->setAncestor(prevNode);
					}
					else{
						//not first child, prevNode is prev argument
						prevNode->setSibling(qNode);
						qNode->setAncestor(prevNode->getAncestor());
					}
					prevNode = qNode;
				}
				//else in ""
				//"x" as a synonym
				else { 						
					tokenType = VARIABLE;

					relType = lastSuchThatChild->getType();
					//retrieve allowed argument
					validTypes = (*grammarTable).getArgument(relType,argPosition);
					//check for any match with the allowed argument types
					for(int j = 0; j < validTypes.size(); j++){
						if (tokenType == validTypes.at(j)){
							correctArg = true;
							break;
						}
					}
					//if we found a match with an allowed argument type VARIABLE
					if (correctArg){
						correctArg = false;
						//check if this is a valid variable in VarTable
						currToken = currToken.substr(1,currToken.size()-2);
						if (!pkb->isVarExists(currToken)){
							error(INVALID_VARIABLE);
							return false;
						}
						else{
							qNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));
							if (argPosition==0){
								//first child, prevNode can be suchThatNode or patternNode
								prevNode->setFirstDescendant(qNode); 
								qNode->setAncestor(prevNode);
							}
							else{
								//not first child, prevNode is prev argument
								prevNode->setSibling(qNode);
								qNode->setAncestor(prevNode->getAncestor());
							}
							prevNode = qNode;
						}
					}
					else{
						error(INVALID_ARGUMENT);
						return false;
					}

				}

			}
			else if (arg_type == "patt"){
				//if underscore
				if(matchToken(currToken,regex("_|\"_\""))){
					if (argPosition==0){
						//first child, prevNode will be lastSuchThatChild										
						qNode = createQTREENode(ANY);
						prevNode->setFirstDescendant(qNode); 
						qNode->setAncestor(prevNode);
					}
					else{
						//not first child, prevNode is prev argument
						//second child: if just a single _ denote ANY, 
						//		PATTERN|-1
						//			/
						//	QUERYVAR|k
						//		/	\
						//	1st arg  _

						qNode = createQTREENode(ANY);
						prevNode->setSibling(qNode);
						qNode->setAncestor(prevNode->getAncestor());
					}
					prevNode = qNode;
				}
				//else in ""
				//"x" or _"x"_
				else { 	
					//first argument
					if (argPosition == 0){
						//shouldnt have "_x_"
						if (!matchToken(currToken,regex("_\".+\"_"))){		
							//strip the ""
							//check that this is a valid variable
							//not existing variable
							currToken = currToken.substr(1,currToken.size()-2);
							if (!isConstant(currToken) && !pkb->isVarExists(currToken)){
								error(INVALID_VARIABLE);
								return false;
							}
							else if (isConstant(currToken)){											
								qNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
							}
							else {	
								qNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));
							}											
							prevNode->setFirstDescendant(qNode); 
							qNode->setAncestor(prevNode);
						}
						else{
							error(INVALID_ARGUMENT);
							return false;
						}
					}
					//position not 0, means not first child/argument
					//not first child, prevNode is prev argument

					else {
						//strip "" or _ _ 
						currToken = currToken.substr(1,currToken.size()-2);
						//if its "x"
						//
						//		PATTERN|-1
						//			/
						//	QUERYVAR|k
						//		/	\
						//	1st arg ASSIGNMENT|-1
						//			/	\
						//		1st arg  x

						if (!matchToken(currToken,regex("\".+\""))){
							//have to check if it is an existing variable
							//not constant and not existing variable
							if (!isConstant(currToken) && !pkb->isVarExists(currToken)){
								error(INVALID_VARIABLE);
								return false;
							}
							
							qNode = createQTREENode(getQVarType(prevNode->getAncestor()->getData()));
							prevNode->setSibling(qNode);
							qNode->setAncestor(prevNode->getAncestor());
							temNode = prevNode;
							prevNode = qNode;
							qNode = createQTREENode(temNode->getType(),temNode->getData());
							prevNode->setFirstDescendant(qNode);
							qNode->setAncestor(prevNode);
							prevNode = qNode;
							
							if (isConstant(currToken)){		
								qNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
							}
							else {
								qNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));
							}

							prevNode->setSibling(qNode);
							qNode->setAncestor(prevNode->getAncestor());
						}
						//if its _"x"_
						//
						//		PATTERN|-1
						//			/
						//	QUERYVAR|k
						//		/	\
						//	1st arg  x

						else{
							//strip the inverted commas
							currToken = currToken.substr(1,currToken.size()-2);
							if (!isConstant(currToken) && !pkb->isVarExists(currToken)){
								error(INVALID_VARIABLE);
								return false;
							}
							if (isConstant(currToken)){					
								qNode = createQTREENode(CONSTANT,atoi(currToken.c_str()));
							}
							else {
								qNode = createQTREENode(VARIABLE,pkb->getVarIndex(currToken));
							}
							prevNode->setSibling(qNode);
							qNode->setAncestor(prevNode->getAncestor());
						}
					}
					
					prevNode = qNode;
				}
			}
			this->argPosition++;
		}
		//invalid argument syntax
		else{
			error(INVALID_ARGUMENT);
			return false;
		}
		this->tokenPosition++;
	}
	return true;
}

void QueryPreprocessor::cleanUp(){
	(*tokens).erase((*tokens).begin(),(*tokens).end());
	(*tokens).clear();
	(*declarations).erase((*declarations).begin(),(*declarations).end());
	(*declarations).clear();
	(*mainQuery).erase((*mainQuery).begin(),(*mainQuery).end());
	(*mainQuery).clear();
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
			errorMsg = "Query Error(): missing select/such that/pattern";
			break;
		case MISSING_BRACKET:
			errorMsg = "Query Error(): missing bracket";
			break;
		case MISSING_ARGUMENT:
			errorMsg = "Query Error(): missing argument";
			break;
		case CONFLICTED_SYNONYM:
			errorMsg = "Query Error(): conflicted synonym";
			break;
		default:
			errorMsg  = "Query Error(): unknown error";


		}
	}
}
