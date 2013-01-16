#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "GrammarTable.h"

class QueryPreprocessor{

public:
	enum ERROR_TYPE{
		INVALID_SYNONYM,
		INVALID_VARIABLE,
		INVALID_RELATIONSHIP,
		INVALID_ENTITY,
		INVALID_ARGUMENT,
		INVALID_RESULT,
		INVALID_QUERY_SYNTAX,
		MISSING_BRACKET,
		MISSING_ARGUMENT,
		CONFLICTED_SYNONYM
	};	

	enum TOKEN_TYPE{
		RES,
		SUCH,
		PATT,
		REL,
		REL_ARG,
		PATT_ARG,
		PATT_SYNONYM
	};
	
	QueryPreprocessor(PKB* pkb);
	~QueryPreprocessor(void);
	
	bool preProcess();
	void setQuery(QUERY query);
	QTREE* getQTree();
	hash_map<int,TYPE> *getQVarTable();
	ERROR_MSG getLastError();

private:
	
	PKB *pkb;	
	QUERY query;		
	GrammarTable* grammarTable;
	
	typedef pair<string,int> dVarPair;				
	typedef pair<int,TYPE> qVarPair;		
	
	QTREE* qTree;
	QTREE* qNode;
	QTREE* prevNode;
	QTREE* temNode;
	QTREE* resultNode;
	QTREE* suchThatNode;
	QTREE* patternNode;
	QTREE* lastSuchThatChild;
	QTREE* lastPatternChild;
		
	QueryPreprocessor::ERROR_MSG errorMsg;

	//used for working within the brackets
	vector<TOKEN> arguments;
	int numOfArg;
	int tokenPosition;
	int argPosition;
	int nextTokenType;
	bool correctArg;


	//helper: prepare to validate
	void splitQuery();
	void createGrammarTables();

	//validating
	bool validate();
	bool validateDeclarations(vector<TOKEN>* declarations);
	bool validateMainQuery(vector<TOKEN>* mainQuery);
	
	//helper: validating
	bool matchToken(TOKEN token, regex reg);
	bool isSynonym(TOKEN token);	
	bool isConstant(TOKEN token);
	bool isDeclared(TOKEN token);
	TYPE getQVarType(TOKEN token);
	TYPE getQVarType(int index);
	int getQVarIndex(TOKEN token);
	vector<TOKEN> tokenizeGrammar(string grammar);
	void getGrammar(string tableType, string data);
	void getGrammar(string tableType, TYPE data);
	QTREE* createQTREENode(TYPE type);
	QTREE* createQTREENode(TYPE type,int data);
	bool checkArg(TOKEN token, int tokenPosition, string arg_type);

	//prepare data structure for query evaluator
	bool createqrVarTable();
	bool createQrTree();

	//clean up memory
	void cleanUp();

	//error management
	void error(int);
	
};

//HELPER CLASSES

class Tokenizer{
public:
	vector<TOKEN> tokenize(QUERY qr){
		vector<TOKEN> tokens;
		string ident = "[a-zA-Z]+[a-zA-Z0-9#]*";
		string integer = "[0-9]+";
		string underscore = "_";
		string synonym = ident;
		
		string expression = "assign|while|prog_line|stmt|constant|variable|procedure|stmtlst|,|select|boolean|such\sthat|pattern|follows(\\*)+|parent(\\*)+|uses|modifies|\\(|"+synonym+"|"+integer+"|\""+integer+"\"|\""+ident+"\"|_\""+ident+"\"_|_\""+integer+"\"_|"+underscore+"|\\)|;";
		
		std::regex rgx(expression);
		
		std::sregex_iterator rgxIter(qr.begin(), qr.end(), rgx), rgxend;

		for (rgxIter; rgxIter != rgxend; ++rgxIter)
		{
			tokens.push_back(rgxIter->str());
		}
		return tokens;
	}

	//for debugging
	void printAll(TOKEN tmp){
		int i = 0;
		for(i = 0; i < tmp.size(); i++){
			//cout << "token " << i << ": " << tmp.at(i) + "\n";
		}
	}
};