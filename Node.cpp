#include "Node.h"

/* Name: Node Constructor
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Creates a new Node with no type.
 */
Node::Node() {
	this->numChildren = 0;
	this->parent = 0;
	this->children = 0;

	this->numKeys = 0;
	this->maxKeys = 0;
	this->keys = 0;
	this->type = NODE_TYPE_NONE;
}

/* Name: Node Constructor
 * Params:
 *	const int maxKeys - The maximum number of keys that the node can hold
 * Author: Joshua Campbell
 * Description:
 *	Creates a new Node with a specified number of max keys. The node has no type.
 */
Node::Node(int maxKeys) 
{
	this->numChildren = 0;
	this->parent = 0;
	this->children = 0;
	
	this->numKeys = 0;
	this->maxKeys = maxKeys;
	this->keys = new int[maxKeys];
	
	this->type = NODE_TYPE_NONE;
}

/* Name: Node Destructor
 * Author: Joshua Campbell
 * Description:
 *	Destroys the Node freeing up any keys or children that were allocated.
 */
Node::~Node() {
	if (this->keys != 0) {
		delete[] this->keys;
		this->keys = 0;
	}
	for (int i = 0; i < this->numChildren && this->children != 0; i++) {
		Node * child = this->children[i];
		if (child != 0) {
			delete child;
		}
	}
	
	if (this->children != 0) {
		delete[] this->children;
		this->children = 0;
	}
}

/* Name: findNeighbours (Node)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Finds the nodes to the left and right of the current node (based on its position in its
 *	parents children array).
 * Returns: a pointer to the left and right neighbours of the node (based on its position in
 *			its parents children)
 */
Node** Node::findNeighbours() {
	bool found = false;
	if (this->parent != 0) {
		Node** neighbours = new Node*[2];
		neighbours[0] = 0;
		neighbours[1] = 0;
		Node* parent = this->parent;
		int identifier = this->keys[0];
		for (int i = 0; i < parent->getNumKeys(); i++) {
			int currentKey = parent->getKey(i);
			if (identifier >= currentKey) {
				if (i + 1 > 0) {
					neighbours[0] = parent->getChild(i);
					found = true;
				}
				if (i + 2 < parent->getNumChildren()) {
					neighbours[1] = parent->getChild(i + 2);
					found = true;
				}
			}
		}
		if (parent->getNumKeys() >= 1 && !found) {
			neighbours[1] = parent->getChild(1);
		}
		return neighbours;
	}
	else {
		return 0;
	}
}

/* Name: findIdentifierKey
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Finds the leftmost key that is used to identify the node.
 * Returns: the node's leftmost key
 */
int Node::findIdentifierKey() {
	return this->keys[0];
}

/* Name: getNodeType
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the type of the node (used for casting).
 * Returns: the type of the node (integer contants in Node.h)
 */
int Node::getNodeType()
{
	return this->type;
}

/* Name: getNumKeys
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the current number of keys stored in the node.
 * Returns: the current number of keys stored in the node
 */
int Node::getNumKeys()
{
	return this->numKeys;
}

/* Name: getMaxKeys
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the maximum number of keys that the node can hold.
 * Returns: the maximum number of keys that the node can hold.
 */
int Node::getMaxKeys()
{
	if (this != 0) {
		return this->maxKeys;
	}
	else {
		return -1;
	}
}

/* Name: setNumKeys
 * Params:
 *	int numKeys - the current number of keys the node is holding
 * Author: Joshua Campbell
 * Description:
 *	Sets the numKeys parameter of the node to the number of keys specified 
 * Returns: None
 */
void Node::setNumKeys(int numKeys)
{
	this->numKeys = numKeys;
}

/* Name: setKey
 * Params:
 *	int index - the index of the key to be updated
 *	int keyValue - the new value for the key
 * Author: Joshua Campbell
 * Description:
 *	Sets the key at the specified index to the new key's value;
 * Returns: true if the key was set, false otherwise
 */
bool Node::setKey(int index, int keyValue) {
	if (index >= 0 && index < this->numKeys) {
		this->keys[index] = keyValue;
		return true;
	}
	else {
		return false;
	}
}

/* Name: removeKey
 * Params:
 *	int index - the index of the key to remove
 * Author: Joshua Campbell
 * Description:
 *	Removes the key from the specified index and shifts the keys to the right of it
 * Returns: None
 */
void Node::removeKey(int index) {
	if (index >= 0 && index < this->numKeys) {
		for (int i = index; i < this->numKeys - 1; i++) {
			this->keys[i] = this->keys[i + 1];
		}
		for (int i = numKeys; i < this->maxKeys; i++) {
			this->keys[i] = 0;
		}
		this->numKeys -= 1;
	}
}

/* Name: printKeys
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Debugging function used to print the node's keys to stdout.
 * Returns: None
 */
