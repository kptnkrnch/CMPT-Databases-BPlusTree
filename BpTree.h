#ifndef BPTREE_H
#define BPTREE_H

#include <string>
#include <fstream>
#include "Node.h"

class BpTree {
public:
	//Constructor
	BpTree(const int);
	BpTree(const BpTree&);

	//Destructor
	~BpTree();
	
	//Public Methods
	bool insert(const int, const std::string);
	bool remove(const int);
	std::string find(const int);
	void printKeys();
	void printValues();

	//Overloaded Operators
	BpTree& operator=(const BpTree&);
private:
	//Private Methods
	bool insertWithParentFull(Node*, Node**, const int, const std::string);
	bool findKey(const int);
	void removeOrCoalesceInteriorNodes(Node*);
	void updateInteriorNodeKeys(Node*);
	Node * findLeafNodeNeighbour(Node*);
	
	//Private Data
	int maxNodes; //maximum number of keys/nodes(for leaves) that can be stored in a node
	Node * head; //the head node of the tree
	bool isACopy; //flag to ensure that trees created through the overloaded = operator or
				  //the copy constructor do not delete head nodes that may have already been
				  //deleted (due to being shallow copies).
};

#endif