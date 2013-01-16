#pragma
#include "Global_Data_Abstraction.h"

class GrammarTable{

public:

	typedef std::string ENTITY;
	typedef pair<string,TYPE> entPair;

	typedef std::string RELATIONSHIP;
	typedef pair<TYPE,string> relSubPair;
	typedef pair<string,pair<TYPE,string>> relPair; 

	typedef std::string PATTERN;
	typedef int PATT_INDEX;
	typedef pair<TYPE,string> pattPair;

	typedef pair<vector<TYPE>,vector<TYPE>> argSubPair;
	typedef pair<TYPE,argSubPair> argPair;

	GrammarTable(void);
	~GrammarTable(void);

	void createEntTable();
	TYPE getEntType(ENTITY ent);
	bool isEntExists(ENTITY ent);
	void printAllEnt();

	void createRelTable();
	string getRelGrammar(RELATIONSHIP rel);
	TYPE getRelType(RELATIONSHIP rel);
	bool isRelExists(RELATIONSHIP rel);
	void printAllRel();

	void createPattTable();
	string getPattGrammar(TYPE patt);
	bool isPattExists(TYPE patt);
	void printAllPatt();

	void createArgTable();
	vector<TYPE> getArgument(TYPE rel,int arg);
	void printAllArg();
};