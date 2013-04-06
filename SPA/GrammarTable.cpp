#pragma
#include "GrammarTable.h"


GrammarTable::GrammarTable(void){	
	compulsoryOne	= "+";
	optional		= "*";
	or				= "|";
	plus			= "\\+";
	minus			= "\\-";
	times			= "\\*";
	invComma		= "\"";
	letter			= "a-zA-Z";
	digit			= "0-9";
	underscore		= "_";
	hash			= "#";
	ident			= "["+letter+"]"+compulsoryOne+"["+letter+digit+hash+"]"+optional;
	synonym			= ident;
	integer			= "["+digit+"]"+compulsoryOne;
	op				= "["+plus+minus+times+"]";

	stmtRef			= synonym+or+underscore+or+integer;
	entRef			= synonym+or+underscore+or+integer+or+invComma+ident+invComma;
	lineRef			= synonym+or+underscore+or+integer;
	varRef			= synonym+or+underscore+or+invComma+ident+invComma;
	nodeRef			= synonym+or+integer+or+invComma+ident+invComma;
	expr			= "\\(*("+synonym+or+integer+")("+op+"("+synonym+or+integer+")"+"\\)*)"+optional;
	wildexpr		= underscore+invComma+expr+invComma+underscore;
	expr_spec		= invComma+expr+invComma+or+wildexpr;
}

GrammarTable::~GrammarTable(void){

}


void GrammarTable::createEntTable(){	
	eTable[0].entName	="procedure";
	eTable[0].type		=PROCEDURE;

	eTable[1].entName	="stmt";
	eTable[1].type		=STATEMENT;

	eTable[2].entName	="assign";
	eTable[2].type		=ASSIGNMENT;

	eTable[3].entName	="while";
	eTable[3].type		=WHILE;

	eTable[4].entName	="if";
	eTable[4].type		=IF;

	eTable[5].entName	="constant";
	eTable[5].type		=CONSTANT;

	eTable[6].entName	="variable";
	eTable[6].type		=VARIABLE;

	eTable[7].entName	="call";
	eTable[7].type		=CALL;

	eTable[8].entName	="stmtlst";
	eTable[8].type		=STMT_LIST;

	eTable[9].entName	="prog_line";
	eTable[9].type		=PROGLINE;
}

TYPE GrammarTable::getEntType(ENTITY ent){
	//find the entity's formal type number
	for(int i=0;i<10; i++){
		if(ent==eTable[i].entName){
			return eTable[i].type;
		}
	}
}

bool GrammarTable::isEntExists(ENTITY ent){
	for(int i=0;i<10; i++){
		if(ent==eTable[i].entName){			
			return true;
		}
	}
	return false;
}

