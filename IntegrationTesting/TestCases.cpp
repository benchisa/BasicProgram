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

SOURCE SampleSimpleSources::sampleSource5 ="procedure xylo{\n"		
											 "apple=1;\n"						//1
											 "banana=apple+1;\n"				//2
											 "carrot=apple;\n"					//3
											 "banana=carrot+1;\n"				//4
											 "donut=0;\n"						//5
											 "egg=1;\n"							//6
											 "fish=egg;\n"						//7
											 "call yello;\n"					//8
											 "while apple{\n"					//9
											    "while egg{\n"					//10
											       "innard = 0;\n"				//11
                                                   "while donut{\n"				//12
														"gummy=egg+1;\n"		//13
													"}\n"
												   "while fish{\n"				//14
														"gummy=0;\n"			//15
														"while innard{\n"		//16
															"fish=innard;\n"	//17
													    "}\n"			
												   "}\n"
												   "ham=gummy;\n"				//18
												"}\n"
												"gummy=apple+9;\n"				//19
											  "}\n"	
											 "apple=11;\n"						//20
											 "if innard then{\n"				//21
													"jam=apple;}\n"				//22
											 "else{\n"			
													"jam=gummy;\n"				//23
													"if egg then{\n"			//24
														"gummy=33;}\n"			//25
													"else{\n"
														"gummy=99;}\n"			//26
													"}\n"
											 "}\n"
									 "procedure yello{\n"	
										"kimchi = 88;\n"						//27
										"leek=0;\n"								//28
										"while leek{\n"							//29
											"kimchi=kimchi+1;\n"				//30
										"}\n"
										"call zebra;\n"							//31
										"apple=kimchi;\n"						//32
										"jam=egg+2;\n"							//33
									"}\n"
									"procedure zebra{\n"
										"meat=1;\n"								//34
										"banana=meat+10;\n"						//35
										"if meat then{\n"						//36
												"carrot=banana+22;}\n"			//37
										"else{\n"
												"carrot=banana;}\n"				//38
												"apple=1;\n"					//39
												"kimchi=apple+10;\n"			//40
												"call extra;\n"					//41
										"}\n"
									"procedure extra{\n"
										"ingredients=1;\n"						//42
										"a=ingredients;\n"						//43
										"while ingredients{\n"					//44
												"b=a;\n"						//45
												"c=b;\n"						//46
												"a=c;\n"						//47
										"}\n"
									"}\n";


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
QUERY SampleQueries::sampleQuery1 = "stmt s; Select s such that Follows(s,4);";//pass
QUERY SampleQueries::sampleQuery2 = "stmt s; Select s such that Follows (2,s);";//pass
QUERY SampleQueries::sampleQuery3 = "stmt s1,s2,s Select <s1,s2> such that Follows (s1,s2);"; //pass
QUERY SampleQueries::sampleQuery4 = "stmt s1,s2; Select s2 such that Follows (s1,s2)"; //pass
QUERY SampleQueries::sampleQuery5 = "stmt s1,s2,s3; Select <s1,s2,s3> such that Follows (s1,s2) and Follows(s2,s3)";//pass
QUERY SampleQueries::sampleQuery6 = "stmt s1,s2,s3; Select s3 such that Follows (s1,s2) and Follows(s2,s3)";//pass
QUERY SampleQueries::sampleQuery7 = "stmt s;Select s with s.stmt#=2;"; //pass
QUERY SampleQueries::sampleQuery8 = "call c; procedure p; variable v;Select p with p.procName = \"Code5\";";
QUERY SampleQueries::sampleQuery9 = "while w; callc; Select w with w.stmt#=4;"; //pass
QUERY SampleQueries::sampleQuery10 = "variable v; Select v with v.varName = \"x\";"; //pass
QUERY SampleQueries::sampleQuery11 = "stmt s; while w; Select <s,w> with s.stmt#=w.stmt#;"; //pass
QUERY SampleQueries::sampleQuery12 = "variable v;Select v such that Modifies(_, v);"; 
QUERY SampleQueries::sampleQuery13 = "variable v; Select v such that Modifies(1, v);"; //fixed
QUERY SampleQueries::sampleQuery14 = "stmt s; Select s such that Uses(s, \"y\");"; 
QUERY SampleQueries::sampleQuery15 = "assign a; while w;Select a such that Follows* (a,w);"; // <-- this one suppose to return none right..
QUERY SampleQueries::sampleQuery16 = "while w; constant c;Select <w,c> with w.stmt#=c.value;"; //fixed
QUERY SampleQueries::sampleQuery17 = "assign a; Select a;"; //fixed
QUERY SampleQueries::sampleQuery18 = "stmt s; constant c;Select s with c.value=s.stmt#;";
QUERY SampleQueries::sampleQuery19 = "assign a; if i; while w; stmt s; variable v; procedure p; call cl; constant c;Select BOOLEAN with p.procName = \"procedure\";";
QUERY SampleQueries::sampleQuery20 = "stmt s1,s2;Select s1 such that Follows* ( s1 , s2 );";
QUERY SampleQueries::sampleQuery21 = "while w; Select BOOLEAN such that Parent (_,_);"; //pass
QUERY SampleQueries::sampleQuery22 = "Select BOOLEAN such that Uses(2,\"apple\");";//pass

