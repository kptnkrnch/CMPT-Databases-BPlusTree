#include "BpTree.h"
#include <vector>
#include <iostream>

/* Name: Constructor
 * Params:
 *	const int maxKeys - The maximum number of keys that nodes of the tree can hold
 * Author: Joshua Campbell
 * Description:
 *	Creates a new BpTree with a maximum of maxKeys keys per node of the tree.
 */
BpTree::BpTree(const int maxKeys)
{
	this->maxNodes = maxKeys; //maxNodes and maxKeys are the same
	this->head = 0;
	this->isACopy = false;
}

/* Name: Copy Constructor
 * Params:
 *	const BpTree &tree - The BpTree that is being copied from
 * Author: Joshua Campbell
 * Description:
 *  Creates a new BpTree by copying an existing tree. Performs a shallow
 *	copy of the existing tree (both trees will point to the same nodes).
 */
BpTree::BpTree(const BpTree &tree) {
	this->maxNodes = tree.maxNodes;
	this->head = tree.head;
	this->isACopy = true;
}

/* Name: Destructor
 * Author: Joshua Campbell
 * Description:
 *	Destroys the BpTree and deletes all of the nodes on the tree.
 */
BpTree::~BpTree() {
	if (this->head != 0 && !(this->isACopy)) {
		delete head;
	}
	this->head = 0;
}

/* Name: Overloaded Operator: =
 * Params:
 *	const BpTree& other - The tree that is used as the source of information for the assignment
 * Author: Joshua Campbell
 * Description:
 *	Assigns the data from an existing tree to a different tree.
 */
BpTree& BpTree::operator=(const BpTree& other) {
	this->maxNodes = other.maxNodes;
	this->head = other.head;
	this->isACopy = true;
	return (*this);
}

/* Name: insert
 * Params:
 *	int key - The key that will identify the position of a string value in the tree.
 *	std::string value - The string value that will be inserted on a key.
 * Author: Joshua Campbell
 * Description:
 *	Inserts a new key/value pair into the existing tree. If no tree currently exists,
 *  a new head node (leaf node) is created and is set as the head node for the tree.
 * Returns: true if the key/value pair was inserted, false otherwise
 */
