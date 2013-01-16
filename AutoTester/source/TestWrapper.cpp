#include "TestWrapper.h"

using namespace std;

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	controller = new SPAController();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	string line;
	SOURCE src;
	ifstream file(filename);
	if(file.is_open()){
		while(file.good()){
			getline(file, line);
			src+= line;
		}

		file.close();
	}
	// call parser
	controller->enterProgram(src);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
	//call query evaluator
	
	// cannot run with this uncommented. Autotester will crash
	results = controller->getQueryResult(query);

	//crafted result
//	std::list<std::string> crafted_result;
//	crafted_result.push_back("a");
//	results = crafted_result;
}
