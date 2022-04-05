#include "Cache.h"

// **** heapNode**********************

heapNode& heapNode::operator=(const heapNode& other)
{
	if (this != &other) {
		element = other.element;
		frequency = other.frequency;
	}
	return *this;
}

bool operator<(const heapNode& left_hs, const heapNode& right_hs){return left_hs.frequency < right_hs.frequency;}


// AVL Node
int maximum(int a, int b) { return(a > b ? a : b); }

int avlNode::getAddr() const { return element->addr; }

void avlNode::copy(avlNode* temp) {
	element = temp->element;
	left = temp->left;
	right = temp->right;
	height = temp->height;
}


// FIFO 
void FIFO::insert(Elem* ele)
{
	rear = (rear + 1) % maxsize;
	arr[rear] = ele;
	count++;
}

void FIFO::access(int addr) {} // don't use

Elem* FIFO::remove()
{
	Elem* ele = arr[front];
	front = (front + 1) % maxsize;
	count--;

	return ele;
}

void FIFO::print()
{
	for (int i = 0; i < count; i++) arr[(front + i) % maxsize]->print();
}


// MRU
void MRU::insert(Elem* ele)
{
	head = new listNode(ele, head);
	if (count == 0)tail = head;
	count++;
}

void MRU::access(int addr)
{
	if (head->element->addr == addr) return;
	listNode* prevNode = head;
	listNode* changedNode = head->next;
	while (changedNode->element->addr != addr) {
		prevNode = changedNode;
		changedNode = changedNode->next;
	}
	prevNode->next = changedNode->next;
	changedNode->next = head;
	head = changedNode;
	// case swap tail
	if (changedNode == tail) tail = prevNode;
}

Elem* MRU::remove()
{
	listNode* tmp = head;
	Elem* ele = tmp->element;
	head = head->next;
	if (count == 1) head = tail = NULL;
	count--;
	delete tmp;

	return ele;
}

void MRU::print()
{
	listNode* tmp = head;
	while (tmp) {
		tmp->element->print();
		tmp = tmp->next;
	}
}


// LRU
Elem* LRU::remove()
{
	Elem* ele = NULL;
	if (count == 1) {
		ele = head->element;
		delete head;
		head = tail = NULL;
	}
	else {
		listNode* prevNode = head;
		ele = tail->element;
		while (prevNode->next != tail) {
			prevNode = prevNode->next;
		}
		delete tail;
		tail = prevNode;
		prevNode->next = NULL;
	}
	count--;
	return ele;
}

bool LFU::isLeaf(int pos) { return (pos >= count / 2) && (pos < count); }

void LFU::swap(int x, int y)
{
	heapNode tmp = arr[x];
	arr[x] = arr[y];
	arr[y] = tmp;
}

void LFU::heapDown(int pos)
{
	while (!isLeaf(pos)) {
		int lChild = 2 * pos + 1;
		int rChild = 2 * pos + 2;
		if (rChild < count && arr[rChild] < arr[lChild])
			lChild = rChild;
		if (arr[pos] < arr[lChild]) return;
		swap(pos, lChild);
		pos = lChild;
	}
}

void LFU::heapUp(int pos)
{
	while (pos > 0) {
		int parent = (pos - 1) / 2;
		if (arr[pos] < arr[parent]) {
			swap(pos, parent);
			pos = parent;
		}
		else return;
	}
}

void LFU::insert(Elem* e)
{
	arr[count] = heapNode(e);
	count++;
	heapUp(count - 1);
}

void LFU::access(int addr)
{
	for (int i = 0; i < count; i++) {
		if (arr[i].element->addr == addr) {
			arr[i].frequency= arr[i].frequency+1;
			heapDown(i);
			return;
		}
	}
}

Elem* LFU::remove()
{
	swap(0, count - 1);
	count--;
	heapDown(0);
	return arr[count].element;
}

void LFU::print()
{
	for (int i = 0; i < count; i++) arr[i].element->print();
}