bool BpTree::insert(const int key, const std::string value)
{
	if (this->head != 0)
	{
		if (this->findKey(key) == true) {
			return false;
		}
		Node * current = this->head;
		while (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			current = static_cast<InteriorNode*>(current)->findNextNode(key);
		}
		if (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			//handling insertions when the current leaf node is full
			if (current->isFull())
			{
				Node * parent = current->getParent();
				//inserting a leaf node when it's parent is not full
				if (parent != 0 && parent->getNodeType() == NODE_TYPE_INTERIOR && !parent->isFull()) {
					Node** children = 0;
					if (current->getNodeType() == NODE_TYPE_INTERIOR)
					{
						children = static_cast<InteriorNode*>(current)->split();
					}
					else if (current->getNodeType() == NODE_TYPE_LEAF)
					{
						children = static_cast<LeafNode*>(current)->split(key, value);
					}
					static_cast<InteriorNode*>(parent)->addChild(children[1], children[1]->getKey(0));
					
					delete children;
					return true;
				}
				//Inserting a leaf node when its parent is full
				else if (parent != 0 && parent->getNodeType() == NODE_TYPE_INTERIOR && parent->isFull()) {
					Node** leafChildren = 0;
					if (current->getNodeType() == NODE_TYPE_INTERIOR)
					{
						leafChildren = static_cast<InteriorNode*>(current)->split();
					}
					else if (current->getNodeType() == NODE_TYPE_LEAF)
					{
						leafChildren = static_cast<LeafNode*>(current)->split(key, value);
					}
					InteriorNode * interiorNode = static_cast<InteriorNode*>(parent);
					int middleKey = interiorNode->getMiddleKey(leafChildren[1]->findIdentifierKey());
					if (interiorNode->getParent() != 0) {
						Node** interiorChildren = interiorNode->split(leafChildren[1]);
						
						if (interiorNode->getParent()->isFull()) {
							this->insertWithParentFull(interiorNode->getParent(), interiorChildren, middleKey, value);
						}
						else {
							static_cast<InteriorNode*>(interiorNode->getParent())->addChild(interiorChildren[1], middleKey);
						}
						delete interiorChildren;
					}
					else {
						this->head = new InteriorNode(this->maxNodes);
						Node** interiorChildren = interiorNode->split(leafChildren[1]);
						static_cast<InteriorNode*>(this->head)->addChild(interiorChildren[0]);
						static_cast<InteriorNode*>(this->head)->addChild(interiorChildren[1], middleKey);
						delete interiorChildren;
					}
					delete leafChildren;
					return true;
				}
				//inserting a leaf node when the head is a leaf node
				else if (parent == 0 && current == head && current->getNodeType() == NODE_TYPE_LEAF)
				{
					Node** children = 0;
					if (current->getNodeType() == NODE_TYPE_INTERIOR)
					{
						children = static_cast<InteriorNode*>(current)->split();
					}
					else if (current->getNodeType() == NODE_TYPE_LEAF)
					{
						children = static_cast<LeafNode*>(current)->split(key, value);
					}
					this->head = new InteriorNode(this->maxNodes);
					static_cast<InteriorNode*>(this->head)->addChild(children[0]);
					static_cast<InteriorNode*>(this->head)->addChild(children[1], children[1]->getKey(0));
					
					delete children;
					return true;
				}
				else {
					return false;
				}
			}
			else //inserting into the leaf when it is not full
			{
				LeafNode* leaf = static_cast<LeafNode*>(current);
				leaf->addPair(key, value);
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	else //creating a new head node when there was none previously
	{
		this->head = new LeafNode(this->maxNodes);
		LeafNode * temp = static_cast<LeafNode*>(this->head);
		temp->addPair(key, value);
		return true;
	}
	return false;
}

/* Name: insertWithParentFull
 * Params:
 *	Node* node - the parent node of a previous node that was split/divided into two new nodes
 *	Node** children - the two children node from a previous Node split/divided into two new nodes
 *	int key - the key value that will be used if there is a new Interior Node split
 *	std::string value - the value that needs to be inserted (unused)
 * Author: Joshua Campbell
 * Description:
 *	Handles updating parent nodes that are full and may need to be split after their children were
 *  split. If the parent node is full and needs to be split, this function will handle updating its
 *  parent node as well until no more parent nodes need to be split. If the parent node is a head 
 *  node and it needs to be split, a new head node will be made that contains the previous split 
 *  head node and its split sibling.
 * Returns: true in all cases
 */
bool BpTree::insertWithParentFull(Node* node, Node** children, const int key, const std::string value) {
	InteriorNode * interiorNode = static_cast<InteriorNode*>(node);
	int middleKey = interiorNode->getMiddleKey(key);
	if (interiorNode->getParent() != 0) {
		Node** interiorChildren = interiorNode->split(children[1], key);
		
		if (!static_cast<InteriorNode*>(interiorNode->getParent())->addChild(interiorChildren[1], middleKey)) {
			this->insertWithParentFull(interiorNode->getParent(), interiorChildren, middleKey, value);
		}
		delete interiorChildren;
	}
	else {
		this->head = new InteriorNode(this->maxNodes);
		Node** interiorChildren = interiorNode->split(children[1], key);
		
		static_cast<InteriorNode*>(this->head)->addChild(interiorChildren[0]);
		static_cast<InteriorNode*>(this->head)->addChild(interiorChildren[1], middleKey);
		delete interiorChildren;
	}
	return true;
}

/* Name: remove
 * Params:
 *	int key - the key that identifies a key/value pair that needs to be removed
 * Author: Joshua Campbell
 * Description:
 *	Removes a key/value pair from the tree if it exists. If a leaf node is less than
 *  half full after the key/value pair is removed, it may undergo redistribution from
 *  a left or a right neighbour, or it may undergo being coalesced into a neighbour.
 *  If upon being coalesced, the leaf node's parent is less than half full, the
 *  removeOrCoalesceInteriorNodes() method is called to handle its Interior parent
 *  node. Pointers between leaf nodes are updated upon coalescence.
 *  In order to ensure that parent interior nodes have the correct keys, the 
 *  updateInteriorNodeKeys() method is called to make sure that each the lowest value of
 *  each interior node's leaves can be reached (updates parent interior node keys).
 * Returns: true if the key was removed, false otherwise
 */
bool BpTree::remove(const int key)
{
	if (this->head != 0) {
		Node * current = this->head;
		//finding the leaf node that might contain the key/value pair to remove
		while (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			current = static_cast<InteriorNode*>(current)->findNextNode(key);
		}
		if (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			LeafNode * leafNode = static_cast<LeafNode*>(current);
			Node * parent = current->getParent();
			int keyIndex = leafNode->getKeyIndex(key);
			Node** neighbours = current->findNeighbours();
			int identifierKey = current->findIdentifierKey();
			Node * parentLeaf = this->findLeafNodeNeighbour(leafNode);
			Node * childLeaf = leafNode->getChild(leafNode->getMaxKeys());
			leafNode->deletePair(keyIndex); //deleting the key value pair
			int numberChildrenMidpoint = (leafNode->getMaxKeys() + 1) / 2;
			
			//if the node is less than half full...
			if (leafNode->getNumChildren() < numberChildrenMidpoint && neighbours != 0 && parent != 0) {
				if (neighbours[0] != 0 && neighbours[0]->getNumChildren() - 1 >= (neighbours[0]->getMaxKeys() + 1) / 2) {
					//redistribute values from left sibling to the current node
					int childKey = neighbours[0]->getKey(neighbours[0]->getNumKeys() - 1);
					std::string childValue = static_cast<LeafNode*>(neighbours[0])->getValue(neighbours[0]->getNumKeys() - 1);
					neighbours[0]->removeChild(neighbours[0]->getNumChildren() - 1);
					neighbours[0]->removeKey(neighbours[0]->getNumKeys() - 1);
					leafNode->addPair(childKey, childValue);
					int newCurrentIdentifierKey = current->findIdentifierKey();
					parent->setKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey), newCurrentIdentifierKey);
				}
				else if (neighbours[1] != 0 && neighbours[1]->getNumChildren() - 1 >= (neighbours[1]->getMaxKeys() + 1) / 2) {
					//redistribute values from right sibling to the current node
					int childKey = neighbours[1]->getKey(0);
					std::string childValue = static_cast<LeafNode*>(neighbours[1])->getValue(0);
					neighbours[1]->removeChild(0);
					neighbours[1]->removeKey(0);
					int newRightIdentifierKey = neighbours[1]->findIdentifierKey();
					parent->setKey(static_cast<InteriorNode*>(parent)->getKeyIndex(childKey), newRightIdentifierKey);
					leafNode->addPair(childKey, childValue);
					if (parent != 0 && parent->getChildIndex(current) > 0) {
						int keyIndex = static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey);
						parent->setKey(keyIndex, current->findIdentifierKey());
					}
				} 
				else if (neighbours[0] != 0 && neighbours[0]->getNumChildren() + leafNode->getNumChildren() <= neighbours[0]->getMaxKeys()) {
					//coalesce the current node with the left sibling
					LeafNode * neighbourNode = static_cast<LeafNode*>(neighbours[0]);
					
					for (int i = 0; i < leafNode->getNumChildren(); i++) {
						int tempKey = leafNode->getKey(i);
						std::string tempValue = leafNode->getValue(i);
						neighbourNode->addPair(tempKey, tempValue);
					}
					int numChildren = leafNode->getNumChildren();
					for (int i = 0; i < numChildren; i++) {
						leafNode->removePair(0);
					}

					if (childLeaf != 0 && neighbourNode->getNumChildren() >= 1 && neighbourNode != leafNode) {
						neighbourNode->setChild(neighbourNode->getMaxKeys(), childLeaf);
					}
					else if (childLeaf != 0 && parentLeaf != 0 && neighbourNode->getNumChildren() == 0 && neighbourNode == leafNode) {
						parentLeaf->setChild(parentLeaf->getMaxKeys(), childLeaf);
					}
					
					current = parent->deleteChild(current);
					parent->removeKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey));
					
					int maxKeys = (neighbourNode->getMaxKeys() + 1) / 2;
					if (parent->getNumChildren() < maxKeys || parent->getNumKeys() == 0) {
						removeOrCoalesceInteriorNodes(parent);
					}
					parent = neighbourNode->getParent();
				}
				else if (neighbours[1] != 0 && neighbours[1]->getNumChildren() + leafNode->getNumChildren() <= neighbours[1]->getMaxKeys()) {
					//coalesce the current node with the right sibling
					LeafNode * neighbourNode = static_cast<LeafNode*>(neighbours[1]);
					for (int i = 0; i < leafNode->getNumChildren(); i++) {
						int tempKey = leafNode->getKey(i);
						std::string tempValue = leafNode->getValue(i);
						neighbourNode->addPair(tempKey, tempValue);
					}
					int numChildren = leafNode->getNumChildren();
					for (int i = 0; i < numChildren; i++) {
						leafNode->removePair(0);
					}

					current = parent->deleteChild(current);
					parent->removeKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey));
					
					if (parentLeaf != 0) {
						parentLeaf->setChild(parentLeaf->getMaxKeys(), neighbourNode);
					}

					if (parent->getNumChildren() < (neighbourNode->getMaxKeys() + 1) / 2 || parent->getNumKeys() == 0) {
						removeOrCoalesceInteriorNodes(parent);
					}
					parent = neighbourNode->getParent();
				}
			}

			updateInteriorNodeKeys(this->head); //validate and update any interior node keys that may have fallen out of sync after
												//a strange redistribution or coalescence

			for (int i = 1; i < this->head->getNumChildren(); i++) {
				Node * child = this->head->getChild(i);
				if (child->getNodeType() == NODE_TYPE_INTERIOR) {
					this->head->setKey(i - 1, static_cast<InteriorNode*>(child)->findLeastLeafKey());
				}
			}
			
			if (parent != 0 && current != 0) {
				identifierKey = current->findIdentifierKey();
				int partitionKeyIndex = static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey);
				int partitionKey = parent->getKey(partitionKeyIndex);
				if (identifierKey >= partitionKey && current == parent->getChild(0)) {
					for (int i = 1; i < current->getNumKeys(); i++) {
						if (current->getKey(i) > partitionKey) {
							parent->setKey(partitionKeyIndex, current->getKey(i));
							break;
						}
					}
				}
			}
		}
	 else {
		 return false;
	 }
		
		return true;
	}
	else {
		return false;
	}
}

