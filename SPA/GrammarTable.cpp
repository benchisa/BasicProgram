#pragma
#include "GrammarTable.h"
#include <hash_map>

hash_map<string,TYPE> (*entTable);
hash_map<string,pair<TYPE,string>> (*relTable);
hash_map<TYPE,string> (*pattTable);
hash_map<TYPE,pair<vector<TYPE>,vector<TYPE>>> (*argTable);
hash_map<string,TYPE>::const_iterator entIter;
hash_map<string,pair<TYPE,string>>::const_iterator relIter;
hash_map<TYPE,string>::const_iterator pattIter;
hash_map<TYPE,pair<vector<TYPE>,vector<TYPE>>>::const_iterator argIter;

GrammarTable::GrammarTable(void){
	entTable = new hash_map<string,TYPE>;
	relTable = new hash_map<string,pair<TYPE,string>>;
	pattTable = new hash_map<TYPE,string>;
	argTable = new hash_map<TYPE,pair<vector<TYPE>,vector<TYPE>>>;
}

GrammarTable::~GrammarTable(void){
	delete entTable;
	delete relTable;
	delete pattTable; 
	delete argTable;
}


void GrammarTable::createEntTable(){

	//required
	(*entTable).insert(entPair("procedure",PROCEDURE));
	(*entTable).insert(entPair("constant",CONSTANT));
	(*entTable).insert(entPair("variable",VARIABLE));
	(*entTable).insert(entPair("assign",ASSIGNMENT));
	(*entTable).insert(entPair("while",WHILE));
	(*entTable).insert(entPair("stmt",STATEMENT));	

	//bonus
	(*entTable).insert(entPair("if",IF));
	//(*entTable).insert(entPair("call",CALL));

}


TYPE GrammarTable::getEntType(ENTITY ent){
	//find the entity's formal type name
	entIter = (*entTable).find(ent);

	return entIter->second;

}

bool GrammarTable::isEntExists(ENTITY ent){
	entIter = (*entTable).find(ent); 

	if(entIter==(*entTable).end()) 
		return false;

	return true;
}