void GrammarTable::createRelTable(){

	rTable[0].relName	="Calls";
	rTable[0].numArg	=2;
	rTable[0].arg1		=entRef;
	rTable[0].arg2		=entRef;
	rTable[0].type		=CALL;

	rTable[1].relName	="Calls*";
	rTable[1].numArg	=2;
	rTable[1].arg1		=entRef;
	rTable[1].arg2		=entRef;
	rTable[1].type		=CALLST;

	rTable[2].relName	="Follows";
	rTable[2].numArg	=2;
	rTable[2].arg1		=stmtRef;
	rTable[2].arg2		=stmtRef;
	rTable[2].type		=FOLLOWS;

	rTable[3].relName	="Follows*";
	rTable[3].numArg	=2;
	rTable[3].arg1		=stmtRef;
	rTable[3].arg2		=stmtRef;
	rTable[3].type		=FOLLOWST;

	rTable[4].relName	="Parent";
	rTable[4].numArg	=2;
	rTable[4].arg1		=stmtRef;
	rTable[4].arg2		=stmtRef;
	rTable[4].type		=PARENT;

	rTable[5].relName	="Parent*";
	rTable[5].numArg	=2;
	rTable[5].arg1		=stmtRef;
	rTable[5].arg2		=stmtRef;
	rTable[5].type		=PARENTST;

	rTable[6].relName	="Uses_p";
	rTable[6].numArg	=2;
	rTable[6].arg1		=entRef;
	rTable[6].arg2		=varRef;
	rTable[6].type		=USES;

	rTable[7].relName	="Uses_s";
	rTable[7].numArg	=2;
	rTable[7].arg1		=stmtRef;
	rTable[7].arg2		=varRef;
	rTable[7].type		=USES;

	rTable[8].relName	="Modifies_p";
	rTable[8].numArg	=2;
	rTable[8].arg1		=entRef;
	rTable[8].arg2		=varRef;
	rTable[8].type		=MODIFIES;

	rTable[9].relName	="Modifies_s";
	rTable[9].numArg	=2;
	rTable[9].arg1		=stmtRef;
	rTable[9].arg2		=varRef;
	rTable[9].type		=MODIFIES;

	rTable[10].relName	="Next";
	rTable[10].numArg	=2;
	rTable[10].arg1		=lineRef;
	rTable[10].arg2		=lineRef;
	rTable[10].type		=NEXT;

	rTable[11].relName	="Next*";
	rTable[11].numArg	=2;
	rTable[11].arg1		=lineRef;
	rTable[11].arg2		=lineRef;
	rTable[11].type		=NEXTST;

	rTable[12].relName	="Affects";
	rTable[12].numArg	=2;
	rTable[12].arg1		=stmtRef;
	rTable[12].arg2		=stmtRef;
	rTable[12].type		=AFFECTS;

	rTable[13].relName	="Affects*";
	rTable[13].numArg	=2;
	rTable[13].arg1		=stmtRef;
	rTable[13].arg2		=stmtRef;
	rTable[13].type		=AFFECTST;

	rTable[14].relName	="Contains";
	rTable[14].numArg	=2;
	rTable[14].arg1		=nodeRef;
	rTable[14].arg2		=nodeRef;
	rTable[14].type		=CONTAINS;

	rTable[15].relName	="Contains*";
	rTable[15].numArg	=2;
	rTable[15].arg1		=nodeRef;
	rTable[15].arg2		=nodeRef;
	rTable[15].type		=CONTAINST;

	rTable[16].relName	="Sibling";
	rTable[16].numArg	=2;
	rTable[16].arg1		=nodeRef;
	rTable[16].arg2		=nodeRef;
	rTable[16].type		=SIBLING;

}

int GrammarTable::getRelArgCount(RELATIONSHIP rel){
	for(int i=0;i<17; i++){
		if(rel==rTable[i].relName){
			return rTable[i].numArg;
		}
	}
}

string GrammarTable::getRelArg(RELATIONSHIP rel,int argPosition){
	for(int i=0;i<17; i++){
		if(rel==rTable[i].relName){
			if(argPosition==1){
				return rTable[i].arg1;
			} else{
				return rTable[i].arg2;
			}
		}
	}
}

TYPE GrammarTable::getRelType(RELATIONSHIP rel){
	for(int i=0;i<17; i++){
		if(rel==rTable[i].relName){
			return rTable[i].type;
		}
	}
}

bool GrammarTable::isRelExists(RELATIONSHIP rel){
	for(int i=0;i<17; i++){
		if(rel==rTable[i].relName){
			return true;
		}
	}	
	return false;
}

void GrammarTable::createPattTable(){
	pTable[0].pattName	="assignment";
	pTable[0].numArg	=2;
	pTable[0].arg1		=varRef;
	pTable[0].arg2		=expr_spec+or+underscore;
	pTable[0].type		=ASSIGNMENT;

	pTable[1].pattName	="while";
	pTable[1].numArg	=2;
	pTable[1].arg1		=varRef;
	pTable[1].arg2		=underscore;
	pTable[1].type		=WHILE;

	pTable[2].pattName	="if";
	pTable[2].numArg	=3;
	pTable[2].arg1		=varRef;
	pTable[2].arg2		=underscore;
	pTable[2].arg3		=underscore;
	pTable[2].type		=IF;
}

