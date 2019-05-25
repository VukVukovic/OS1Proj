#ifndef LIST_H
#define LIST_H

#include "Util.h"

template<class T>
class List {
protected:
	struct Node{
		T data;
		Node* next;

		Node(T data, Node *next=nullptr) : data(data), next(next) {}
	};

	Node *first, *last, *curr;
	int len;
	
public:
	List() : first(nullptr), last(nullptr), curr(nullptr), len(0) {}
	void add(T data) {
		Node *newnode = new Node(data, first);
		first = newnode;
		if (len==0) last = newnode;
		len++;
	}

	void append(T data) {
		Node *newnode = new Node(data);
		if (len==0) first = newnode;
		else last->next = newnode;
		last = newnode;
		len++;
	}

	void begin() { curr = first; }
	bool exists() { return curr!=nullptr; }
	void next() { if (exists()) curr = curr->next; }
	T& get() { return curr->data; }
	int size() { return len; }

};
#endif
