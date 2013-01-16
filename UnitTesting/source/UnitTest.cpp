#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{


// Get the top level suite from the registry
CppUnit::TestSuite *unitSuite = new CppUnit::TestSuite( "All unit test" );
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("VarTableTest").makeTest());	
unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProcedureTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ProcTableTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ASTTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ParserTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("QueryPreprocessorTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("FollowsTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ParentTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("ModifiesTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("UsesTest").makeTest());
//unitSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("PKBTest").makeTest());

CppUnit::TestFactoryRegistry::getRegistry().addTestToSuite(unitSuite);
CppUnit::TextUi::TestRunner runner;

runner.addTest(unitSuite);
bool wasSucessful = runner.run();

//getchar();
return wasSucessful ? 0 : 1;
}