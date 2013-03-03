#pragma
#include "Global_Data_Abstraction.h";


class GrammarTable{

public:

	typedef std::string ENTITY;
	typedef std::string RELATIONSHIP;
	typedef std::string PATTERN;
	typedef std::string ATTRIBUTE;

	struct entTable{
		ENTITY entName;
		TYPE type;
	};
	
	struct syntaxTable{
		RELATIONSHIP relName;
		PATTERN pattName;
		int numArg;
		TYPE type;
		string arg1;
		string arg2;
		string arg3;
	};

	struct argTable{
		TYPE type;
		vector<TYPE> arg1;
		vector<TYPE> arg2;
	};

	struct attrTable{
		TYPE type;
		ATTRIBUTE attr;
	};

	struct entTable eTable[10];
	struct syntaxTable rTable[14];
	struct syntaxTable pTable[3];
	struct argTable aTable[12];
	struct attrTable atTable[9];

	string compulsoryOne,optional,or,plus,minus,times,invComma,letter,digit,underscore,hash,ident,synonym,integer,op;
	string stmtRef,entRef,lineRef,varRef,expr,wildexpr,expr_spec;

	GrammarTable(void);
	~GrammarTable(void);

	void createEntTable();
	TYPE getEntType(ENTITY);
	bool isEntExists(ENTITY);
	void printAllEnt();

	void createRelTable();
	int getRelArgCount(RELATIONSHIP);
	string getRelArg(RELATIONSHIP, int);
	TYPE getRelType(RELATIONSHIP);
	bool isRelExists(RELATIONSHIP);
	void printAllRel();

	void createPattTable();
	int getPattArgCount(TYPE);
	string getPattArg(TYPE, int);
	TYPE getPattType(PATTERN);
	bool isPattExists(PATTERN);
	void printAllPatt();

	void createArgTable();
	vector<TYPE> getArgument(TYPE,int);
	void printAllArg();

	void createAttrTable();
	vector<ATTRIBUTE> getAttr(TYPE);
	void printAllAttr();
};