int GrammarTable::getPattArgCount(TYPE pattType){
	for(int i=0;i<3; i++){
		if(pattType==pTable[i].type){
			return pTable[i].numArg;
		}
	}
}

string GrammarTable::getPattArg(TYPE pattType,int argPosition){
	for(int i=0;i<3; i++){
		if(pattType==pTable[i].type){
			if(argPosition==1){
				return pTable[i].arg1;
			} else if (argPosition==2){
				return pTable[i].arg2;
			} else{
				return pTable[i].arg3;
			}
		}
	}
}

TYPE GrammarTable::getPattType(PATTERN patt){
	for(int i=0;i<3; i++){
		if(patt==pTable[i].pattName){
			return pTable[i].type;
		}
	}
}

bool GrammarTable::isPattExists(PATTERN patt){
	for(int i=0;i<3; i++){
		if(patt==pTable[i].pattName){
			return true;
		}
	}	
	return false;
}

void GrammarTable::createArgTable(){
	vector<TYPE> case1,case2,case3,case4,case5,case6,case7,case8;

	case1.push_back(STATEMENT);
	case1.push_back(WHILE);
	case1.push_back(IF);
	case1.push_back(ANY);

	case2.push_back(STATEMENT);
	case2.push_back(ASSIGNMENT);
	case2.push_back(CALL);
	case2.push_back(WHILE);
	case2.push_back(IF);
	case2.push_back(ANY);

	case3.push_back(PROCEDURE);
	case3.push_back(STATEMENT);
	case3.push_back(ASSIGNMENT);
	case3.push_back(WHILE);
	case3.push_back(IF);
	case3.push_back(CALL);
	case3.push_back(ANY);

	case4.push_back(VARIABLE);
	case4.push_back(ANY);

	case5.push_back(PROGLINE);
	case5.push_back(STATEMENT);
	case5.push_back(ASSIGNMENT);
	case5.push_back(CALL);
	case5.push_back(WHILE);
	case5.push_back(IF);
	case5.push_back(ANY);

	case6.push_back(ASSIGNMENT);
	case6.push_back(ANY);
	
	case7.push_back(PROCEDURE);
	case7.push_back(ANY);

	case8.push_back(PROCEDURE);
	case8.push_back(STMT_LIST);
	case8.push_back(STATEMENT);
	case8.push_back(ASSIGNMENT);
	case8.push_back(CALL);
	case8.push_back(WHILE);
	case8.push_back(IF);
	case8.push_back(PLUS);
	case8.push_back(MINUS);
	case8.push_back(MULTIPLY);
	case8.push_back(VARIABLE);
	case8.push_back(CONSTANT);

	aTable[0].type = FOLLOWS;
	aTable[0].arg1 = case2;
	aTable[0].arg2 = case2;

	aTable[1].type = FOLLOWST;
	aTable[1].arg1 = case2;
	aTable[1].arg2 = case2;

	aTable[2].type = PARENT;
	aTable[2].arg1 = case1;
	aTable[2].arg2 = case2;

	aTable[3].type = PARENTST;
	aTable[3].arg1 = case1;
	aTable[3].arg2 = case2;

	aTable[4].type = MODIFIES;
	aTable[4].arg1 = case3;
	aTable[4].arg2 = case4;

	aTable[5].type = USES;
	aTable[5].arg1 = case3;
	aTable[5].arg2 = case4;

	aTable[6].type = NEXT;
	aTable[6].arg1 = case5;
	aTable[6].arg2 = case5;

	aTable[7].type = NEXTST;
	aTable[7].arg1 = case5;
	aTable[7].arg2 = case5;

	aTable[8].type = AFFECTS;
	aTable[8].arg1 = case6;
	aTable[8].arg2 = case6;

	aTable[9].type = AFFECTST;
	aTable[9].arg1 = case6;
	aTable[9].arg2 = case6;

	aTable[10].type = CALL;
	aTable[10].arg1 = case7;
	aTable[10].arg2 = case7;

	aTable[11].type = CALLST;
	aTable[11].arg1 = case7;
	aTable[11].arg2 = case7;

	aTable[12].type = CONTAINS;
	aTable[12].arg1 = case8;
	aTable[12].arg2 = case8;

	aTable[13].type = CONTAINST;
	aTable[13].arg1 = case8;
	aTable[13].arg2 = case8;

	aTable[14].type = SIBLING;
	aTable[14].arg1 = case8;
	aTable[14].arg2 = case8;
}

