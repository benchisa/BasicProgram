#include "TestCases.h"

// ==== SampleSimpleSources ==== //

//Purpose: Test for Follows, Modifies, Uses for Assignment
SOURCE SampleSimpleSources::sampleSource1 = "procedure Code1{\n"
									/*1*/	"a = b + 1;\n"
									/*2*/	"c = c + a;\n"
									/*3*/	"d = x + y + z;}";

// Purpose: Test for Follows, Parent, Modifies, Uses for single while loop and procedure
SOURCE SampleSimpleSources::sampleSource2 = "procedure Code2{\n"
									/*1*/	"while q{\n"
									/*2*/	"c = c + a;\n"
									/*3*/	"d = x + y + z;}}";


// Purpose: Test for Follows, Parent, Modifies, Uses for two consecutives while loop
SOURCE SampleSimpleSources::sampleSource3 = "procedure Code3{\n"
									/*1*/	"while a{\n"
									/*2*/	"c = c + a;\n"
									/*3*/	"d = x + y + z;}\n"
									/*4*/	"while b{\n"
									/*5*/	"y = a;\n"
									/*6*/	"b = q+1;}}\n";

// Purpose: Test for Follows, Parent, Modifies, Uses for nested while loop
SOURCE SampleSimpleSources::sampleSource4 = "procedure Code4{\n"
									/*1*/	"while a{\n"
									/*2*/	"c = c + a;\n"
									/*3*/	"d = x + y + z;\n"
									/*4*/	"while b{\n"
									/*5*/	"y = a;\n"
									/*6*/	"b = q+1;}}}\n";

SOURCE SampleSimpleSources::sampleSource5 = "procedure Code5{\n"
									/*1*/	"while a{\n"
									/*2*/	"c = c + a;\n"
									/*3*/	"c = x + y + z;\n"
									/*4*/	"while b{\n"
									/*5*/	"d = a;\n"
									/*6*/	"c = q+1;}\n"
									/*7*/	"e = 2;}}\n";


// Purpose: Test for missing {
SOURCE SampleSimpleSources::sampleInvalidSource1 = "procedure InvalidCode1\n"
											/*1*/  "while a{\n"
											/*2*/  "c = c + a;\n"
											/*3*/  "d = x + y + z;}";


// Purpose: Missing Procedure Name
SOURCE SampleSimpleSources::sampleInvalidSource2 = "procedure {\n"
											/*1*/  "while a{\n"
											/*2*/  "c = c + a;\n"
											/*3*/  "d = x + y + z;}";

// Purpose: Missing while name
SOURCE SampleSimpleSources::sampleInvalidSource3 = "procedure InvalidCode3{\n"
											/*1*/  "while {\n"
											/*2*/  "c = c + a;\n"
											/*3*/  "d = x + y + z;}";


// Purpose: Missing colon
SOURCE SampleSimpleSources::sampleInvalidSource4 = "procedure InvalidCode3{\n"
											/*1*/  "while {\n"
											/*2*/  "c = c + a\n"
											/*3*/  "d = x + y + z;}";


// Purpose: Missing keyword "procedure"
SOURCE SampleSimpleSources::sampleInvalidSource5 = "InvalidCode3{\n"
											/*1*/  "while {\n"
											/*2*/  "c = c + a\n"
											/*3*/  "d = x + y + z;}";


// ==== SamplePQLQueries ====//
// Not sure if you people need this... feel free to add a class for this if needed.