void Node::printKeys() {
	for (int i = 0; i < this->numKeys; i++) {
		std::cout << this->keys[i] << " ";
	}
	std::cout << std::endl;
}

/* Name: getParent
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the parent of the current node.
 * Returns: a pointer to the parent of the current node
 */
Node* Node::getParent()
{
	return this->parent;
}

/* Name: getKeyIndex
 * Params:
 *	int key - the key to get the index of
 * Author: Joshua Campbell
 * Description:
 *	Finds the index of the key in the current node that has the exact same value
 *	as the specified key parameter
 * Returns: the index of the key, -1 otherwise
 */
int Node::getKeyIndex(int key) {
	for (int i = 0; i < this->numKeys; i++) {
		int currentKey = this->keys[i];
		if (currentKey == key) {
			return i;
		}
	}
	return -1;
}

/* Name: getChild
 * Params:
 *	int child - the index of the child
 * Author: Joshua Campbell
 * Description:
 *	Returns the child at the "child" index.
 * Returns: the child at the "child" index
 */
Node* Node::getChild(int child)
{
	if (child >= 0 && child < this->numChildren) {
		return this->children[child];
	}
	else if (this->getNodeType() == NODE_TYPE_LEAF && child == this->getMaxKeys()) {
		return this->children[child];
	}
	else {
		return 0;
	}
}

/* Name: getChildIndex
 * Params:
 *	Node* child - the node to get the index of
 * Author: Joshua Campbell
 * Description:
 *	Returns the index of the "child" node if it is in the children of the current node.
 * Returns: the index of the "child" node
 */
int Node::getChildIndex(Node* child) {
	for (int i = 0; i < this->numChildren; i++) {
		if (this->children[i] == child) {
			return i;
		}
	}
	return -1;
}

/* Name: setChild
 * Params:
 *	int index - the index of the child to update
 *	Node* child - the new child pointer for the specified index
 * Author: Joshua Campbell
 * Description:
 *	Sets the child pointer at the specified index to the new child node pointer
 * Returns: true if the child is set, false otherwise
 */
bool Node::setChild(int index, Node* child) {
	if (index >= 0 && index <= this->maxKeys) {
		this->children[index] = child;
		return true;
	}
	return false;
}

/* Name: removeChild
 * Params:
 *	int child - the index of the child to remove
 * Author: Joshua Campbell
 * Description:
 *	Removes the child from the specified index, shifts all children to the right of that index.
 * Returns: the pointer to the removed child
 */
Node* Node::removeChild(int child)
{
	Node* removedChild = this->children[child];
	for (int i = child; i < this->numChildren - 1; i++) {
		this->children[i] = this->children[i + 1];
	}
	for (int i = numChildren; i < this->maxKeys; i++) {
		this->children[i] = 0;
	}
	this->numChildren -= 1;
	return removedChild;
}

/* Name: removeChild
 * Params:
 *	Node* node - the pointer to the child to remove
 * Author: Joshua Campbell
 * Description:
 *	Removes the child that shares the same pointer as the specified node pointer.
 * Returns: the pointer to the removed child, 0 otherwise
 */
Node* Node::removeChild(Node* node)
{
	int index = -1;
	for (int i = 0; i < this->numChildren; i++) {
		if (this->children[i] == node) {
			index = i;
			break;
		}
	}
	if (index >= 0) {
		Node* removedChild = this->children[index];
		for (int i = index; i < this->numChildren - 1; i++) {
			this->children[i] = this->children[i + 1];
		}
		for (int i = numChildren; i < this->maxKeys; i++) {
			this->children[i] = 0;
		}
		this->numChildren -= 1;
		return removedChild;
	}
	else {
		return 0;
	}
}

/* Name: deleteChild
 * Params:
 *	int child - the index of the child to remove and delete
 * Author: Joshua Campbell
 * Description:
 *	Removes and deletes the child from the specified index, shifts all children to the 
 *	right of that index.
 * Returns: None
 */
void Node::deleteChild(int child)
{
	delete this->children[child];
	this->children[child] = 0;
	for (int i = child; i < this->numChildren - 1; i++) {
		this->children[i] = this->children[i + 1];
	}
	for (int i = numChildren; i < this->maxKeys; i++) {
		this->children[i] = 0;
	}
	this->numChildren -= 1;
}

/* Name: deleteChild
 * Params:
 *	Node* node - the pointer to the child to remove and delete
 * Author: Joshua Campbell
 * Description:
 *	Removes and deletes the child that shares the same pointer as the specified node pointer.
 * Returns: 0
 */