////////////////////////////////////////////////////////////
// DBHashing
void DBHashing::insert(Elem* ele)
{
	int i = 0;
	int hash1 = h1(ele->addr);
	int hash2 = h2(ele->addr);
	while ((i <= size) && arr[(hash1 + i * hash2) % size]) {
		i++;
	}
	if (i > size)return;
	arr[(hash1 + i * hash2) % size] = ele;
}

void DBHashing::removeNode(int addr)
{
	int i = 0;
	int hash1 = h1(addr);
	int hash2 = h2(addr);
	while ((i <= size) && arr[(hash1 + i * hash2) % size] && arr[(hash1 + i * hash2) % size]->addr != addr)i++;
	if (i > size)return;
	arr[(hash1 + i * hash2) % size] = NULL;
}

Elem* DBHashing::search(int addr)
{
	int i = 0;
	int hash1 = h1(addr);
	int hash2 = h2(addr);
	while ((i < size) && arr[(hash1 + i * hash2) % size] && arr[(hash1 + i * hash2) % size]->addr != addr) {
		i++;
	}
	if (arr[(hash1 + i * hash2) % size] ==NULL || arr[(hash1 + i * hash2) % size]->addr != addr)return NULL;
	else return arr[(hash1 + i * hash2) % size];
}
void DBHashing::print()
{
	cout << "Prime memory:\n";
	for (int i = 0; i < size; i++) {
		if (arr[i])
			arr[i]->print();
	}
}


// AVL tree
// avl private part
int AVL::height(avlNode* root)
{
	if (!root)
		return 0;
	return root->height;
}

avlNode* AVL::find(avlNode* root, int key)
{
	if (!root || key == root->getAddr())
		return root;
	else if (key < root->getAddr())
		return find(root->left, key);
	else
		return find(root->right, key);
}

void AVL::preOrder(avlNode* root)
{
	if (root) {
		root->element->print();
		preOrder(root->left);
		preOrder(root->right);
	}
}

void AVL::inOrder(avlNode* root)
{
	if (root) {
		inOrder(root->left);
		root->element->print();
		inOrder(root->right);
	}
}

void AVL::freeTree(avlNode* root)
{
	if (!root) return;
	freeTree(root->left);
	freeTree(root->right);
	delete root->element;
	delete root;
}

avlNode* AVL::rightRotate(avlNode* root)
{
	avlNode* leftNode = root->left;
	avlNode* rightOfLeft = leftNode->right;

	// Perform rotation
	leftNode->right = root;
	root->left = rightOfLeft;


	// Update heights
	root->height = max(height(root->left), height(root->right)) + 1;
	leftNode->height = max(height(leftNode->left), height(leftNode->right)) + 1;

	return leftNode;
}

avlNode* AVL::leftRotate(avlNode* root)
{
	avlNode* rightNode = root->right;
	avlNode* leftOfRight = rightNode->left;

	// Perform rotation
	rightNode->left = root;
	root->right = leftOfRight;


	// Update heights
	root->height = max(height(root->left), height(root->right)) + 1;
	rightNode->height = max(height(rightNode->left), height(rightNode->right)) + 1;

	return rightNode;
}

int AVL::getBalance(avlNode* root)
{
	if (!root)
		return 0;
	return height(root->left) - height(root->right);
}

avlNode* AVL::getMinNode(avlNode* root)
{
	avlNode* current = root;
	while (current->left)
		current = current->left;
	return current;
}

avlNode* AVL::insert(avlNode* root, Elem* ele)
{
	int key = ele->addr;

	// Perform the normal BST insertion
	if (!root)
		return new avlNode(ele);

	if (key < root->getAddr())
		root->left = insert(root->left, ele);
	else if (key > root->getAddr())
		root->right = insert(root->right, ele);
	else // Equal keys are not allowed in BST
		return root;

	// Update height of this ancestor root
	root->height = 1 + max(height(root->left), height(root->right));

	// Check for balance
	int balance = getBalance(root);

	// 4 cases

	// Left Left Case
	if (balance > 1 && key < root->left->getAddr())
		return rightRotate(root);

	// Right Right Case
	if (balance < -1 && key > root->right->getAddr())
		return leftRotate(root);

	// Left Right Case
	if (balance > 1 && key > root->left->getAddr()) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left Case
	if (balance < -1 && key < root->right->getAddr()) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	// Return the unchanged root pointer
	return root;
}

