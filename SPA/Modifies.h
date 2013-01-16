#include "Global_Data_Abstraction.h"
using namespace std;
#include <map>
#include <utility>
#include <list>
class Modifies
{
public:
	Modifies(void);
	~Modifies(void);

	bool insertModifies(TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);

	//type can not be NULL. Take care of three cases:
	//1. getModifies(type, empty, 1)
	//2. getModifies(type,1,empty)
	//3. getModifies(type,empty, empty)
	MODIFIES_LIST getModifies(TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);
	bool isModifies(TYPE type, MODIFIES_INDEX index,VAR_INDEX varIndex);
	
	void debug();

private:
	multimap<int,multimap<int, int>> *modifiesDictionary;
	 multimap<int,multimap<int, int>>::iterator firstItr;
	 multimap<int,int>::iterator secondItr;
	
};