/* Name: removeOrCoalesceInteriorNodes
 * Params:
 *	Node* node - the node that is half full and needs to undergo redistribution or coalescence
 * Author: Joshua Campbell
 * Description:
 *	Handles redistribution or coalescence of interior nodes if they are less than half full.
 *  Checks if their parent is half full and if so, the removeOrCoalesceInteriorNodes()
 *  method is called for that parent. If the head node has only one child, that head node is
 *  removed and its child becomes the new head node.
 * Returns: None
 */
void BpTree::removeOrCoalesceInteriorNodes(Node* node) {
	if (node != 0) {
		InteriorNode * interiorNode = static_cast<InteriorNode*>(node);
		Node * parent = interiorNode->getParent();
		Node** neighbours = interiorNode->findNeighbours();
		int identifierKey = interiorNode->findIdentifierKey();

		//if the interior node is less than half full...
		if (interiorNode->getNumKeys() <= (interiorNode->getMaxKeys() + 1) / 2) {
			if (neighbours != 0 && neighbours[0] != 0 && neighbours[0]->getNumChildren() - 1 >= (neighbours[0]->getMaxKeys() + 1) / 2 && neighbours[0]->getNumKeys() > 1) {
				//redistribute values from left sibling to the current node
				int leastKeyValue = static_cast<InteriorNode*>(neighbours[0])->leastChildValue();
				int newKey = interiorNode->getChild(0)->findIdentifierKey();
				Node* childValue = static_cast<InteriorNode*>(neighbours[0])->getChild(neighbours[0]->getNumKeys());
				
				neighbours[0]->removeChild(neighbours[0]->getNumChildren() - 1);
				neighbours[0]->removeKey(neighbours[0]->getNumKeys() - 1);

				interiorNode->addChild(childValue, newKey);
				
				parent->setKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey), leastKeyValue);
			}
			else if (neighbours != 0 && neighbours[1] != 0 && neighbours[1]->getNumChildren() - 1 >= (neighbours[1]->getMaxKeys() + 1) / 2 && neighbours[1]->getNumKeys() > 1) {
				//redistribute values from right sibling to the current node
				int childKey = neighbours[1]->getKey(0);
				Node* childValue = static_cast<InteriorNode*>(neighbours[1])->getChild(0);
				childKey = childValue->findIdentifierKey();
				neighbours[1]->removeChild(0);
				neighbours[1]->removeKey(0);
				
				int newRightIdentifierKey = static_cast<InteriorNode*>(neighbours[1])->leastChildValue();
				parent->setKey(static_cast<InteriorNode*>(parent)->getKeyIndex(childKey), newRightIdentifierKey);
				interiorNode->addChild(childValue, childKey);
			}
			else if (neighbours != 0 && neighbours[0] != 0 && neighbours[0]->getNumKeys() + interiorNode->getNumKeys() - 1 <= neighbours[0]->getMaxKeys()) {
				//coalesce the current node with the left sibling
				InteriorNode * neighbourNode = static_cast<InteriorNode*>(neighbours[0]);
				for (int i = 0; i < interiorNode->getNumChildren(); i++) {
					Node* tempValue = interiorNode->getChild(i);
					int tempKey = tempValue->getKey(0);
					neighbourNode->addChild(tempValue, tempKey);
				}
				
				int numChildren = interiorNode->getNumChildren();
				for (int i = 0; i < numChildren; i++) {
					interiorNode->removeChild(0);
				}
				
				parent->deleteChild(interiorNode);
				interiorNode = 0;
				parent->removeKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey));
				
				if (parent->getNumChildren() < (neighbourNode->getMaxKeys() + 1) / 2 || parent->getNumKeys() == 0) {
					removeOrCoalesceInteriorNodes(parent);
				}
				parent = neighbourNode->getParent();
				
				if (parent != 0 && neighbourNode->getNumChildren() > 0 && neighbourNode != node) {
					int leastChildValue = neighbourNode->leastChildValue();
					
					if (neighbourNode != parent->getChild(0)) {
						int neighbourKeyIndex = static_cast<InteriorNode*>(parent)->getKeyIndex(neighbourNode->findIdentifierKey());
						parent->setKey(neighbourKeyIndex, leastChildValue);
					}
					else if (parent->getNumChildren() > 1){
						leastChildValue = static_cast<InteriorNode*>(parent->getChild(1))->leastChildValue();
						parent->setKey(0, leastChildValue);
					}
				}
			}
			else if (neighbours != 0 && neighbours[1] != 0 && neighbours[1]->getNumKeys() + interiorNode->getNumKeys() - 1 <= neighbours[1]->getMaxKeys()) {
				//coalesce the current node with the right sibling
				InteriorNode * neighbourNode = static_cast<InteriorNode*>(neighbours[1]);

				for (int i = interiorNode->getNumChildren() - 1; i >= 0; i--) {
					if (i == interiorNode->getNumChildren() - 1 && neighbourNode->getNumChildren() > 1) {
						int tempKey = neighbourNode->getChild(0)->getKey(0);
						Node* tempValue = interiorNode->getChild(i);
						neighbourNode->addChild(tempValue, tempKey);
					}
					else if (i == interiorNode->getNumChildren() - 1 && neighbourNode->getNumChildren() == 1 && neighbourNode->getNumKeys() == 1) {
						Node* tempValue = interiorNode->getChild(i);
						neighbourNode->addChildToLeft(tempValue);
					}
					else {
						int tempKey = interiorNode->getKey(i);
						if (tempKey != -1) {
							Node* tempValue = interiorNode->getChild(i);
							neighbourNode->addChild(tempValue, tempKey);
						}
						else {
							Node* tempValue = interiorNode->getChild(i);
							tempKey = tempValue->getKey(0);
							neighbourNode->addChild(tempValue, tempKey);
						}
					}
				}
				int numChildren = interiorNode->getNumChildren();
				for (int i = 0; i < numChildren; i++) {
					interiorNode->removeChild(0);
				}
				
				parent->deleteChild(interiorNode);
				interiorNode = 0;
				parent->removeKey(static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey));

				if (parent->getNumChildren() < (neighbourNode->getMaxKeys() + 1) / 2 || parent->getNumKeys() == 0) {
					removeOrCoalesceInteriorNodes(parent);
				}
			}
			else if (interiorNode == this->head && this->head->getNumChildren() == 1) {
				//if the head only contains one child, the current head is set to that child
				//and the previous head node is deleted/removed.
				Node * temp = this->head;
				this->head = interiorNode->getChild(0);
				this->head->setParent(0);

				const int numChildren = temp->getNumChildren();
				for (int i = 0; i < numChildren; i++) {
					temp->removeChild(0);
				}
				delete temp;
				
				for (int i = 1; i < this->head->getNumChildren(); i++) {
					Node * child = this->head->getChild(i);
					if (child->getNodeType() == NODE_TYPE_INTERIOR) {
						int leastChildValue = static_cast<InteriorNode*>(child)->leastChildValue();
						this->head->setKey(i - 1, leastChildValue);
					}
					else if (child->getNodeType() == NODE_TYPE_LEAF) {
						int leastChildValue = static_cast<LeafNode*>(child)->findIdentifierKey();
						this->head->setKey(i - 1, leastChildValue);
					}
				}
				return;
			}
			if (parent != 0 && interiorNode != 0) {
				identifierKey = interiorNode->findIdentifierKey();
				int partitionKeyIndex = static_cast<InteriorNode*>(parent)->getKeyIndex(identifierKey);
				int partitionKey = parent->getKey(partitionKeyIndex);
				if (identifierKey >= partitionKey && interiorNode == parent->getChild(0)) {	
					for (int i = 1; i < interiorNode->getNumKeys(); i++) {
						if (interiorNode->getKey(i) > partitionKey) {
							parent->setKey(partitionKeyIndex, interiorNode->getKey(i));
							break;
						}
					}
				}
			}
		}
	}
}

