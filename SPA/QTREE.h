#pragma once
#include "Global_Data_Abstraction.h"


class QTREE{

public:
	//type will tell what is data representing
	QTREE * createQTREENode(QTREENODE_TYPE type, int data);
	bool setFirstDescendant(QTREE* firstDescendant);
	bool setAncestor(QTREE* ancestor);
	bool setSibling(QTREE* rightSibling);
	bool setType(QTREENODE_TYPE type);
	bool setData(int data);

	int getData();
	QTREENODE_TYPE getType();
	QTREE* getFirstDescendant();
	QTREE* getRightSibling();
	QTREE* getLeftSibling();
	QTREE* getAncestor();

private:
	
	QTREENODE_TYPE type;
	int data;

	QTREE* leftSibling;
	QTREE* rightSibling;
	QTREE* ancestor;
	QTREE* firstDescendant;
};