Node* Node::deleteChild(Node* node)
{
	int index = -1;
	for (int i = 0; i < this->numChildren; i++) {
		if (this->children[i] == node) {
			index = i;
			break;
		}
	}
	if (index >= 0) {
		delete this->children[index];
		this->children[index] = 0;
		for (int i = index; i < this->numChildren - 1; i++) {
			this->children[i] = this->children[i + 1];
		}
		for (int i = numChildren; i < this->maxKeys; i++) {
			this->children[i] = 0;
		}
		this->numChildren -= 1;
	}
	return 0;
}

/* Name: split (Node)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Placeholder method, no functionality at this time.
 * Returns: 0
 */
Node** Node::split()
{
	return 0;
}

/* Name: getKey
 * Params:
 *	int index - the index of the key to retrieve
 * Author: Joshua Campbell
 * Description:
 *	Returns the key at the specified index.
 * Returns: the key at the specified index, -1 otherwise
 */
int Node::getKey(int index) {
	if (index >= 0 && index < this->numKeys) {
		return this->keys[index];
	}
	else {
		return -1;
	}
	
}

/* Name: getNumChildren
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the number of children currently contained in the node.
 * Returns: the number of children currently contained in the node
 */
int Node::getNumChildren() {
	return this->numChildren;
}

/* Name: split (LeafNode)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Divides the current leafnode into two halves and returns pointers to the two nodes.
 * Returns: pointers to the original node and the new split node
 */
Node** LeafNode::split()
{
	Node** newNodes = new Node*[2];
	newNodes[0] = this;
	newNodes[1] = new LeafNode(this->maxKeys);
	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2) + 1;
	}
	else
	{
		middleKey = (this->maxKeys / 2);
	}
	for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
		static_cast<LeafNode*>(newNodes[1])->addPair(static_cast<LeafNode*>(newNodes[0])->getKey(i), 
			static_cast<LeafNode*>(newNodes[0])->getValue(i));
	}
	for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
		static_cast<LeafNode*>(newNodes[0])->removePair(i);
	}
	newNodes[0]->setChild(newNodes[0]->getMaxKeys(), newNodes[1]);
	return newNodes;
}

/* Name: split (LeafNode)
 * Params:
 *	int key - the new key that is being added during the splitting process
 *	std::string value - the new value that is being added during the splitting process
 * Author: Joshua Campbell
 * Description:
 *	Splits the current node into two halves and uses the new key/value pair to decide how many
 *  data elements will be redistributed to the new leaf node.
 * Returns: pointers to the original node and the new split node
 */
Node** LeafNode::split(int key, std::string value)
{
	Node** newNodes = new Node*[2];
	newNodes[0] = this;
	newNodes[1] = new LeafNode(this->maxKeys);
	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2);
	}
	else
	{
		middleKey = (this->maxKeys / 2) - 1;
	}
	
	if (key < (newNodes[0])->getKey(middleKey))
	{
		for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
			static_cast<LeafNode*>(newNodes[1])->addPair(static_cast<LeafNode*>(newNodes[0])->getKey(i),
				static_cast<LeafNode*>(newNodes[0])->getValue(i));
		}
		for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
			static_cast<LeafNode*>(newNodes[0])->removePair(middleKey);
		}
		static_cast<LeafNode*>(newNodes[0])->addPair(key, value);
	}
	else
	{
		for (int i = middleKey + 1; i < newNodes[0]->getMaxKeys(); i++) {
			static_cast<LeafNode*>(newNodes[1])->addPair(static_cast<LeafNode*>(newNodes[0])->getKey(i),
				static_cast<LeafNode*>(newNodes[0])->getValue(i));
		}
		for (int i = middleKey + 1; i < newNodes[0]->getMaxKeys(); i++) {
			static_cast<LeafNode*>(newNodes[0])->removePair(middleKey + 1);
		}
		static_cast<LeafNode*>(newNodes[1])->addPair(key, value);
	}
	newNodes[1]->setChild(newNodes[1]->getMaxKeys(), newNodes[0]->getChild(newNodes[0]->getMaxKeys()));
	newNodes[0]->setChild(newNodes[0]->getMaxKeys(), newNodes[1]);
	return newNodes;
}

/* Name: isFull
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Checks if the node's key array is full.
 * Returns: true if the node is full of keys, false otherwise
 */