//===============Test for Follows =================== PKB5
QUERY SampleQueries::sampleQuery1 = "stmt s; select s such that Follows(s,4);";//pass
QUERY SampleQueries::sampleQuery2 = "stmt s; select s such that Follows (2,s);";//pass
QUERY SampleQueries::sampleQuery3 = "stmt s1,s2,s select <s1,s2> such that Follows (s1,s2);"; //pass
QUERY SampleQueries::sampleQuery4 = "stmt s1,s2; select s2 such that Follows (s1,s2)"; //pass
QUERY SampleQueries::sampleQuery5 = "stmt s1,s2,s3; select <s1,s2,s3> such that Follows (s1,s2) and Follows(s2,s3)";//pass
QUERY SampleQueries::sampleQuery6 = "stmt s1,s2,s3; select s3 such that Follows (s1,s2) and Follows(s2,s3)";//pass
QUERY SampleQueries::sampleQuery7 = "stmt s;select s with s.stmt#=2;"; //pass
QUERY SampleQueries::sampleQuery8 = "call c; procedure p; variable v;select p with p.procName = \"Code5\";";
QUERY SampleQueries::sampleQuery9 = "while w; callc; select w with w.stmt#=4;"; //pass
QUERY SampleQueries::sampleQuery10 = "variable v; select v with v.varName = \"x\";"; //pass
QUERY SampleQueries::sampleQuery11 = "stmt s; while w; select <s,w> with s.stmt#=w.stmt#;"; //pass
QUERY SampleQueries::sampleQuery12 = "variable v;select v such that Modifies(_, v);";
QUERY SampleQueries::sampleQuery13 = "stmt s; select s such that Uses(s, \"y\");"; 
QUERY SampleQueries::sampleQuery14 = "variable v; select v such that Uses(_, v);"; 
QUERY SampleQueries::sampleQuery15 = "assign a; while w;select a such that Follows* (a,w);"; // <-- this one suppose to return none right..
QUERY SampleQueries::sampleQuery16 = "assign a1,a2;select a2 such that Follows* (a1,a2);";

//===============Test for Parent=================== PKB4
QUERY SampleQueries::sampleQuery17 = "stmt s; select s such that Parent(s,2);";
QUERY SampleQueries::sampleQuery18 = "stmt s; select s such that Parent (4,s);";
QUERY SampleQueries::sampleQuery19 = "stmt s1,s2; select s2 such that Parent (s1,s2);";
QUERY SampleQueries::sampleQuery20 = "stmt s1; select s1 such that Parent (s1,_);";
QUERY SampleQueries::sampleQuery21 = "while w; select w such that Parent (_,_);";
QUERY SampleQueries::sampleQuery22 = "stmt s1; select s1 such that Parent (_,s1);";

//these two cases fail for PKB4, types are defined correctly but SUCHTAHT returns empty result - FIXED
QUERY SampleQueries::sampleQuery23 = "while w1,w2;select w2 such that Parent (w1,w2);";
QUERY SampleQueries::sampleQuery24 = "while w;assign a;select a such that Parent (w,a);";

//===============Test for Parent* =================== PKB4
QUERY SampleQueries::sampleQuery25 = "stmt s; select s such that Parent*(s,5);";
QUERY SampleQueries::sampleQuery26 = "stmt s; select s such that Parent*(1,s);";
QUERY SampleQueries::sampleQuery27 = "stmt s1,s2; select s1 such that Parent* (s1,s2);";
QUERY SampleQueries::sampleQuery28 = "stmt s1; select s1 such that Parent* (s1,_);";
QUERY SampleQueries::sampleQuery29 = "while w; select w such that Parent* (_,_);";
QUERY SampleQueries::sampleQuery30 = "stmt s1; select s1 such that Parent* (_,s1);";

//these two cases fail for PKB4, types are defined correctly but SUCHTAHT returns empty result - FIXED
QUERY SampleQueries::sampleQuery31 = "stmt s;select s such that Follow* (s,s);";
QUERY SampleQueries::sampleQuery32 = "while w;assign a;select a such that Parent* (w,a);";

//===============Test for Modifies=================== PKB4
QUERY SampleQueries::sampleQuery33 = "assign a;select a such that Uses(a,\"e\");";
QUERY SampleQueries::sampleQuery34 = "variable v;stmt s;select v such that Modifies(s, v);";

