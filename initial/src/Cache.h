#ifndef CACHE_H
#define CACHE_H

#include "main.h"
//////////////////////////////// 2 Abstract Class RepalcementPolicy and Search ///////////////////////////
// ReplacementPolicy
class ReplacementPolicy {
protected:
	int count;

public:
	virtual ~ReplacementPolicy() = default;
	virtual void insert(Elem* ele) = 0;
	virtual void access(int addr) = 0;
	virtual Elem* remove() = 0;
	virtual void print() = 0;

	bool isFull() {
		return count == MAXSIZE;
	}
};

// SearchEngine
class SearchEngine {
public:
	virtual ~SearchEngine() = default;
	virtual Elem* search(int addr) = 0;
	virtual void insert(Elem* ele) = 0;
	virtual	void removeNode(int addr) = 0;
	virtual void print() = 0;

	virtual bool hashcheck() = 0;
};


//////////////////////////////////// 4 Replacement policy classes ////////////////////////////////
/////////////////////////////////// 1.FIFO
class FIFO : public ReplacementPolicy {
	int maxsize;
	int front;
	int rear;
	Elem** arr;

public:
	FIFO() {
		count = 0;
		maxsize = MAXSIZE + 1;
		rear = 0;
		front = 1;
		arr = new Elem * [maxsize];
	}

	~FIFO() {
		delete[]arr;
	}
	void insert(Elem* ele);
	void access(int addr);
	Elem* remove();
	void print();
};
////////////////////////// 2.MRU and 3.LRU ////////////////////////////
class listNode {
	Elem* element;
	listNode* next;

	friend class MRU;
	friend class LRU;
public:
	

	listNode(Elem* ele, listNode* nxt = NULL) {
		element = ele;
		next = nxt;
	}
};

// MRU
class MRU : public ReplacementPolicy {
protected:
	listNode* head;
	listNode* tail;

public:
	MRU() {
		head = NULL;
		tail = NULL;
		count = 0;
	}
	~MRU() {
		while (head) {
			listNode* temp = head;
			head = head->next;
			delete temp;
		}
	}
	void insert(Elem* ele);
	void access(int addr);
	Elem* remove();
	void print();
};

// LRU
class LRU : public MRU {
public:
	Elem* remove() override;
};
///////////////////////////////////// 4.LFU ///////////////////////////////////
// heapNode
class heapNode {
	Elem* element;
	int frequency;

	friend class LFU;
public:
	
	friend bool operator< (const heapNode& left_hs, const heapNode& right_hs);

	heapNode(Elem* ele = NULL, int freq = 1) : element(ele), frequency(freq) {}
	heapNode& operator=(const heapNode& other);
};
bool operator< (const heapNode& left_hs, const heapNode& right_hs);

// LFU
class LFU : public ReplacementPolicy {
	heapNode* arr;

	// Methods 
	bool isLeaf(int pos);
	
	void swap(int x, int y);
	void heapDown(int pos);
	void heapUp(int pos);

public:
	LFU() {
		arr = new heapNode[MAXSIZE];
		count = 0;
	}
	~LFU() {
		delete[]arr;
	}
	void insert(Elem* ele);
	void access(int addr);
	Elem* remove();
	void print();
};


//////////////////////////////////////////// Searching(2 Parts) ////////////////////////////////
// 1.DBHashing
class DBHashing : public SearchEngine {
	int (*h1)(int);
	int (*h2)(int);
	int size;
	Elem** arr;
	
public:
	DBHashing(int (*h1)(int), int (*h2)(int), int s) : h1(h1), h2(h2), size(s) {
		arr = new Elem * [size];
		for (int i = 0; i < size; i++)
			arr[i] = NULL;
	}
	~DBHashing() {
		for (int i = 0; i < size; i++) {
			if (arr[i])
				delete arr[i];
		}
		delete[]arr;
	}
	Elem* search(int addr);
	void insert(Elem* ele);
	void removeNode(int addr);
	void print();

	bool hashcheck() { return true; };
};
/////////////////////////////////// 2. AVL tree ///////////////////////
// avlNode
class avlNode {
	Elem* element;
	avlNode* left;
	avlNode* right;
	int height;

	friend class AVL;
public:
	
	// Methods
	avlNode() : element(NULL), left(NULL), right(NULL), height(0) {}
	avlNode(Elem* val) : element(val), left(NULL), right(NULL), height(1) {}
	int getAddr() const;
	void copy(avlNode* other);
};

// AVL
class AVL : public SearchEngine {
	avlNode* root;

	// Implementations
	int height(avlNode* root);
	int getBalance(avlNode* root);
	void preOrder(avlNode* root);
	void inOrder(avlNode* root);
	void freeTree(avlNode* root);

	avlNode* rightRotate(avlNode* root);
	avlNode* leftRotate(avlNode* root);
	avlNode* getMinNode(avlNode* root);
	avlNode* insert(avlNode* root, Elem* ele);
	avlNode* remove(avlNode* root, int addr);
	avlNode* find(avlNode* root, int addr);
	
public:
	AVL() : root(NULL) {}
	~AVL() {
		freeTree(root);
	}

	Elem* search(int addr);
	void insert(Elem* ele);
	void removeNode(int addr);
	void print();

	bool hashcheck() { return false; }
};
#endif