/* Name: findLeafNodeNeighbour
 * Params:
 *	Node * rightMostChildNode - the node that the leaves are searched for to see if
								their right-most child is a pointer to this node.
 * Author: Joshua Campbell
 * Description:
 *	Finds the leaf node that is directly to the left of rightMostChildNode node.
 * Returns: the node that has its righmost child pointer equal to the rightMostChildNode node
 */
Node* BpTree::findLeafNodeNeighbour(Node * rightMostChildNode) {
	Node * current = this->head;
	if (current != 0) {
		while (current->getNodeType() == NODE_TYPE_INTERIOR) {
			current = current->getChild(0);
		}
		while (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			if (current->getChild(current->getMaxKeys()) == rightMostChildNode) {
				return current;
			}
			current = current->getChild(current->getMaxKeys());
			if (current == 0) {
				break;
			}
		}
		return 0;
	}
	else {
		return 0;
	}
}

/* Name: findKey
 * Params:
 *	int key - key that needs to be found in the leaves of the tree
 * Author: Joshua Campbell
 * Description:
 *	Checks to see if the key exists in the leaves of the tree.
 * Returns: true if the key was found, false otherwise
 */
bool BpTree::findKey(const int key) {
	bool notFound = false;
	if (this->head != 0 && this->head->getNodeType() == NODE_TYPE_LEAF) {
		int index = this->head->getKeyIndex(key);
		if (index != -1)
		{
			return true;
		}
		else
		{
			return notFound;
		}
	}
	else if (this->head != 0 && this->head->getNodeType() == NODE_TYPE_INTERIOR)
	{
		Node * current = this->head;
		while (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			int index = static_cast<InteriorNode*>(current)->getKeyIndex(key);
			int currentKey = current->getKey(index);
			if (key < currentKey) {
				if (current->getNumChildren() >= 1) {
					current = current->getChild(index);
				} else {
					current = 0;
				}
			}
			else
			{
				if (current->getNumChildren() >= 2) {
					current = current->getChild(index + 1);
				}
				else if (current->getNumChildren() == 1) {
					current = current->getChild(0);
				}
				else {
					current = 0;
				}
			}
		}
		if (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			int index = current->getKeyIndex(key);
			if (index != -1)
			{
				return true;
			}
			else
			{
				return notFound;
			}
		}
		else
		{
			return notFound;
		}
	}
	return notFound;
}

