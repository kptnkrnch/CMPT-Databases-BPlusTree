#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>
#include <vector>

/* Constants used for determining the type of a Node for casting */
#define NODE_TYPE_NONE          0
#define NODE_TYPE_LEAF          1
#define NODE_TYPE_INTERIOR      2
#define NODE_TYPE_DATA			3

class Node {
public:
	Node();
	~Node();
	Node(int);
	bool generateChildren(int);

	Node* findNextNode(int);
	Node** findNeighbours();
	int findIdentifierKey();
	int getNodeType();

	Node* getParent();

	Node** split();
	Node** split(int, std::string);

	int getNumKeys();
	int getMaxKeys();
	void setNumKeys(int);
	int getKey(int);
	void removeKey(int);
	bool setKey(int, int);

	int getKeyIndex(int);

	Node* getChild(int);
	int getChildIndex(Node*);
	bool setChild(int, Node*);
	Node* removeChild(int);
	Node* removeChild(Node*);
	void deleteChild(int);
	Node* deleteChild(Node*);
	int getNumChildren();
	
	void setParent(Node *);

	bool isFull();

	/* debugging tools */
	void printKeys();
	void printChildren();
protected:
	Node * parent; //the parent of the node
	Node ** children; //the children of the node (allocated array (dynamic memory))
	int * keys; //the keys for the node (allocated array (dynamic memory))
	int numKeys; //the current number of keys held by the node
	int maxKeys; //the maximum number of keys held by the node
	int numChildren; //the current number of children held by the node
	int type; //the type of the node (leaf, interior, data, or no type); used for casting
};

class LeafNode : public Node {
public:
	LeafNode(int);

	void addPair(int, std::string);

	Node* findNextNode(int);

	Node** split();
	Node** split(int, std::string);

	int* getKeys();
	std::string* getValues();
	int getKey(int);
	std::string getValue(int);
	
	void removePair(int);
	bool deletePair(int);

	int getNumChildren();
};

class InteriorNode : public Node {
public:
	InteriorNode(int);
	int* getKeys();
	bool addChild(Node*);
	bool addChild(Node*, int);
	bool addChildToLeft(Node*);
	bool insertChild(Node*, int);
	int getKeyIndex(int);
	int getMiddleKey();
	int getMiddleKey(int);
	Node** findNeighbours();
	Node* findNextNode(int);
	Node** split();
	Node** split(Node*);
	Node** split(Node*, int);
	int leastChildValue();
	int findLeastLeafKey();
};

class DataNode : public Node {
public:
	DataNode(std::string);
	std::string value; //the string value that is stored
};

#endif