bool Node::isFull() {
	if (this->numKeys >= this->maxKeys)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Name: setParent
 * Params:
 *	Node * parent - the new parent node for the current node
 * Author: Joshua Campbell
 * Description:
 *	Sets the parent pointer of the node to the specified new parent.
 * Returns: None
 */
void Node::setParent(Node * parent) {
	this->parent = parent;
}

/* Name: findNextNode (Node)
 * Params:
 *	int key - placeholder
 * Author: Joshua Campbell
 * Description:
 *	Placeholder method, no real functionality at this time.
 * Returns: 0
 */
Node* Node::findNextNode(int key) { return 0; }

/* Name: LeafNode Constructor
 * Params:
 *	int maxKeys - the maximum number of keys and data elements the node can hold
 * Author: Joshua Campbell
 * Description:
 *	Creates a new LeafNode with the specified number of keys and data elements.
 *	One more child pointer is allocated for pointing to its right neighbour leaf.
 */
LeafNode::LeafNode(int maxKeys) : Node(maxKeys)
{
	this->type = NODE_TYPE_LEAF;
	this->children = new Node*[maxKeys + 1];
	for (int i = 0; i < maxKeys + 1; i++) {
		this->children[i] = 0;
	}
}

/* Name: findNextNode (LeafNode)
 * Params:
 *	int key - unused
 * Author: Joshua Campbell
 * Description:
 *	Returns the pointer to the right neighbour node of the leaf.
 * Returns: the pointer to the right neighbour node of the leaf
 */
Node* LeafNode::findNextNode(int key) 
{
	return this->children[this->maxKeys];
}

/* Name: addPair
 * Params:
 *	int key - the key to add to the leaf
 *	std::string value - the value to add to the leaf
 * Author: Joshua Campbell
 * Description:
 *	Adds the key value pair into the appropriate spot in the leaf.
 * Returns: None
 */
void LeafNode::addPair(int key, std::string value)
{
	bool inserted = false;
	if (this->numChildren < this->maxKeys) {
		for (int i = 0; i < this->numChildren; i++) {
			if (key < this->keys[i]) {
				for (int k = this->numChildren - 1; k >= i; k--) {
					this->keys[k + 1] = this->keys[k];
					this->children[k + 1] = this->children[k];
				}
				this->keys[i] = key;
				this->children[i] = new DataNode(value);
				inserted = true;
				break;
			}
		}
		if (!inserted) {
			this->keys[this->numChildren] = key;
			this->children[this->numChildren] = new DataNode(value);
		}
		this->numChildren += 1;
		this->numKeys += 1;
	}
}

/* Name: getKeys
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the pointer to the key array of the leaf.
 * Returns: the pointer to the key array of the leaf
 */
int* LeafNode::getKeys()
{
	return this->keys;
}

/* Name: getKey
 * Params:
 *	int index - the index of the key to retrieve
 * Author: Joshua Campbell
 * Description:
 *	Returns the key at the specified index.
 * Returns: the key at the specified index
 */
int LeafNode::getKey(int index)
{
	return this->keys[index];
}

/* Name: getValue
 * Params:
 *	int index - the index of the string value to retrieve
 * Author: Joshua Campbell
 * Description:
 *	Returns the string value of the DataNode stored at the specified index of the leaf.
 * Returns: the string value of the DataNode stored at the specified index of the leaf
 */
std::string LeafNode::getValue(int index)
{
	Node * child = this->children[index];
	if (child->getNodeType() == NODE_TYPE_DATA) {
		return static_cast<DataNode*>(child)->value;
	}
	return "";
}

/* Name: removePair
 * Params:
 *	int index - the index of the key/value pair to remove
 * Author: Joshua Campbell
 * Description:
 *	Removes the key/value pair from the node and shuffles the key value pairs to the right
 *	of that index.
 * Returns: None
 */
void LeafNode::removePair(int index) {
	if (this->numChildren > 0) {
		for (int i = index; i < this->numKeys - 1; i++) {
			this->keys[i] = this->keys[i + 1];
			this->children[i] = this->children[i + 1];
		}
		for (int i = numChildren; i < this->maxKeys; i++) {
			this->children[i] = 0;
		}
		this->numKeys -= 1;
		this->numChildren -= 1;
	}
}

/* Name: deletePair
 * Params:
 *	int index - the index of the key/value pair to remove and delete
 * Author: Joshua Campbell
 * Description:
 *	Removes and deletes (frees memory) the key/value pair from the node and shuffles the key 
 *	value pairs to the right of that index.
 * Returns: true if the key/value pair was deleted, false otherwise
 */
bool LeafNode::deletePair(int index) {
	if (index >= 0 && index < this->numKeys && this->numChildren > 0) {
		delete this->children[index];
		for (int i = index; i < this->numChildren - 1; i++) {
			this->children[i] = this->children[i + 1];
			this->keys[i] = this->keys[i + 1];
		}
		for (int i = numChildren; i < this->maxKeys; i++) {
			this->children[i] = 0;
			this->children[i] = 0;
		}
		this->numChildren -= 1;
		this->numKeys -= 1;
		return true;
	}
	else {
		return false;
	}
}

/* Name: getNumChildren
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the current number of children stored in the leaf.
 * Returns: the current number of children stored in the leaf
 */
int LeafNode::getNumChildren() {
	return this->numChildren;
}

/* Name: InteriorNode Constructor
 * Params:
 *	int maxKeys - the maximum number of keys the node can hold
 * Author: Joshua Campbell
 * Description:
 *	Creates a new interior node and allocates memory for maxKeys + 1 children.
 */
InteriorNode::InteriorNode(int maxKeys) : Node(maxKeys)
{
	this->type = NODE_TYPE_INTERIOR;
	this->children = new Node*[maxKeys + 1];
	for (int i = 0; i < maxKeys + 1; i++) {
		this->children[i] = 0;
	}
}

/* Name: findNextNode (InteriorNode)
 * Params:
 *	int key - the key used to find the next immediate child
 * Author: Joshua Campbell
 * Description:
 *	Finds the next immediate child of the interior node that matches the key provided.
 * Returns: returns the child node that matches the provided key
 */
Node* InteriorNode::findNextNode(int key) 
{
	if (this->numChildren > 0) {
		for (int i = 0; i < this->numKeys; i++) {
			if (key < keys[i] && i < this->numChildren) {
				return children[i];
			}
		}
		if (this->numChildren > this->numKeys) {
			return children[this->numKeys];
		} else if (this->numChildren > 0) {
			return children[0];
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

/* Name: addChild (InteriorNode)
 * Params:
 *	Node* child - the child that will be added to the node
 * Author: Joshua Campbell
 * Description:
 *	Adds a child to the rightmost empty child pointer of the node. No new keys are added to the node.
 *  If the child is a leaf node, its rightmost pointer is adjusted to point to the next node and the 
 *	node to the left of the new node is adjusted so that its rightmost pointer points at the new node.
 * Returns: true if the child was inserted, false otherwise
 */
bool InteriorNode::addChild(Node* child)
{
	if (this->isFull())
	{
		return false;
	}
	else
	{
		this->children[this->numChildren] = child;
		if (this->numChildren > 0 && this->children[this->numChildren - 1]->getNodeType() == NODE_TYPE_LEAF) {
			child->setChild(child->getMaxKeys(), this->children[this->numChildren - 1]->getChild(this->children[this->numChildren - 1]->getMaxKeys()));
			this->children[this->numChildren - 1]->setChild(this->children[this->numChildren - 1]->getMaxKeys(), child);
		}
		child->setParent(this);
		this->numChildren += 1;
		return true;
	}
}

/* Name: addChildToLeft
 * Params:
 *	Node* child - the child that will be added to the node
 * Author: Joshua Campbell
 * Description:
 *	Adds a child to the interior node at its leftmost position. No new keys are added to the node.
 * Returns: true if the child was inserted, false otherwise
 */
bool InteriorNode::addChildToLeft(Node* child)
{
	if (this->numChildren >= this->maxKeys + 1)
	{
		return false;
	}
	else
	{
		for (int i = this->numChildren - 1; i >= 0; i--) {
			this->children[i + 1] = this->children[i];
		}
		this->children[0] = child;
		child->setParent(this);
		this->numChildren += 1;
		return true;
	}
}

/* Name: addChild (InteriorNode)
 * Params:
 *	Node* child - the child that will be added to the node
 *	int lowestKeyValue - the lowest key value to add the child on
 * Author: Joshua Campbell
 * Description:
 *	Adds a child to the node using the specified lowestKeyValue as its identifier
 * Returns: true if the child was inserted, false otherwise
 */
bool InteriorNode::addChild(Node* child, int lowestKeyValue)
{
	if (this->isFull())
	{
		return false;
	}
	else
	{
		child->setParent(this);	
		if (this->numKeys == 0) {
			this->keys[0] = lowestKeyValue;
			int childKey = child->findIdentifierKey();
			
			if (this->numChildren > 0) {
				int existingChildKey = this->children[0]->findIdentifierKey();
				if (childKey < existingChildKey) {
					this->children[1] = this->children[0];
					this->children[0] = child;
				}
				else {
					this->children[1] = child;
				}
			}
			else {
				this->children[this->numChildren] = child;
			}
			this->numChildren += 1;
			this->numKeys += 1;
		}
		else
		{
			bool inserted = false;
			for (int i = 0; i < this->numKeys; i++) {
				if (lowestKeyValue < this->keys[i]) {
					if (this->getNumChildren() == this->getNumKeys()) {
						int idKey = child->findIdentifierKey();
						int index = 0;
						for (int k = 0; k < this->numKeys; k++) {
							if (idKey >= this->keys[k]) {
								index = k + 1;
								break;
							}
						}
						this->insertChild(child, index);
					}
					else {
						for (int k = this->numKeys - 1; k >= i && this->numKeys < this->maxKeys; k--) {
							this->keys[k + 1] = this->keys[k];
						}
						this->keys[i] = lowestKeyValue;
						int idKey = child->findIdentifierKey();
						if (idKey < lowestKeyValue) {
							this->insertChild(child, i);
						}
						else {
							this->insertChild(child, i + 1);
						}
						this->numKeys += 1;
					}
					
					/*if (i + 2 < this->numChildren && child->getNodeType() == NODE_TYPE_LEAF) {
						child->setChild(child->getMaxKeys(), this->children[i + 2]);
					}
					else if (i + 2 == this->maxKeys && i >= 0 && i <= this->children[i]->getMaxKeys() && child->getNodeType() == NODE_TYPE_LEAF) {
						child->setChild(child->getMaxKeys(), this->children[i]->getChild(this->children[i]->getMaxKeys()));
					}
					if (i >= 0 && i <= this->children[i]->getMaxKeys() && this->children[i]->getNodeType() == NODE_TYPE_LEAF) {
						this->children[i]->setChild(this->children[i]->getMaxKeys(), child);
					}*/
					
					inserted = true;
					return true;
				}
			}
			if (!inserted && this->numKeys < this->maxKeys) {
				this->keys[this->numKeys] = lowestKeyValue;
				this->insertChild(child, this->numKeys + 1);
				this->numKeys += 1;
			}
			else {
				return false;
			}
		}

		return true;
	}
}

/* Name: getMiddleKey
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Returns the middle/median key of the interior node's keys
 * Returns: the integer value of the middle/median key
 */
int InteriorNode::getMiddleKey()
{
	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2);
	}
	else
	{
		middleKey = (this->maxKeys / 2) - 1;
	}
	return this->keys[middleKey];
}

/* Name: getMiddleKey
 * Params:
 *	int newKey - the new key that will be added to the existing keys
 * Author: Joshua Campbell
 * Description:
 *	Returns the value of the middle key after temporarily adding the new key to the interior
 *  node's keys.
 * Returns: returns the middle keys value (using the newKey)
 */
int InteriorNode::getMiddleKey(int newKey)
{
	int middleKey = -1;
	std::vector<int> tempkeys;
	bool inserted = false;
	for (int i = 0; i < this->maxKeys; i++) {
		if (!inserted && newKey < this->keys[i]) {
			tempkeys.push_back(newKey);
			tempkeys.push_back(this->keys[i]);
			inserted = true;
		}
		else {
			tempkeys.push_back(this->keys[i]);
		}
	}
	if (!inserted) {
		tempkeys.push_back(newKey);
	}
	if ((this->maxKeys + 1) % 2 == 1)
	{
		middleKey = ((this->maxKeys + 1) / 2);
	}
	else
	{
		middleKey = ((this->maxKeys + 1) / 2);
	}
	return tempkeys[middleKey];
}

/* Name: getKeyIndex (InteriorNode)
 * Params:
 *	int key - the specified key that needs to be found
 * Author: Joshua Campbell
 * Description:
 *	Searches for the index of the key that would point to the key specified
 * Returns: the index for the key that would point to the specified key (leaf) in the tree
 */
int InteriorNode::getKeyIndex(int key)
{
	if (this != 0) {
		int lowestIndex = 0;
		for (int i = 0; i < this->numKeys; i++) {
			if (key >= keys[i]) {
				lowestIndex = i;
			}
		}
		if (lowestIndex == -1) {
			return this->numKeys - 1;
		}
		else {
			return lowestIndex;
		}
	}
	else {
		return -1;
	}
}

/* Name: insertChild (InteriorNode)
 * Params:
 *	Node* child - the child node to be inserted
 *	int index - the index to insert the child into
 * Author: Joshua Campbell
 * Description:
 *	Inserts the specified child into the specified index of the interior node. Returns false if the
 *	node is full.
 * Returns: true if the child was inserted, false otherwise
 */
bool InteriorNode::insertChild(Node* child, int index)
{
	if (this->isFull())
	{
		return false;
	}
	else
	{
		for (int i = numChildren - 1; i >= index && this->numChildren <= this->maxKeys; i--) {
			this->children[i + 1] = this->children[i];
		}
		this->children[index] = child;
		/*if (index == numChildren - 1 && index >= 1 && child->getNodeType() == NODE_TYPE_LEAF) {
			child->setChild(this->maxKeys, this->children[index - 1]->getChild(this->maxKeys));
		} else if (index + 1 <= numChildren && child->getNodeType() == NODE_TYPE_LEAF) {
			child->setChild(this->maxKeys, this->children[index + 1]);
		}*/
		//std::cout << "inserting" << std::endl;
		//this->printChildren();
		/*if (index >= 1 && index <= this->maxKeys && this->children[index - 1]->getNodeType() == NODE_TYPE_LEAF) {
			this->children[index - 1]->setChild(this->maxKeys, child);
		}*/
		//this->printChildren();
		this->numChildren += 1;
		return true;
	}
}

/* Name: split (InteriorNode)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Splits the current interior node in two.
 * Returns: pointers to the current node and the new split node
 */
Node** InteriorNode::split()
{
	Node** newNodes = new Node*[2];
	newNodes[0] = this;
	newNodes[1] = new InteriorNode(this->maxKeys);
	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2);
	}
	else
	{
		middleKey = (this->maxKeys / 2) - 1;
	}
	for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
		static_cast<InteriorNode*>(newNodes[1])->addChild(newNodes[0]->getChild(i));
	}
	int childrenRemoved = 0;
	for (int i = middleKey; i < newNodes[0]->getMaxKeys(); i++) {
		newNodes[0]->removeChild(i);
		childrenRemoved += 1;
	}
	this->numKeys -= childrenRemoved;

	return newNodes;
}

