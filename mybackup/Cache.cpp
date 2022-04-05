#include "Cache.h"

heapNode& heapNode::operator=(const heapNode& other)
{
	element = other.element;
	frequency = other.frequency;

	return *this;
}

bool operator<(const heapNode& lhs, const heapNode& rhs)
{
	return lhs.frequency < rhs.frequency;
}

// AVL Node
int maximum(int a, int b) { return(a > b ? a : b); }

int avlNode::getAddr() const { return val->addr; }

Elem* avlNode::getElem() const { return val; }

void avlNode::setElem(Elem* element) { val = element; }

void avlNode::copy(avlNode* temp) {
	val = temp->val;
	left = temp->left;
	right = temp->right;
	height = temp->height;
}

void avlNode::removeElem() {
	free(val);
}

// FIFO
void FIFO::insert(Elem* ele)
{
	rear = (rear + 1) % maxsize;
	arr[rear] = ele;
	count++;
}

void FIFO::access(int addr) {}

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
		changedNode = changedNode->next;
		prevNode = changedNode;
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
		listNode* preNode = head;
		ele = tail->element;
		while (preNode->next != tail) {
			preNode = preNode->next;
		}
		delete tail;
		tail = preNode;
		preNode->next = NULL;
	}
	count--;
	return ele;
}

// LFU
bool LFU::isLeaf(int pos)
{
	return (pos >= count / 2) && (pos < count);
}

int LFU::leftChild(int pos)
{
	return 2 * pos + 1;
}

int LFU::rightChild(int pos)
{
	return 2 * pos + 2;
}

int LFU::parent(int pos)
{
	return (pos - 1) / 2;
}

void LFU::swap(int a, int b)
{
	heapNode temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

void LFU::heapDown(int pos)
{
	while (!isLeaf(pos)) {
		int i = leftChild(pos);
		int rc = rightChild(pos);
		if (rc < count && arr[rc] < arr[i])
			i = rc;
		if (arr[pos] < arr[i]) return;
		swap(pos, i);
		pos = i;
	}
}

void LFU::heapUp(int pos)
{
	while (pos > 0) {
		if (arr[pos] < arr[parent(pos)]) {
			swap(pos, parent(pos));
			pos = parent(pos);
		}
		else return;
	}
}

void LFU::insert(Elem* e)
{
	arr[count++] = heapNode(e);
	heapUp(count - 1);
}

void LFU::access(int adr)
{
	for (int i = 0; i < count; i++) {
		if (arr[i].element->addr == adr) {
			arr[i].frequency++;
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
	for (int i = 0; i < count; i++) {
		arr[i].element->print();
	}
}
////////////////////////////////////////////////////////////
// DBHashing
void DBHashing::insert(Elem* ele)
{
	int i = 0;
	int hash1 = h1(ele->addr);
	int hash2 = h2(ele->addr);
	while (arr[(hash1 + i * hash2) % size])
		i++;
	arr[(hash1 + i * hash2) % size] = ele;
}

void DBHashing::removeNode(int addr)
{
	int i = 0;
	int hash1 = h1(addr);
	int hash2 = h2(addr);
	while (arr[(hash1 + i * hash2) % size]->addr != addr)
		i++;
	arr[(hash1 + i * hash2) % size] = NULL;
}

Elem* DBHashing::search(int addr)
{
	int i = 0;
	int hash1 = h1(addr);
	int hash2 = h2(addr);
	while (arr[(hash1 + i * hash2) % size] && arr[(hash1 + i * hash2) % size]->addr != addr)
		i++;
	return arr[(hash1 + i * hash2) % size];
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
int AVL::height(avlNode* root) const {
	if (!root)return 0;
	else return root->height;
}

avlNode* AVL::find(avlNode* root, int addr) const {
	if (!root || (addr == root->getAddr()))
		return root;
	else if (addr < root->getAddr())
		return find(root->left, addr);
	else return find(root->right, addr);
}

void  AVL::preOrder(avlNode* root) {
	if (root) {
		root->getElem()->print();
		preOrder(root->left);
		preOrder(root->right);
	}
}
void  AVL::inOrder(avlNode* root) {
	if (root) {
		inOrder(root->left);
		root->getElem()->print();
		inOrder(root->right);
	}
}
void  AVL::freetree(avlNode* root) {
	if (!root)return;
	freetree(root->left);
	freetree(root->right);
	root->removeElem();
	delete root;
}

// keypoint functions for AVL 
avlNode* AVL::rightRotate(avlNode* x) {
	avlNode* y = x->left;
	avlNode* T2 = y->right;

	// Perform rotation
	y->right = x;
	x->left = T2;


	// Update heights
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}

avlNode* AVL::leftRotate(avlNode* y) {
	avlNode* x = y->right;
	avlNode* T2 = x->left;

	// Perform rotation
	x->left = y;
	y->right = T2;


	// Update heights
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}

int AVL::getBalance(avlNode* root)
{
	if (!root)
		return 0;
	return height(root->left) - height(root->right);
}

avlNode* AVL::minNode(avlNode* root) {
	while (root->right)
		root = root->right;
	return root;
}


avlNode* AVL::insert(avlNode* root, Elem* element) {
	int addr = element->addr;

	// BST insertion
	if (!root)
		return new avlNode(element);

	if (addr < root->getAddr())
		root->left = insert(root->left, element);
	else if (addr > root->getAddr())
		root->right = insert(root->right, element);
	else return root; // Equal keys are not allowed in BST

	// Update height for ancestor root
	root->height = 1 + max(height(root->left), height(root->right));

	// Check for balance
	int balance = getBalance(root);

	// Left Right 
	if (balance > 1 && addr > root->left->getAddr()) {
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Left 
	if (balance < -1 && addr < root->right->getAddr()) {
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	// Left Left 
	if (balance > 1 && addr < root->left->getAddr())
		return rightRotate(root);

	// Right Right 
	if (balance < -1 && addr > root->right->getAddr())
		return leftRotate(root);

	// Return the unchanged root
	return root;
}
avlNode* AVL::remove(avlNode* root, int addr) {

	// BST deletion
	if (!root) return root;

	// Tree traversal
	if (addr < root->getAddr())root->left = remove(root->left, addr);
	else if (addr > root->getAddr())
		root->right = remove(root->right, addr);
	else {
		// If root has 1 or 0 child
		if (!root->left || !root->right) {
			avlNode* tmp = root->left ? root->left : root->right;

			// 0 child
			if (!tmp) {
				delete root;
				root = NULL;
			}
			// 1 child
			else {
				root->copy(tmp);
				delete tmp;
			}
		}
		// has two children
		else {
			// Get min of right
			avlNode* tmp = minNode(root->right);

			// Copy value of max of left to node
			root->setElem(tmp->getElem());

			// Delete the max of left node
			root->left = remove(root->left, tmp->getAddr());
		}
	}

	// Go back up
	if (!root)
		return root;

	// After deletion, update height -> going back
	root->height = 1 + max(height(root->left), height(root->right));

	// Check for balance
	int balance = getBalance(root);

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
	// Left Left Case
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	// Return the unchanged root
	return root;
}

//AVL's public part
void AVL::insert(Elem* element) { root = insert(root, element); }

void AVL::removeNode(int addr) { root = remove(root, addr); }

Elem* AVL::search(int addr) {
	avlNode* temp = find(root, addr);

	if (temp != NULL)return temp->val;
	else return NULL;
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
	return temp ? temp->data : NULL;
}
Elem* Cache::put(int addr, Data* cont) {
	Elem* element = new Elem(addr, cont, true);
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
