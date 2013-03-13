#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>
#include <string>
#include <list>
#include <map>
#include <hash_map>
#include <unordered_map>
#include <set>

using namespace std;

class Procedure;
class AST;

enum TYPE{	
	PROCEDURE,
	WHILE,
	IF,
	CALL,
	ASSIGNMENT,	
	STATEMENT,
	STMT_LIST,
	PROGLINE,
	VARIABLE,
	CONSTANT,
	ANY,
	THEN,
	ELSE,
	INTEGER,	
	PLUS,
	MULTIPLY,
	MINUS,
	BRACKET,
	BOOL,
	STRING,
	SELECT,
	WITH,
	SUCHTHAT,
	PATTERN,
	RESULT,
	QUERYVAR,
	PARAM,
	NAME,
	FOLLOWS,
	FOLLOWST,
	PARENT,
	PARENTST,
	MODIFIES,
	USES,
	CALLST,
	NEXT,
	NEXTST,
	AFFECTS,
	AFFECTST
};
enum CONTENT{
	EMPTY,
	ALL,
};
typedef TYPE ASTNODE_TYPE,QTREENODE_TYPE, MODIFIES_TYPE,USES_TYPE;

typedef list<pair<int, int>> MODIFIES_LIST;
typedef list<pair<int, int>> USES_LIST;
typedef list<pair<int, int>> FOLLOWS_LIST;
typedef list<pair<int, int>> PARENT_LIST;
typedef list<pair<int, int>> RELATION_LIST;
typedef list<pair<int, int>> NEXT_LIST;
typedef list<pair<int, int>> AFFECT_LIST;
typedef list<int> STATEMENT_LIST;
typedef list<int> INDEX_LIST;
typedef std::list<int> CONSTANT_LIST;
typedef list<string> FINAL_RESULT;
typedef std::list<string> VAR_LIST;
typedef std::list<Procedure> PROC_LIST;
typedef std::list<AST*> AST_LIST;
typedef hash_map<int,TYPE> QUERYTABLE;
typedef hash_map<int,string> QUERYPARAM;
typedef list<pair<string,string>> CALL_LIST;
typedef pair<string,string> CALL_PAIR;
typedef list<CALL_PAIR> CALLPAIR_LIST;
typedef pair<TYPE,int> RELATION_PAIR;
typedef vector<int> ROW;
typedef vector<ROW> ROW_LIST;
typedef vector<int> DATA_LIST;
typedef vector<DATA_LIST> RAWDATA;
typedef hash_map<string,DATA_LIST> FINALRAW;


typedef int STATEMENT_NUM,VAR_INDEX,SIZE, PROC_INDEX,PROG_LINE,USES_INDEX,MODIFIES_INDEX,INDEX,CONSTANT_INDEX,CALL_INDEX,JOIN_ATTR;
typedef string EXPRESSION,PREFIXEXPR, VAR_NAME,PROC_NAME, OPERAND, OPERATOR;
typedef string REPORT_MSG, QUERY, SOURCE, ERROR_MSG, TOKEN;

//for new prog line
struct ProgLine
{
	int progLineNum;
	int statementNum;
};

struct ASSIGNENTRY{
	int varIndex;
	string prefixTree;
};