vector<TYPE> GrammarTable::getArgument(TYPE rel, int argPosition){
	for(int i=0;i<15; i++){
		if(rel==aTable[i].type){
			if(argPosition==1){
				return aTable[i].arg1;
			} else{
				return aTable[i].arg2;
			}
		}
	}
}

void GrammarTable::createAttrTable(){
	atTable[0].type=CONSTANT;
	atTable[0].attr="value";

	atTable[1].type=STATEMENT;
	atTable[1].attr="stmt#";

	atTable[2].type=ASSIGNMENT;
	atTable[2].attr="stmt#";

	atTable[3].type=WHILE;
	atTable[3].attr="stmt#";

	atTable[4].type=IF;
	atTable[4].attr="stmt#";
	
	atTable[5].type=CALL;
	atTable[5].attr="stmt#";
	
	atTable[6].type=CALL;
	atTable[6].attr="procName";

	atTable[7].type=PROCEDURE;
	atTable[7].attr="procName";

	atTable[8].type=VARIABLE;
	atTable[8].attr="varName";
}

vector<GrammarTable::ATTRIBUTE> GrammarTable::getAttr(TYPE ent){
	vector<ATTRIBUTE> res;
	for(int i=0;i<9; i++){
		if(ent==atTable[i].type){
			res.push_back(atTable[i].attr);
		}
	}
	return res;
}

void GrammarTable::createContainsValNodeTable(){
	vector<TYPE> temp;

	containsValTable[0].left=ASSIGNMENT;
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containsValTable[0].right=temp;
	temp.clear();

	containsValTable[1].left=IF;
	temp.push_back(VARIABLE);
	temp.push_back(STMT_LIST);
	containsValTable[1].right=temp;
	temp.clear();

	containsValTable[2].left=WHILE;
	temp.push_back(VARIABLE);
	temp.push_back(STMT_LIST);
	containsValTable[2].right=temp;
	temp.clear();

	containsValTable[3].left=PROCEDURE;
	temp.push_back(STMT_LIST);
	containsValTable[3].right=temp;
	temp.clear();

	containsValTable[4].left=STMT_LIST;
	temp.push_back(STATEMENT);
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(CALL);
	containsValTable[4].right=temp;
	temp.clear();
	
	containsValTable[5].left=STATEMENT;
	temp.push_back(VARIABLE);
	temp.push_back(STMT_LIST);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containsValTable[5].right=temp;
	temp.clear();

	containsValTable[6].left=CALL;
	//should not have anything under CALL
	containsValTable[6].right=temp;
	temp.clear();

	containsValTable[7].left=CONSTANT;
	//should not have anything under CONSTANT
	containsValTable[7].right=temp;
	temp.clear();

	containsValTable[8].left=VARIABLE;
	//should not have anything under VARIABLE
	containsValTable[8].right=temp;
	temp.clear();

	containsValTable[9].left=PLUS;
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containsValTable[9].right=temp;
	temp.clear();

	containsValTable[10].left=MINUS;
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containsValTable[10].right=temp;
	temp.clear();

	containsValTable[11].left=MULTIPLY;
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containsValTable[11].right=temp;
	temp.clear();

}

vector<TYPE> GrammarTable::getContainsValNodes(TYPE left){
	for(int i=0;i<12; i++){
		if(left==containsValTable[i].left){
			return containsValTable[i].right;
		}
	}	
}

