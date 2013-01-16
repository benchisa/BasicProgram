#pragma once
#include "QTREE.h"

QTREE * QTREE::createQTREENode(TYPE type, int data){
	this->type = type;
	this->data = data;

	leftSibling = NULL;
	rightSibling = NULL;
	ancestor = NULL;
	firstDescendant = NULL;

	return this;
}

bool QTREE::setFirstDescendant(QTREE* firstDescendant){
	//set the first child of the current node, and link the current node as the parent of the child
	if(this->firstDescendant==NULL && (*firstDescendant).ancestor==NULL){
		this->firstDescendant = firstDescendant;
		(*firstDescendant).ancestor = this;
		return true;	
	}
	return false;
}

bool QTREE::setAncestor(QTREE* ancestor){
	//link the parent node of current node as ancestor
	if(this->ancestor==NULL){
		this->ancestor=ancestor;
		return true;
	}
	return false;
}


bool QTREE::setSibling(QTREE* sibling){
	//link next node as current node's right sibling, link current node as left sibling of the next node
	if(rightSibling==NULL && (*sibling).leftSibling==NULL){
		rightSibling =sibling;
		(*sibling).leftSibling = this;
		return true;
	}
	return false;
}

int QTREE::getData(){
	return data;
}

TYPE QTREE::getType(){
	return type;
}

QTREE* QTREE::getFirstDescendant(){
	
	return firstDescendant;

}

QTREE* QTREE::getRightSibling(){

	return rightSibling;

}

QTREE* QTREE::getLeftSibling(){

	return leftSibling;

}

QTREE* QTREE::getAncestor(){

	return ancestor;

}

