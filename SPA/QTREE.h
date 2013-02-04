#pragma once
#include "Global_Data_Abstraction.h"


class QTREE{

public:
	//type will tell what is data representing
	QTREE * createQTREENode(QTREENODE_TYPE, int);
	bool setFirstDescendant(QTREE*);
	void setLastDescendant(QTREE*);
	bool setAncestor(QTREE*);
	bool setSibling(QTREE*);
	void setLastSibling(QTREE*);
	void setType(TYPE);
	void setData(int);

	int getData();
	QTREENODE_TYPE getType();
	QTREE* getFirstDescendant();
	QTREE* getLastDescendant();
	QTREE* getRightSibling();
	QTREE* getLeftSibling();
	QTREE* getLastSibling();
	QTREE* getAncestor();

private:
	
	QTREENODE_TYPE type;
	int data;

	QTREE* leftSibling;
	QTREE* rightSibling;
	QTREE* ancestor;
	QTREE* firstDescendant;
	QTREE* lastDescendant;
	QTREE* lastSibling;
};