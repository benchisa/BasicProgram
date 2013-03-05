#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include "QTREE.h"
#include "GrammarTable.h"
#include <hash_map>
#include <ctype.h>

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
	hash_map<int,string> *getParamTable();
	bool getSelectBool();
	ERROR_MSG getLastError();

private:
	
	PKB *pkb;	
	QUERY query;		
	GrammarTable grammarTable;
	vector<TOKEN> *tokens;	
	bool selectBool;
	bool keepUpperCase;
	
	struct qVar{
		int qVarIndex;
		int groupNum;
		TYPE qVarType;
		bool selected;
		bool explored;
		vector<int> clauseNum;
	};
	
	struct sortBySecond {
		bool operator()(const std::pair<int,TYPE> &left, const std::pair<int,TYPE> &right) {
			return left.second < right.second;
		}
	};

	//qVarTable keeps track of the synonym's number and type
	//qVarTable is the final data structure to pass to Evaluator
	hash_map<int,TYPE> *qVarTable;
	hash_map<int,TYPE>::const_iterator qVarIter;

	//paramTable keeps track of the string for "with"
	hash_map<int,string> *paramTable;
	hash_map<int,string>::const_iterator paramIter;

	//dVarTable keeps track of the declared synonym and its corresponding number
	//dVarTable is only used internally
	hash_map<string,qVar> *dVarTable;
	hash_map<string,qVar>::const_iterator dVarIter;

	typedef pair<int,TYPE> qVarPair;	
	typedef pair<int,string> paramPair;	
	typedef pair<string,qVar> dVarPair;			
	
	vector<QTREE*> clauses; //keeps 'SUCHTHAT','WITH' and 'PATTERN' headNodes
	vector<int> wildClauses; //keeps the clause numbers of the clauses that has two wilds "_"
	vector<int> twoConstantClauses; //keeps the clause numbers of the clauses that has two constants
	vector<int> oneConstantClauses;
	vector<int> flagGroups;
	vector<qVarPair> sorted_qVarTable;

	vector<TOKEN> exprPieces;
	//iterators for expression tree
	std::vector<TOKEN>::iterator next;
	std::vector<TOKEN>::iterator end;
	
	int clauseCount; //keeps track of how many condition clauses are there
	int groupCount;	

	QTREE* firstNode;
	QTREE* currNode;
	QTREE* prevNode;
	QTREE* headNode;
	
	//helpers for rearranging clauses
	QTREE* firstAffect;
	QTREE* firstFollowsNext;
	QTREE* firstUsesMod;
	QTREE* firstCallPar;
	QTREE* firstWithPatt;
	QTREE* lastAffect;
	QTREE* lastFollowsNext;
	QTREE* lastUsesMod;
	QTREE* lastCallPar;
	QTREE* lastWithPatt;

	QueryPreprocessor::ERROR_MSG errorMsg;

	//regex
	string compulsoryOne,optional,or,underscore,hash,invComma,letter,digit,plus,minus,times;
	string op,integer,ident,synonym,rel,ref;
	string attrName,attrRef,attrCompare;
	string designEnt,elem,tuple;
	string declare,result_cl,suchthat_cl,with_cl,pattern_cl;
	
	//helper: prepare to validate
	void createGrammarTables();
	vector<TOKEN> parse(QUERY);
	vector<TOKEN> tokenize(TOKEN,string);

	//validating
	bool validate();

	//helper: validating
	bool isConstant(TOKEN);
	bool isDeclaredVar(TOKEN);
	bool isResultVar(TOKEN);
	bool isExploredVar(TOKEN);
	bool hasConstantVar(int);
	bool verifyDeclaration(TOKEN);
	bool verifySelect(TOKEN);
	bool verifyCondition(TOKEN);
	bool processSuchThat(TOKEN);
	bool processWith(TOKEN);
	bool processPattern(TOKEN);

	void setAsSelected(TOKEN);
	void setQVarGroup(vector<TOKEN>);
	void updateQVarGroup(TOKEN,int);
	void updateQVarClause(TOKEN);
	void mergeGroup(int,int);
	dVarPair getQVar(int);
	int getQVarGroup(TOKEN);
	TYPE getQVarType(TOKEN);
	TYPE getQVarType(int);
	int getQVarIndex(TOKEN);
	void sortQVarTable();
	void createQVarTable(int);
	QTREE* createQTREENode(TYPE);
	QTREE* createQTREENode(TYPE,int);
	void setChild(QTREE* parent, QTREE* child);
	QTREE* createSubTree(QTREE*,QTREE*,QTREE*);
	QTREE* createExprTree(TOKEN);
	QTREE* extractPlusMinus();	
	QTREE* extractTimes();
	QTREE* extractAll();
	void insertClause(QTREE*);

	//setup Query Tree for evaluator's use
	void setQTree();
	void arrangeClauseByRel(QTREE*);
	void joinClauses();
	bool isFlaggedGroup(int);

	//clean up memory
	void cleanUp();

	//error management
	void error(int);

};