/* Name: split (InteriorNode)
 * Params:
 *	Node * newChild - the new child that is being added to the node as it is being split
 * Author: Joshua Campbell
 * Description:
 *	Splits the current interior node in two while taking into account which node the new child will be added to.
 *  This affects the number of nodes that will be removed from the original node so that the split node and it
 *  will have a balanced number of children. Uses the leftmost key of the newChild as the key to identify the 
 *  newChild node.
 * Returns: pointers to the current node and the new split node
 */
Node** InteriorNode::split(Node * newChild)
{
	Node** newNodes = new Node*[2];
	newNodes[0] = this;
	newNodes[1] = new InteriorNode(this->maxKeys);
	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2) + 1;
	}
	else
	{
		middleKey = (this->maxKeys / 2);
	}

	int newKey = newChild->getKey(0);

	if (newKey < (static_cast<InteriorNode*>(newNodes[0])->getMiddleKey(newKey)))
	{
		for (int i = middleKey; i <= newNodes[0]->getMaxKeys(); i++) {
			Node * child = newNodes[0]->getChild(i);
			if (i == middleKey && middleKey <= newNodes[0]->getMaxKeys() - 1) {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child);
			}
			else {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child, newNodes[0]->getKey(i - 1));
			}
		}
		int childrenRemoved = 0;
		for (int i = middleKey; i <= newNodes[0]->getMaxKeys(); i++) {
			newNodes[0]->removeChild(middleKey);
			childrenRemoved += 1;
		}
		this->numKeys -= childrenRemoved;
		static_cast<InteriorNode*>(newNodes[0])->addChild(newChild, newKey);
	}
	else
	{
		int initialKey = newNodes[0]->getKey(middleKey);
		if (initialKey > newKey) {
			static_cast<InteriorNode*>(newNodes[1])->addChild(newChild);
		}
		for (int i = middleKey + 1; i <= newNodes[0]->getMaxKeys(); i++) {
			Node * child = newNodes[0]->getChild(i);
			if (i == middleKey + 1 && middleKey + 1 <= newNodes[0]->getMaxKeys() && initialKey < newKey) {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child);
			}
			else {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child, newNodes[0]->getKey(i - 1));
			}
		}
		int childrenRemoved = 0;
		for (int i = middleKey + 1; i <= newNodes[0]->getMaxKeys(); i++) {
			newNodes[0]->removeChild(middleKey + 1);
			childrenRemoved += 1;
		}
		this->numKeys -= childrenRemoved;
		
		if (initialKey < newKey) {
			static_cast<InteriorNode*>(newNodes[1])->addChild(newChild, newKey);
		}
	}

	return newNodes;
}

