#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include <stack>
using namespace std;

class Parser
{
	
public:
	Parser(void);
	~Parser(void);

	enum ERROR_TYPE{
		INVALID_PROCEDURE,
		INVALID_PROCEDURE_NAME,
		INVALID_WHILE_NAME,
		MISSING_COLON,
		MISSING_OPERATOR,
		INVALID_EXPR,
		INVALID_STMTLST,
		MISSING_BRACKET,
		INVALID_IF_NAME,
		INVALID_IF_FORMAT,
		INVALID_CALL_NAME
	};

	SOURCE getSource();
	void setSource(SOURCE src);
	int startParse();
	ERROR_MSG getLastError();
	PKB* getPKB();

private:
	SOURCE src;
	TOKEN curToken, prevToken;
	PROG_LINE progLine;
	vector<TOKEN> tokens;
	Parser::ERROR_MSG errorMsg;
	int tokenIndex;
	int nesting;
	
	// PKB related helper
	PROC_INDEX curProcIndex;
	VAR_INDEX curVarIndex;
	vector<pair<PROG_LINE, TYPE>> containerIndex;
	int lastProcEndLine;
	pair<PROG_LINE, TYPE> containerInfo;
	PKB *pkb;
	AST *ast,*curAST, *prevAST;
	//int procLine;
	
	bool parse();

	// HELPER methods
	stack<AST*> operators;
	stack<AST*> operands;
	bool sibling;
	bool matchToken(TOKEN token);
	bool matchToken(regex reg);
	void error(int);

	// Various Grammar
	bool program(void);
	bool procedure(void);
	bool stmtlst(void);
	bool stmt(void);
	bool stmt_while(void);
	bool stmt_call(void);
	bool stmt_if(void);
	bool stmt_assign(void);
	bool factor(void); 
	bool expr(void);
	bool term(void);
	bool const_value(void);
	bool name(void); // handles proc_name and var_name
	bool openBracket(void);
	bool closeBracket(void);
	
	// Helper for inserting relationship
	bool isSameProc(PROG_LINE, PROG_LINE);
	void insertFollows(PROG_LINE, PROG_LINE);
	void insertFollowsParentForStmt(PROG_LINE, PROG_LINE);
	void insertFollowsParentForCon(TOKEN, PROG_LINE, PROG_LINE);
	void createExprTree();
	void createASTLink(AST*);
};

class ParserTokenizer{
public:
	static vector<TOKEN> tokenize(SOURCE src){
		vector<TOKEN> tokens;
		regex rx("call|while|procedure|if|then|else|\\(|\\)|\\{|\\}|\\=|\\;|\\-|\\*|\\+|[^\\s\\n\\{\\}\\-\\*\\+\\;\\=\\(\\)]+");
		sregex_iterator rxItr(src.begin(), src.end(), rx), rxend;

		for (rxItr; rxItr != rxend; ++rxItr)
		{
			tokens.push_back(rxItr->str());
		}
		return tokens;
	}
	
	//debug
	void printAll(vector<TOKEN> tmp){
		int i = 0;
		for(i = 0; i < tmp.size(); i++){
			cout << "token " << i << ": " << tmp.at(i) + "\n";
		}
	}
};