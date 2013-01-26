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

using namespace std;

class Procedure;
class AST;

enum TYPE{
	STMT_LIST,
	ASSIGNMENT,
	IF,
	INTEGER,
	WHILE,
	PROCEDURE,
	VARIABLE,
	CONSTANT,
	PLUS,
	MULTIPLY,
	MINUS,
	BRACKET,
	STATEMENT,
	BOOL,
	STRING,
	SUCHTHAT,
	PATTERN,
	SELECT,
	RESULT,
	QUERYVAR,
	FOLLOWS,
	FOLLOWST,
	PARENT,
	PARENTST,
	MODIFIES,
	USES,
	ANY,
	PARAM,
	CALL,
	PROGLINE
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
typedef list<int> STATEMENT_LIST;
typedef list<int> INDEX_LIST;
typedef std::list<int> CONSTANT_LIST;
typedef list<string> FINAL_RESULT;
typedef std::list<string> VAR_LIST;
typedef std::list<Procedure> PROC_LIST;
typedef std::list<AST*> AST_LIST;
typedef hash_map<int,TYPE> QUERYTABLE;
typedef hash_map<int,string> QUERYPARAM;
typedef pair<TYPE,list<int> *> RAWDATA;
typedef list<pair<string,string>> CALL_LIST;
typedef pair<string,string> CALL_PAIR;
typedef list<CALL_PAIR> CALLPAIR_LIST;


typedef int STATEMENT_NUM,VAR_INDEX,SIZE, PROC_INDEX,PROG_LINE,USES_INDEX,MODIFIES_INDEX,INDEX,CONSTANT_INDEX,CALL_INDEX;
typedef string EXPRESSION,VAR_NAME,PROC_NAME;
typedef string REPORT_MSG, QUERY, SOURCE, ERROR_MSG, TOKEN;

//for new prog line
struct ProgLine
{
	int progLineNum;
	int statementNum;
};