//below hant tested
QUERY SampleQueries::sampleQuery35 = "variable v;while w;select v such that Modifies(w, v);"; // <-- this didnt call suchthat?
QUERY SampleQueries::sampleQuery36 = "variable v;assign a;select v such that Modifies(a, v);"; // <-- works ok
QUERY SampleQueries::sampleQuery37 = "variable v;select v such that Modifies(_, v);"; // <-- works ok
QUERY SampleQueries::sampleQuery38 = "stmt s;select s such that Modifies(s,\"q\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery39 = "while w;select w such that Modifies(w,\"y\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery40 = "assign a;select a such that Modifies(a,\"a\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery41 = "while w;select w such that Modifies(_,_);";// <-- works ok

//===============Test for Pattern=================== PKB5
QUERY SampleQueries::sampleQuery42 = "assign a;Select a  pattern a(\"d\",_);"; //pass
QUERY SampleQueries::sampleQuery43 = "assign a;Select a  pattern a(_,_);";     //pass 
QUERY SampleQueries::sampleQuery44 = "while w;Select w  pattern w(\"a\",_);";  //no prob with qtree,pass
QUERY SampleQueries::sampleQuery45 = "while w;Select w  pattern w(_,_);"; //no prob with qtree,pass
QUERY SampleQueries::sampleQuery46 = "assign a;Select a  pattern a(_,_\"y\"_);";  //no prob with qtree,pass
QUERY SampleQueries::sampleQuery47 = "assign a;Select a  pattern a(_,\"a\");";  //no prob with qtree
QUERY SampleQueries::sampleQuery48 = "assign a;Select a  pattern a(\"c\",\"2\");"; //no prob with qtree,pass
QUERY SampleQueries::sampleQuery49 = "assign a;variable v;Select v  pattern a(v,_\"a\"_);";  //no prob with qtree

//===============Test for mix of suchthat and pattern=================== PKB5
QUERY SampleQueries::sampleQuery51 = "assign a;select a such that Modifies(a,\"c\") pattern a(\"c\",_);"; //pass
QUERY SampleQueries::sampleQuery52 = "assign a;select a such that Uses(a,\"q\") pattern a(\"d\",_);";    //pass
QUERY SampleQueries::sampleQuery53 = "assign a;variable v;select v such that Uses(a,v) pattern a(v,_\"a\"_);";  //pass
QUERY SampleQueries::sampleQuery54 = "assign a1,a2;select a2 such that Parent(_,a1) pattern a1(\"c\",_);";  //pass

//===============Test for Boolean=================== PKB5	
QUERY SampleQueries::sampleQuery55 = "select boolean such that Parent(2,3);";
//two clauses have relation and need to take intersection
QUERY SampleQueries::sampleQuery56 = "assign a;select boolean such that Modifies(a,\"c\") pattern a(\"c\",_\"y\"_);";
//two clauses have no relation
QUERY SampleQueries::sampleQuery57 = "assign a1,a2;select boolean such that Modifies(a1,\"a\") pattern a2(\"d\",\"a\");"; //first false
QUERY SampleQueries::sampleQuery58 = "assign a1,a2;select boolean such that Modifies(a1,\"c\") pattern a2(\"d\",\"a\");";//both true
QUERY SampleQueries::sampleQuery59 = "assign a1,a2;select boolean such that Modifies(a1,\"c\") pattern a2(\"d\",\"y\");"; //second false
//test other cases
QUERY SampleQueries::sampleQuery60 = "assign a;Select boolean  such that Uses(a,\"vcz\");";
// ==== SamplePKB ====//

PKB* SamplePKB::pkb1(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource1);
	p.startParse();
	return p.getPKB();
}

PKB* SamplePKB::pkb2(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource2);
	p.startParse();
	return p.getPKB();
}

PKB* SamplePKB::pkb3(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource3);
	p.startParse();
	return p.getPKB();
}


PKB* SamplePKB::pkb4(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource4);
	p.startParse();
	return p.getPKB();
}
PKB* SamplePKB::pkb5(){
	Parser p;
	p.setSource(SampleSimpleSources::sampleSource5);
	p.startParse();
	return p.getPKB();
}