/* Name: find
 * Params:
 *	int key - key that needs to be found in the leaves of the tree
 * Author: Joshua Campbell
 * Description:
 *	Searches the tree for the key and returns its string value if found.
 *  If it cannot be found, the empty string is returned.
 * Returns: a string containing the value stored on the key
 */
std::string BpTree::find(const int key)
{
	std::string errorMessage = "";
	if (this->head->getNodeType() == NODE_TYPE_LEAF) {
		int index = this->head->getKeyIndex(key);
		if (index != -1)
		{
			LeafNode * temp = static_cast<LeafNode*>(this->head);
			return temp->getValue(index);
		}
		else 
		{
			return errorMessage;
		}
	}
	else if (this->head->getNodeType() == NODE_TYPE_INTERIOR)
	{
		Node * current = this->head;
		while (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			int index = static_cast<InteriorNode*>(current)->getKeyIndex(key);
			int currentKey = current->getKey(index);
			if (key < currentKey && current->getNumChildren() >= 1) {
				current = current->getChild(index);
			}
			else if (current->getNumChildren() >= 2) 
			{
				current = current->getChild(index + 1);
			}
			else if (current->getNumChildren() >= 1) {
				current = current->getChild(0);
			}
			else {
				current = 0;
			}
		}
		if (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			int index = current->getKeyIndex(key);
			if (index != -1)
			{
				LeafNode * temp = static_cast<LeafNode*>(current);
				return temp->getValue(index);
			}
			else
			{
				return errorMessage;
			}
		}
		else 
		{
			return errorMessage;
		}
	}
	return errorMessage;
}

