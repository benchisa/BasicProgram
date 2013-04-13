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
	
	struct qVar{
		int qVarIndex;
		int groupNum;
		TYPE qVarType;
		bool selected;
		bool explored;
		vector<int> rightVar;
		vector<int> leftVar;
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
	vector<int> trackProbes;
	vector<qVarPair> sorted_qVarTable;

	//vector<TOKEN> exprPieces;
	//iterators for expression tree
	//std::vector<TOKEN>::iterator next;
	//std::vector<TOKEN>::iterator end;
	
	int clauseCount; //keeps track of how many condition clauses are there
	int groupCount;	

	QTREE* firstNode;
	QTREE* currNode;
	QTREE* prevNode;
	QTREE* headNode;
	
	//helpers for rearranging clauses
	QTREE* firstWithPatt;
	QTREE* firstFollows; 
	QTREE* firstFollowsSt;
	QTREE* firstUsesMod_Proc;
	QTREE* firstUsesMod_Call;
	QTREE* firstUsesMod_Stmt;
	QTREE* firstUsesMod_Assign;
	QTREE* firstUsesMod_Container;
	QTREE* firstUsesMod;
	QTREE* firstCall;
	QTREE* firstCallSt;
	QTREE* firstParent_KU;
	QTREE* firstParent_UK;
	QTREE* firstParent;
	QTREE* firstParentSt_KU;
	QTREE* firstParentSt_UK;
	QTREE* firstParentSt;
	QTREE* firstAffect;
	QTREE* firstAffectSt;
	QTREE* firstNext;
	QTREE* firstNextSt;
	QTREE* firstContainSibling;
	QTREE* lastWithPatt;
	QTREE* lastFollows; 
	QTREE* lastFollowsSt;
	QTREE* lastUsesMod_Proc;
	QTREE* lastUsesMod_Call;
	QTREE* lastUsesMod_Stmt;
	QTREE* lastUsesMod_Assign;
	QTREE* lastUsesMod_Container;
	QTREE* lastUsesMod;
	QTREE* lastCall;	
	QTREE* lastCallSt;
	QTREE* lastParent_KU;
	QTREE* lastParent_UK;
	QTREE* lastParent;
	QTREE* lastParentSt_KU;
	QTREE* lastParentSt_UK;
	QTREE* lastParentSt;
	QTREE* lastAffect;
	QTREE* lastAffectSt;
	QTREE* lastNext;
	QTREE* lastNextSt;
	QTREE* lastContainSibling;
	

	QueryPreprocessor::ERROR_MSG errorMsg;

	//regex
	string compulsoryOne,optional,or,underscore,hash,invComma,letter,digit,plus,minus,times;
	string op,integer,ident,synonym,rel,ref,nodeRef;
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
	void updateQVarClause(TOKEN,vector<int>);
	void updateRelatedVar(TOKEN,TOKEN);
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
	int chain(int);
	void setQTree();
	void arrangeClauseByRel(QTREE*);
	void joinClauses();
	void processClauses(vector<int>,int);
	vector<QTREE*> addToProbe(QTREE*);
	bool isFlaggedGroup(int);

	//clean up memory
	void cleanUp();

	//error management
	void error(int);

};