/* Name: split (InteriorNode)
 * Params:
 *	Node * newChild - the new child that is being added to the node as it is being split
 *	int key - the key that the new child will be listed under after the split
 * Author: Joshua Campbell
 * Description:
 *	Splits the current interior node in two while taking into account which node the new child will be added to.
 *  This affects the number of nodes that will be removed from the original node so that the split node and it
 *  will have a balanced number of children. Uses the specified key as the identifier for the newChild.
 * Returns: pointers to the current node and the new split node
 */
Node** InteriorNode::split(Node * newChild, int key)
{
	Node** newNodes = new Node*[2];
	newNodes[0] = this;
	newNodes[1] = new InteriorNode(this->maxKeys);

	int middleKey = -1;
	if (this->maxKeys % 2 == 1)
	{
		middleKey = (this->maxKeys / 2) + 1;
	}
	else
	{
		middleKey = (this->maxKeys / 2);
	}
	
	if (key < (static_cast<InteriorNode*>(newNodes[0])->getMiddleKey(key)))
	{
		for (int i = middleKey; i <= newNodes[0]->getMaxKeys(); i++) {
			Node * child = newNodes[0]->getChild(i);
			
			if (i == middleKey && middleKey <= newNodes[0]->getMaxKeys() - 1) {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child);
			}
			else {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child, newNodes[0]->getKey(i - 1));
			}
		}
		int childrenRemoved = 0;
		for (int i = middleKey; i <= newNodes[0]->getMaxKeys(); i++) {
			newNodes[0]->removeChild(middleKey);
			
			childrenRemoved += 1;
		}
		this->numKeys -= childrenRemoved;

		static_cast<InteriorNode*>(newNodes[0])->addChild(newChild, key);
	}
	else
	{
		int initialKey = newNodes[0]->getKey(middleKey);
		if (initialKey > key) {
			static_cast<InteriorNode*>(newNodes[1])->addChild(newChild);
		}
		for (int i = middleKey + 1; i <= newNodes[0]->getMaxKeys(); i++) {
			Node * child = newNodes[0]->getChild(i);
			if (i == middleKey + 1 && middleKey + 1 <= newNodes[0]->getMaxKeys() && initialKey < key) {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child);
			}
			else {
				static_cast<InteriorNode*>(newNodes[1])->addChild(child, newNodes[0]->getKey(i - 1));
			}
		}
		int childrenRemoved = 0;
		for (int i = middleKey + 1; i <= newNodes[0]->getMaxKeys(); i++) {
			newNodes[0]->removeChild(middleKey + 1);
			childrenRemoved += 1;
		}
		this->numKeys -= childrenRemoved;
		if (initialKey < key) {
			static_cast<InteriorNode*>(newNodes[1])->addChild(newChild, key);
		}
	}

	return newNodes;
}