avlNode* AVL::remove(avlNode* root, int key)
{
	// Perform the normal BST deletion
	if (!root)
		return root;

	// Perform traversal
	if (key < root->getAddr())
		root->left = remove(root->left, key);
	else if (key > root->getAddr())
		root->right = remove(root->right, key);
	else { // Node to delete
		// If root has one or no child
		if (!root->left || !root->right) {
			avlNode* temp = root->left ? root->left : root->right;

			// No child
			if (!temp) {
				delete root;
				root = NULL;
			}
			// One child
			else {
				root->copy(temp);
				delete temp;
			}
		}
		// If root has two children
		else {
			// Get min of right
			avlNode* temp = getMinNode(root->right);

			// Copy value of min of right to root
			root->element = temp->element;

			// Delete the min of right root
			root->right = remove(root->right, temp->getAddr());
		}
	}

	// Go back up
	if (!root)
		return root;

	// After BST deletion, update height when going back
	root->height = 1 + max(height(root->left), height(root->right));

	// Check for balance
	int balance = getBalance(root);

	// 4 cases

	// Left Left Case
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	// Left Right Case
	if (balance > 1 && getBalance(root->left) < 0) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left Case
	if (balance < -1 && getBalance(root->right) > 0) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	// Return the unchanged root pointer
	return root;
}


//////////////////////////////////////////////
void AVL::insert(Elem* ele)
{
	root = insert(root, ele);
}

void AVL::removeNode(int key)
{
	root = remove(root, key);
}

Elem* AVL::search(int key)
{
	avlNode* result = find(root, key);
	return result ? result->element : NULL;
}

void AVL::print()
{
	cout << "Print AVL in inorder:\n";
	inOrder(root);
	cout << "Print AVL in preorder:\n";
	preOrder(root);
}

// den day ne/////////////////////////////////////////////////////////////////////////////
Cache::Cache(SearchEngine* s, ReplacementPolicy* r) {
	rp = r;
	s_engine = s;
}
Cache::~Cache() {
	delete rp;
	delete s_engine;
}
Data* Cache::read(int addr) {
	Elem* temp = s_engine->search(addr);
	if (temp)rp->access(temp->addr);
	if (temp) {
		return temp->data;
	}else {
		return NULL;
	}
}
Elem* Cache::put(int addr, Data* cont) {
	Elem* element = new Elem(addr, cont, true);
	Elem* removeElem = NULL;

	// If cache is full
	if (rp->isFull() == true) {
		// Remove the element of out the cache
		removeElem = rp->remove();
		s_engine->removeNode(removeElem->addr);
	}

	// Add new element
	rp->insert(element);
	s_engine->insert(element);

	return removeElem;
}
Elem* Cache::write(int addr, Data* cont) {
	// Search Elem has addr
	Elem* temp = s_engine->search(addr);

	// If addr in the cache
	if (temp) {
		// This should be commented when submitting
		//delete result->data;
		temp->data = cont;
		temp->sync = false;
		rp->access(temp->addr);
		return NULL;
	}
	// addr not in the cache
	else {
		Elem* element = new Elem(addr, cont, false);
		Elem* removeElem = NULL;

		// If cache is full
		if (rp->isFull()) {
			// Remove the element of out the cache
			removeElem = rp->remove();
			s_engine->removeNode(removeElem->addr);
		}

		// Add new element
		rp->insert(element);
		s_engine->insert(element);

		return removeElem;
	}
}
void Cache::printRP() {
	rp->print();
}
void Cache::printSE() {
	s_engine->print();
}