void GrammarTable::createRelTable(){

	string ident = "[a-zA-Z]+[a-zA-Z0-9#]*";
	string synonym = ident;
	string integer = "[0-9]+";
	string underscore = "_";

	(*relTable).insert(relPair("follows", relSubPair(FOLLOWS,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|"+integer)));
	(*relTable).insert(relPair("follows*", relSubPair(FOLLOWST,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|"+integer)));
	(*relTable).insert(relPair("parent", relSubPair(PARENT,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|"+integer)));
	(*relTable).insert(relPair("parent*", relSubPair(PARENTST,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|"+integer)));
	//(*relTable).insert(relPair("modifiesP",relSubPair(MODIFIES,synonym+"|"+underscore+"|\\\""+ident+"\\\"|"+integer+","+synonym+"|"+underscore+"|\\\""+ident+"\\\"")));
	(*relTable).insert(relPair("modifies",relSubPair(MODIFIES,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|\\\""+ident+"\\\"")));
	//(*relTable).insert(relPair("usesP",relSubPair(USES,synonym+"|"+underscore+"|\\\""+ident+"\\\"|"+integer+","+synonym+"|"+underscore+"|\\\""+ident+"\\\"")));
	(*relTable).insert(relPair("uses",relSubPair(USES,synonym+"|"+underscore+"|"+integer+","+synonym+"|"+underscore+"|\\\""+ident+"\\\"")));


}


string GrammarTable::getRelGrammar(RELATIONSHIP rel){
	string info = "";
	relIter = (*relTable).find(rel); 

	if(relIter!=(*relTable).end())
		info = (relIter->second).second;
	return info;
}

TYPE GrammarTable::getRelType(RELATIONSHIP rel){
	TYPE relType;
	relIter = (*relTable).find(rel); 

	if(relIter!=(*relTable).end())
		relType = (relIter->second).first;
	return relType;
}

bool GrammarTable::isRelExists(RELATIONSHIP rel){
	relIter = (*relTable).find(rel); 

	if(relIter==(*relTable).end()) 
		return false;

	return true;
}


void GrammarTable::createPattTable(){
	string ident = "[a-zA-Z]+[a-zA-Z0-9#]*";
	string synonym = ident;
	string constant = "[0-9]+";
	string underscore = "_";

	(*pattTable).insert(pattPair(ASSIGNMENT,synonym+"|"+underscore+"|\""+ident+"\",\"" + synonym + "(\\+" + synonym + ")*\"|\"" + synonym + "(\\+" + constant + ")*\"|\""+constant+"(\\+" + synonym + ")*\"|_\"" + synonym + "(\\+" + synonym + ")*\"_|_\"" + constant + "(\\+" + synonym + ")*\"_|_\"" + synonym + "(\\+" + constant + ")*\"_|_"));
	(*pattTable).insert(pattPair(WHILE,synonym+"|"+underscore+"|\""+ident+"\",_" ));

	//future
	//(*pattTable).insert(pattPair("pattern_if", ));
}

string GrammarTable::getPattGrammar(TYPE patt){
	string info = "";
	pattIter = (*pattTable).find(patt); 

	if(pattIter!=(*pattTable).end())
		info = pattIter->second;	

	return info;
}

bool GrammarTable::isPattExists(TYPE patt){
	pattIter = (*pattTable).find(patt); 

	if(pattIter==(*pattTable).end()) 
		return false;

	return true;
}

void GrammarTable::createArgTable(){
	vector<TYPE> case1,case2,case3;

	case1.push_back(WHILE);
	case1.push_back(IF);
	case1.push_back(STATEMENT);

	case2.push_back(STATEMENT);
	case2.push_back(ASSIGNMENT);
	case2.push_back(WHILE);
	case2.push_back(IF);

	case3.push_back(VARIABLE);

	(*argTable).insert(argPair(FOLLOWS,argSubPair(case2,case2)));
	(*argTable).insert(argPair(FOLLOWST,argSubPair(case2,case2)));
	(*argTable).insert(argPair(PARENT,argSubPair(case1,case2)));
	(*argTable).insert(argPair(PARENTST,argSubPair(case1,case2)));
	(*argTable).insert(argPair(MODIFIES,argSubPair(case2,case3)));
	(*argTable).insert(argPair(USES,argSubPair(case2,case3)));

}

vector<TYPE> GrammarTable::getArgument(TYPE rel, int arg){

	vector<TYPE> res;
	argIter = (*argTable).find(rel); 

	if(argIter!=(*argTable).end())
		if (arg == 0)
			res = (argIter->second).first;	
		else
			res = (argIter->second).second;

	return res;
}

void GrammarTable::printAllEnt(){
		for(entIter=(*entTable).begin( );entIter!= (*entTable).end( );entIter++){
			cout << "entity : " << entIter->first << "|" << entIter->second << endl;
		}
}

void GrammarTable::printAllRel(){
		for(relIter=(*relTable).begin( );relIter!= (*relTable).end( );relIter++){
			cout << "relationship : " << relIter->first << "|" << (relIter->second).first << "|" << (relIter->second).second << endl;
		}
}

void GrammarTable::printAllPatt(){
		for(pattIter=(*pattTable).begin( );pattIter!= (*pattTable).end( );pattIter++){
			cout << "pattern : " << pattIter->first << "|" << pattIter->second << endl;
		}
}

void GrammarTable::printAllArg(){
		for(argIter=(*argTable).begin( );argIter!= (*argTable).end( );argIter++){
			cout << "arguments : " << argIter->first << endl;
			for(int j = 0; j < (argIter->second).first.size(); j++){
				cout<<	(argIter->second).first.at(j);
			}
			for(int j = 0; j < (argIter->second).second.size(); j++){
				cout<<	(argIter->second).second.at(j);
			}
		}
}