void GrammarTable::createContainSTInvNodeTable(){
	vector<TYPE> temp;

	containSTInvTable[0].left=ASSIGNMENT;
	temp.push_back(PROCEDURE);	
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(CALL);
	containSTInvTable[0].right=temp;
	temp.clear();

	containSTInvTable[1].left=IF;
	temp.push_back(PROCEDURE);
	containSTInvTable[1].right=temp;
	temp.clear();

	containSTInvTable[2].left=WHILE;
	temp.push_back(PROCEDURE);
	containSTInvTable[2].right=temp;
	temp.clear();

	containSTInvTable[3].left=PROCEDURE;
	temp.push_back(PROCEDURE);
	containSTInvTable[3].right=temp;
	temp.clear();

	containSTInvTable[4].left=STMT_LIST;
	temp.push_back(PROCEDURE);
	containSTInvTable[4].right=temp;
	temp.clear();
	
	containSTInvTable[5].left=STATEMENT;
	temp.push_back(PROCEDURE);
	containSTInvTable[5].right=temp;
	temp.clear();

	containSTInvTable[6].left=CALL;
	//should not have anything under CALL
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(CALL);
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containSTInvTable[6].right=temp;
	temp.clear();

	containSTInvTable[7].left=CONSTANT;
	//should not have anything under CONSTANT
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(CALL);
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containSTInvTable[7].right=temp;
	temp.clear();

	containSTInvTable[8].left=VARIABLE;
	//should not have anything under VARIABLE
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(CALL);
	temp.push_back(CONSTANT);
	temp.push_back(VARIABLE);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(MULTIPLY);
	containSTInvTable[8].right=temp;
	temp.clear();

	containSTInvTable[9].left=PLUS;	
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(ASSIGNMENT);
	temp.push_back(CALL);
	containSTInvTable[9].right=temp;
	temp.clear();

	containSTInvTable[10].left=MINUS;
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(ASSIGNMENT);
	temp.push_back(CALL);
	containSTInvTable[10].right=temp;
	temp.clear();

	containSTInvTable[11].left=MULTIPLY;
	temp.push_back(PROCEDURE);
	temp.push_back(STMT_LIST);
	temp.push_back(STATEMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(ASSIGNMENT);
	temp.push_back(CALL);
	containSTInvTable[11].right=temp;
	temp.clear();

}

vector<TYPE> GrammarTable::getContainSTInvNodes(TYPE left){
	for(int i=0;i<12; i++){
		if(left==containSTInvTable[i].left){
			return containSTInvTable[i].right;
		}
	}	
}

void GrammarTable::createSiblingValNodeTable(){
	vector<TYPE> temp;

	siblingValTable[0].left=ASSIGNMENT;	
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(STATEMENT);
	temp.push_back(CALL);
	siblingValTable[0].right=temp;
	temp.clear();

	siblingValTable[1].left=IF;
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(STATEMENT);
	temp.push_back(CALL);
	siblingValTable[1].right=temp;
	temp.clear();

	siblingValTable[2].left=WHILE;
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(STATEMENT);
	temp.push_back(CALL);
	siblingValTable[2].right=temp;
	temp.clear();

	siblingValTable[3].left=PROCEDURE;
	temp.push_back(PROCEDURE);
	siblingValTable[3].right=temp;
	temp.clear();

	siblingValTable[4].left=STMT_LIST;
	temp.push_back(STMT_LIST);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[4].right=temp;
	temp.clear();
	
	siblingValTable[5].left=STATEMENT;
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(STATEMENT);
	temp.push_back(CALL);
	siblingValTable[5].right=temp;
	temp.clear();

	siblingValTable[6].left=CALL;
	temp.push_back(ASSIGNMENT);
	temp.push_back(WHILE);
	temp.push_back(IF);
	temp.push_back(STATEMENT);
	temp.push_back(CALL);
	siblingValTable[6].right=temp;
	temp.clear();

	siblingValTable[7].left=CONSTANT;
	temp.push_back(MULTIPLY);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[7].right=temp;
	temp.clear();

	siblingValTable[8].left=VARIABLE;
	temp.push_back(STMT_LIST);
	temp.push_back(MULTIPLY);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[8].right=temp;
	temp.clear();

	siblingValTable[9].left=PLUS;	
	temp.push_back(MULTIPLY);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[9].right=temp;
	temp.clear();

	siblingValTable[10].left=MINUS;
	temp.push_back(MULTIPLY);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[10].right=temp;
	temp.clear();

	siblingValTable[11].left=MULTIPLY;
	temp.push_back(MULTIPLY);
	temp.push_back(PLUS);
	temp.push_back(MINUS);
	temp.push_back(VARIABLE);
	temp.push_back(CONSTANT);
	siblingValTable[11].right=temp;
	temp.clear();

}

vector<TYPE> GrammarTable::getSiblingValNodes(TYPE left){
	for(int i=0;i<12; i++){
		if(left==siblingValTable[i].left){
			return siblingValTable[i].right;
		}
	}	
}

void GrammarTable::printAllEnt(){
	cout<<"====================All Entities======================"<<endl;
	for(int i=0;i<9; i++){
		cout<<"ENTITY NAME: " << eTable[i].entName<<endl;
		cout<<"ENTITY TYPE: " << eTable[i].type<<endl;
	}
}

void GrammarTable::printAllRel(){	
	cout<<"====================All Relationships======================"<<endl;
	for(int i=0;i<14; i++){
		cout<<"REL NAME: " << rTable[i].relName<<endl;
		cout<<"REL TYPE: " << rTable[i].type<<endl;
		cout<<"NUM ARG: " << rTable[i].numArg<<endl;
		cout<<"ARG1: " << rTable[i].arg1<<endl;
		cout<<"ARG2: " << rTable[i].arg2<<endl;
	}
}

void GrammarTable::printAllPatt(){	
	cout<<"====================All Patterns======================"<<endl;
	for(int i=0;i<3; i++){
		cout<<"PATT NAME: " << pTable[i].pattName<<endl;
		cout<<"PATT TYPE: " << pTable[i].type<<endl;
		cout<<"NUM ARG: " << pTable[i].numArg<<endl;
		cout<<"ARG1: " << pTable[i].arg1<<endl;
		cout<<"ARG2: " << pTable[i].arg2<<endl;
		cout<<"ARG3: " << pTable[i].arg3<<endl;		
	}
}

void GrammarTable::printAllAttr(){
	cout<<"====================All ATTRIBUTES======================"<<endl;
	for(int i=0;i<9; i++){
		cout<<"ENTITY TYPE: " << atTable[i].type<<endl;
		cout<<"ATTRIBUTE: " << atTable[i].attr<<endl;
	}
}

void GrammarTable::printAllArg(){
	cout<<"====================All Arguments======================"<<endl;
	for(int i=0;i<10; i++){
		cout<<"TYPE: " << aTable[i].type<<endl;
		for(int j=0;j<aTable[i].arg1.size();j++)
			cout<<"ARG1: " << aTable[i].arg1.at(j)<<endl;
		for(int j=0;j<aTable[i].arg2.size();j++)
			cout<<"ARG2: " << aTable[i].arg2.at(j)<<endl;
	}
}

void GrammarTable::printAllContainsValNodes(){
	cout<<"====================All VALID CONTAINS======================"<<endl;
	for(int i=0;i<12; i++){
		for (int j=0;j<containsValTable[i].right.size();j++){
			cout<<"Contains( " << containsValTable[i].left << " , " << containsValTable[i].right[j] <<" )"<<endl;
		}
	}
}

void GrammarTable::printAllContainSTInvNodes(){
	cout<<"====================All VALID CONTAINS======================"<<endl;
	for(int i=0;i<12; i++){
		for (int j=0;j<containSTInvTable[i].right.size();j++){
			cout<<"Contains( " << containSTInvTable[i].left << " , " << containSTInvTable[i].right[j] <<" )"<<endl;
		}
	}
}

void GrammarTable::printAllSiblingValNodes(){
	cout<<"====================All VALID CONTAINS======================"<<endl;
	for(int i=0;i<12; i++){
		for (int j=0;j<siblingValTable[i].right.size();j++){
			cout<<"Contains( " << siblingValTable[i].left << " , " << siblingValTable[i].right[j] <<" )"<<endl;
		}
	}
}