/* Name: printKeys
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Prints all of the keys for the tree in order of the levels of the tree they appear on.
 * Returns: None
 */
void BpTree::printKeys()
{
	std::vector<std::vector<Node*> > levels;
	std::vector<Node*> queue;
	if (this->head != 0) {
		queue.push_back(this->head);
		levels.push_back(queue);
		while (levels.size() > 0) {
			queue = levels.at(0);
			levels.erase(levels.begin());
			std::vector<Node*> nextLevel;
			while (queue.size() > 0) {
				Node * current = queue.at(0);
				queue.erase(queue.begin());
				std::cout << "[";
				for (int i = 0; i < current->getNumChildren(); i++) {
					if (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
						nextLevel.push_back(current->getChild(i));
					}
				}
				for (int i = 0; current != 0 && i < current->getNumKeys(); i++) {
					if (i + 1 == current->getNumKeys()) {
						std::cout << current->getKey(i);
					}
					else {
						std::cout << current->getKey(i) << ",";
					}
				}
				std::cout << "] ";
			}
			if (nextLevel.size() > 0) {
				levels.push_back(nextLevel);
			}
			std::cout << std::endl;
		}
	}
}

/* Name: printValues
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Prints the string values stored in the tree from the lowest key value to the highest key value.
 * Returns: None 
 */
