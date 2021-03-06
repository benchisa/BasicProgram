#pragma once
#include "Global_Data_Abstraction.h"
#include "PKB.h"
#include <stack>
#include "DesignExtractor.h"
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
	PROG_LINE prevProgLine, curProgLine;
	EXPRESSION exp;
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
			//if(rxItr->str() == "\n")
			//	cout << "asd\n";

			if(rxItr->str() != ";" && rxItr->str() != "\n")
			{
				//cout << "asd\n";
				if(rxItr->str() == "}")
				{
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					progline--;
					//cout << "1: " << tmp.first << ", " << tmp.second << "\n";

				}
				
				if(rxItr->str() == "{")
				{
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					//cout << "8: " << tmp.first << ", " << tmp.second << "\n";

					progline++;
				}
				else if(rxItr->str() != "else" && rxItr->str() != "procedure")
				{	
					if(rxItr->str() == "}"){
						tmp.first = rxItr->str();
						tmp.second = 0;
						progline++;
					}
					else{
						tmp.first = rxItr->str();
						tmp.second = progline;
						tokens.push_back(tmp);
					}
					
					//cout << "7: " << tmp.first << ", " << tmp.second << "\n";

				}
				else
				{
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					//cout << "6: " << tmp.first << ", " << tmp.second << "\n";

				}
				
				
				if(rxItr->str() == "procedure")
				{
					// put the name into the token
					rxItr++;
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					//cout << "2: " << tmp.first << ", " << tmp.second << "\n";
					// put { into the token
					rxItr++;
					while(rxItr->str() == "\n"){
						rxItr++;
					}
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					//cout << "3: " << tmp.first << ", " << tmp.second << "\n";
					//progline++;
				}

				if(rxItr->str() == "else")
				{
					// put { in token
					rxItr++;
					tmp.first = rxItr->str();
					tmp.second = 0;
					tokens.push_back(tmp);
					//cout << "4: " << tmp.first << ", " << tmp.second << "\n";
				}

			}
			else if(rxItr->str() == ";")
			{
				tmp.first = rxItr->str();
				tmp.second = progline;
				tokens.push_back(tmp);
				//cout << "5: " << tmp.first << ", " << tmp.second << "\n";
				progline++;
			}
		}
		return tokens;
	}
	
	//debug
	void printAll(vector<pair<TOKEN, PROG_LINE>> tmp){
		int i = 0;
		for(i = 0; i < tmp.size(); i++){
			cout << "index: " << i << ", prog_line: " << tmp.at(i).second << ",token " << i << ": " << tmp.at(i).first + "\n";
		}
	}
};