/* Name: leastChildValue
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Finds and returns the leftmost key of the interior node's leftmost child.
 * Returns: the leftmost key of the interior node's leftmost child
 */
int InteriorNode::leastChildValue() {
	return children[0]->getKey(0);
}

/* Name: findLeastLeafKey (InteriorNode)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Finds the leftmost key of the interior node's leftmost leaf child. Used for updating
 *  the keys of interior nodes so that they will eventually lead to their leftmost child key.
 * Returns: the leftmost key of the interior node's leftmost leaf child.
 */
int InteriorNode::findLeastLeafKey() {
	if (this->numChildren > 0) {
		Node* current = this->getChild(0);
		while (current->getNumChildren() > 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
			current = current->getChild(0);
		}
		if (current->getNodeType() == NODE_TYPE_LEAF) {
			return current->getKey(0);
		}
	}
	return -1;
}

/* Name: printChildren
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Prints out the keys of the current node, the keys of its children, and the keys of their children
 *  and so on. Used for debugging.
 * Returns: None
 */
void Node::printChildren() {
	Node * current = this;
	std::vector<Node*> queue;
	std::cout << "PRINTING NODE CHILDREN:" << std::endl;
	std::cout << "Parent Node: ";
	current->printKeys();
	while (current != 0 && current->getNodeType() == NODE_TYPE_INTERIOR) {
		std::cout << "node: ";
		current->printKeys();
		for (int k = 0; k < current->getNumChildren(); k++) {
			Node* child = current->getChild(k);
			child->printKeys();
			queue.push_back(child);
		}
		std::cout << std::endl << std::endl;
		if (queue.size() > 0) {
			current = queue.at(0);
			queue.erase(queue.begin());
		}
		else {
			current = 0;
		}
	}
}