void BpTree::printValues()
{
	Node* current = this->head;
	if (current != 0) {
		while (current != 0 && current->getNumChildren() > 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			current = current->getChild(0);
		}
		while (current != 0 && current->getNodeType() == NODE_TYPE_LEAF) {
			for (int i = 0; i < current->getNumKeys(); i++) {
				std::cout << static_cast<LeafNode*>(current)->getValue(i) << std::endl;
			}
			current = current->getChild(current->getMaxKeys());
			if (current == 0) {
				break;
			}
		}
	}
}

/* Name: updateInteriorNodeKeys
 * Params:
 *	Node* node - starting node from where to update its keys and its childrens keys
 * Author: Joshua Campbell
 * Description:
 *	Updates the keys of all interior nodes from a starting node so that they can be used to find
 *  the lowest key value of the leftmost child leaf. It does not update the leaf nodes' keys or 
 *  their direct parent node's keys.
 * Returns: None
 */
void BpTree::updateInteriorNodeKeys(Node* node) {
	for (int i = 0; i < node->getNumChildren(); i++) {
		Node * child = node->getChild(i);
		if (child != 0 && child->getNodeType() == NODE_TYPE_INTERIOR && child->getNumChildren() > 0 && child->getChild(0)->getNodeType() == NODE_TYPE_INTERIOR) {
			this->updateInteriorNodeKeys(child);
			for (int k = 1; k < child->getNumChildren(); k++) {
				int lowestLeafKeyValue = static_cast<InteriorNode*>(child->getChild(k))->findLeastLeafKey();
				child->setKey(k - 1, lowestLeafKeyValue);
			}
		}
		else {
			return;
		}
	}
}
