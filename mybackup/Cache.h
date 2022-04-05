#ifndef CACHE_H
#define CACHE_H

#include "main.h"


class ListNode {
	Elem* element;
	ListNode* next;

public:
	friend class MRU;
	friend class LRU;

	ListNode(Elem* e, ListNode* nxt = NULL) : element(e), next(nxt) {}
};

// HeapNode
class HeapNode {
	Elem* element;
	int frequency;

public:
	friend class LFU;
	friend bool operator< (const HeapNode& lhs, const HeapNode& rhs);

	HeapNode(Elem* e = NULL, int freq = 1) : element(e), frequency(freq) {}
	HeapNode& operator=(const HeapNode& other);
};
bool operator< (const HeapNode& lhs, const HeapNode& rhs);

// AVLNode
class AVLNode {
	Elem* element;
	AVLNode* left;
	AVLNode* right;
	int height;

public:
	friend class AVL;
	// Methods
	AVLNode() : element(NULL), left(NULL), right(NULL), height(0) {}
	AVLNode(Elem* val) : element(val), left(NULL), right(NULL), height(1) {}
	int getKey();
	void copy(AVLNode* other);
};



// ReplacementPolicy
class ReplacementPolicy {
protected:
	int count;

public:
	virtual ~ReplacementPolicy() = default;
	virtual void insert(Elem* e) = 0;
	virtual void access(int adr) = 0;
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
	virtual Elem* search(int key) = 0;
	virtual void insert(Elem* ele) = 0;
	virtual	void deleteNode(int key) = 0;
	virtual void print() = 0;
};


/// <summary>
/// RP concrete classes
/// </summary>
// FIFO
class FIFO : public ReplacementPolicy {
	int maxSize;
	int front;
	int rear;
	Elem** arr;

public:
	FIFO() {
		count = 0;
		maxSize = MAXSIZE + 1;
		rear = 0;
		front = 1;
		arr = new Elem * [maxSize];
	}

	~FIFO() {
		delete[]arr;
	}
	void insert(Elem* e);
	void access(int adr);
	Elem* remove();
	void print();
};

// MRU
class MRU : public ReplacementPolicy {
protected:
	ListNode* head;
	ListNode* tail;

public:
	MRU() {
		head = NULL;
		tail = NULL;
		count = 0;
	}
	~MRU() {
		while (head) {
			ListNode* temp = head;
			head = head->next;
			delete temp;
		}
	}
	void insert(Elem* e);
	void access(int adr);
	Elem* remove();
	void print();
};
// LRU
class LRU : public MRU {
public:
	Elem* remove() override;
};

// LFU
class LFU : public ReplacementPolicy {
	HeapNode* arr;

	// Methods 
	bool isLeaf(int pos);
	int leftChild(int pos);
	int rightChild(int pos);
	int parent(int pos);
	void swap(int a, int b);
	void heapDown(int pos);
	void heapUp(int pos);

public:
	LFU() {
		arr = new HeapNode[MAXSIZE];
	}
	~LFU() {
		delete[]arr;
	}
	void insert(Elem* e);
	void access(int adr);
	Elem* remove();
	void print();
};


/// <summary>
/// SE concrete classes
/// </summary>
// DBHashing
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
	void insert(Elem* ele);
	void deleteNode(int key);
	void print();
	Elem* search(int key);
};

// AVL
class AVL : public SearchEngine {
	AVLNode* root;

	// Implementations
	int getHeight(AVLNode* node);
	AVLNode* rightRotate(AVLNode* node);
	AVLNode* leftRotate(AVLNode* node);
	int getBalance(AVLNode* node);
	AVLNode* getMinNode(AVLNode* node);
	AVLNode* insert(AVLNode* node, Elem* ele);
	AVLNode* remove(AVLNode* node, int key);
	AVLNode* search(AVLNode* node, int key);
	void preOrder(AVLNode* node);
	void inOrder(AVLNode* node);
	void clearTree(AVLNode* node);

public:
	AVL() : root(NULL) {}
	~AVL() {
		clearTree(root);
	}

	Elem* search(int key);
	void insert(Elem* ele);
	void deleteNode(int key);
	void print();

};



#endif