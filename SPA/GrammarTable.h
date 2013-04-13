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

	struct nodeTable{
		TYPE left;
		vector<TYPE> right;
	};

	struct entTable eTable[13];
	struct syntaxTable rTable[17];
	struct syntaxTable pTable[3];
	struct argTable aTable[15];
	struct attrTable atTable[9];
	struct nodeTable containsValTable[12];//NEW extension
	struct nodeTable containSTInvTable[12];//NEW extension
	struct nodeTable siblingValTable[12];//NEW extension
	
	string compulsoryOne,optional,or,plus,minus,times,invComma,letter,digit,underscore,hash,ident,synonym,integer,op;
	string stmtRef,entRef,lineRef,nodeRef,varRef,expr,wildexpr,expr_spec;

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
	
	//check valid
	void createContainsValNodeTable();
	vector<TYPE> getContainsValNodes(TYPE);
	void printAllContainsValNodes();
	
	//check invalid
	void createContainSTInvNodeTable();
	vector<TYPE> getContainSTInvNodes(TYPE);
	void printAllContainSTInvNodes();

	//check valid
	void createSiblingValNodeTable();
	vector<TYPE> getSiblingValNodes(TYPE);
	void printAllSiblingValNodes();
};