//these two cases fail for PKB4, types are defined correctly but SUCHTAHT returns empty result - FIXED
QUERY SampleQueries::sampleQuery23 = "while w1,w2;Select w2 such that Parent (w1,w2);";
QUERY SampleQueries::sampleQuery24 = "while w;assign a;Select a such that Parent (w,a);";

//===============Test for Parent* =================== PKB4
QUERY SampleQueries::sampleQuery25 = "stmt s; Select s such that Parent*(s,5);";
QUERY SampleQueries::sampleQuery26 = "stmt s; Select s such that Parent*(1,s);";
QUERY SampleQueries::sampleQuery27 = "stmt s1,s2; Select s1 such that Parent* (s1,s2);";
QUERY SampleQueries::sampleQuery28 = "stmt s1; Select s1 such that Parent* (s1,_);";
QUERY SampleQueries::sampleQuery29 = "while w; Select w such that Parent* (_,_);";
QUERY SampleQueries::sampleQuery30 = "stmt s1; Select s1 such that Parent* (_,s1);";

//these two cases fail for PKB4, types are defined correctly but SUCHTAHT returns empty result - FIXED
QUERY SampleQueries::sampleQuery31 = "stmt s;Select s such that Follow* (s,s);";
QUERY SampleQueries::sampleQuery32 = "while w;assign a;Select a such that Parent* (w,a);";

//===============Test for Modifies=================== PKB4
QUERY SampleQueries::sampleQuery33 = "assign a;Select a such that Uses(a,\"innard\");";
QUERY SampleQueries::sampleQuery34 = "variable v;stmt s;Select v such that Modifies(s, v);";

//below hant tested
QUERY SampleQueries::sampleQuery35 = "variable v;while w;Select v such that Modifies(w, v);"; // <-- this didnt call suchthat?
QUERY SampleQueries::sampleQuery36 = "variable v;assign a;Select v such that Modifies(a, v);"; // <-- works ok
QUERY SampleQueries::sampleQuery37 = "variable v;Select v such that Modifies(_, v);"; // <-- works ok
QUERY SampleQueries::sampleQuery38 = "stmt s;Select s such that Modifies(s,\"banana\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery39 = "while w;Select w such that Modifies(w,\"apple\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery40 = "assign a;Select a such that Modifies(a,\"kimchi\");"; // empty result, works ok
QUERY SampleQueries::sampleQuery41 = "while w;Select w such that Modifies(_,_);";// <-- works ok

//===============Test for Pattern=================== PKB5
QUERY SampleQueries::sampleQuery42 = "assign a;Select a  pattern a(\"banana\",\"apple+1\");"; //pass
QUERY SampleQueries::sampleQuery43 = "assign a;variable v;Select v pattern a(v, _\"apple+1\"_);";     //pass 
QUERY SampleQueries::sampleQuery44 = "assign a;variable v;Select <a,v>  pattern a(v, _\"apple+10\"_);";     //pass 
QUERY SampleQueries::sampleQuery45 = "assign a;Select a  pattern a(\"banana\", _);";     //not pass 
QUERY SampleQueries::sampleQuery46 = "while w;Select w  pattern w(\"apple\",_);";  //pass
QUERY SampleQueries::sampleQuery47 = "while w;Select w  pattern w(_,_);"; //pass
QUERY SampleQueries::sampleQuery48 = "while w;Select w  pattern w(w,_);";  //pass
QUERY SampleQueries::sampleQuery49 = "if f;Select f pattern f(_,_)";  //pass
QUERY SampleQueries::sampleQuery50 = "if f;Select f pattern f(f,_)"; //no prob with qtree,pass
QUERY SampleQueries::sampleQuery51 = "assign a;variable v;Select v  pattern a(v,_\"apple\"_);";  //no prob with qtree

//===============Test for mix of suchthat and pattern=================== PKB5
QUERY SampleQueries::sampleQuery52 = "assign a;Select a such that Modifies(a,\"carrot\") pattern a(\"carrot\",_);"; //pass
QUERY SampleQueries::sampleQuery53 = "assign a;Select a such that Uses(a,\"kimchi\") pattern a(\"donut\",_);";    //pass
QUERY SampleQueries::sampleQuery54 = "assign a;variable v;Select v such that Uses(a,v) pattern a(v,_\"apple\"_);";  //pass
QUERY SampleQueries::sampleQuery55 = "assign a1,a2;Select a2 such that Parent(_,a1) pattern a1(\"carrot\",_);";  //pass

//===============Test for Boolean=================== PKB5	
QUERY SampleQueries::sampleQuery56 = "Select BOOLEAN such that Parent(2,3);";
//two clauses have relation and need to take intersection
QUERY SampleQueries::sampleQuery57 = "assign a;Select BOOLEAN such that Modifies(a,\"carrot\") pattern a(\"carrot\",_\"egg\"_);";
//two clauses have no relation
QUERY SampleQueries::sampleQuery58 = "assign a1,a2;Select BOOLEAN such that Modifies(a1,\"apple\") pattern a2(\"donut\",\"apple\");"; 
QUERY SampleQueries::sampleQuery59 = "assign a1,a2;Select BOOLEAN such that Modifies(a1,\"carrot\") pattern a2(\"donut\",\"apple\");";
QUERY SampleQueries::sampleQuery60 = "assign a1,a2;Select BOOLEAN such that Modifies(a1,\"carrot\") pattern a2(\"donut\",\"egg\");"; 
//test other cases
QUERY SampleQueries::sampleQuery61 = "assign a;Select BOOLEAN  such that Uses(a,\"vcz\");";
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