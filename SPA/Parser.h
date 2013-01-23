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
	STATEMENT_NUM stmt_num;
	vector<pair<TOKEN, PROG_LINE>> tokens;
	Parser::ERROR_MSG errorMsg;
	int tokenIndex;
	int nesting;
	
	// PKB related helper
	PROC_INDEX curProcIndex;
	VAR_INDEX curVarIndex;
	vector<pair<STATEMENT_NUM, TYPE>> containerIndex;
	int lastProcEndLine;
	pair<PROG_LINE, TYPE> containerInfo;
	PKB *pkb;
	AST *ast,*curAST, *prevAST;
	PROC_NAME curProc;
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
	void insertFollows(STATEMENT_NUM, STATEMENT_NUM);
	void insertFollowsParentForStmt(STATEMENT_NUM, STATEMENT_NUM);
	void insertFollowsParentForCon(TOKEN, STATEMENT_NUM, STATEMENT_NUM);
	void createExprTree();
	void createASTLink(AST*);
};

class ParserTokenizer{
public:
	static vector<pair<TOKEN, PROG_LINE>> tokenize(SOURCE src){
		vector<pair<TOKEN, PROG_LINE>> tokens;
		pair<TOKEN, PROG_LINE> tmp;
		int progline = 1;
		regex rx("call|while|procedure|if|then|else|\\n|\\(|\\)|\\{|\\}|\\=|\\;|\\-|\\*|\\+|[^\\s\\n\\{\\}\\-\\*\\+\\;\\=\\(\\)]+");
		sregex_iterator rxItr(src.begin(), src.end(), rx), rxend;

		for (rxItr; rxItr != rxend; ++rxItr)
		{
			if(rxItr->str() != "\n")
			{
				tmp.first = rxItr->str();
				tmp.second = progline;
				tokens.push_back(tmp);
			}
			else
			{
				progline++;
			}
		}
		return tokens;
	}
	
	//debug
	void printAll(vector<pair<TOKEN, PROG_LINE>> tmp){
		int i = 0;
		for(i = 0; i < tmp.size(); i++){
			//cout << "prog_line: " << tmp.at(i).second << ",token " << i << ": " << tmp.at(i).first + "\n";
		}
	}
};