/* Name: findNeighbours (InteriorNode)
 * Params:
 *	None
 * Author: Joshua Campbell
 * Description:
 *	Finds any neighbours if they exist for the current node by looking at the children of its parent.
 *  Neighbours are the nodes that are to the right of the node or the left of the node (or itself in
 *  situations where the node is the only child of the parent).
 * Returns: A pointer (Node**) to two pointers for the neighbour nodes
 */
Node** InteriorNode::findNeighbours() {
	bool found = false;
	if (this->parent != 0) {
		Node** neighbours = new Node*[2];
		neighbours[0] = 0;
		neighbours[1] = 0;
		Node* parent = this->parent;
		int identifier = this->keys[0];
		for (int i = 0; i < parent->getNumKeys(); i++) {
			int currentKey = parent->getKey(i);
			if (identifier >= currentKey) {
				if (i + 1 > 0) {
					neighbours[0] = parent->getChild(i);
					found = true;
				}
				if (i + 2 < parent->getNumChildren()) {
					neighbours[1] = parent->getChild(i + 2);
					found = true;
				}
			}
		}
		if (parent->getNumKeys() >= 1 && !found) {
			neighbours[1] = parent->getChild(1);
		}
		return neighbours;
	}
	else {
		return 0;
	}
}

/* Name: DataNode Constructor
 * Params: 
 *	std::string value - the string value that will be stored in the node
 * Author: Joshua Campbell
 * Description:
 *	Creates a node that is used for storing string values.
 */
DataNode::DataNode(std::string value) {
	this->type = NODE_TYPE_DATA;
	this->value = value;
}