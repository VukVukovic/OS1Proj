#ifndef LIST_H
#define LIST_H
#include "utils.h"
#include <iostream.h>

template<class T>
class List {
public:
	struct Elem {
		T data;
		Elem *prev, *next;
		Elem(T data) : data(data), prev(nullptr), next(nullptr) {}
	};

	int n;
	Elem *first, *last;
	List() : first(nullptr), last(nullptr), n(0) {}

	bool empty() { return n==0; }
	int size() { return n; }

	void pushBack(T data) {
		lock;
		Elem *newelem = new Elem(data);
		unlock;
		if (empty()) first = newelem;
		else { newelem->prev = last; last->next = newelem; }
		last = newelem;
		n++;
	}

	void pushFront(T data) {
		lock;
		Elem *newelem = new Elem(data);
		unlock;
		if (empty()) last = newelem;
		else { newelem->next = first; first->prev = newelem; }
		first = newelem;
		n++;
	}

	T popFront() {
		T ret = first->data;
		Elem *toDelete = first;
		first = first->next;
		n--;
		if (empty()) first = last = nullptr;
		else first->prev = nullptr;
		lock;
		delete toDelete;
		unlock;
		return ret;
	}

/*
	void printFwd() {
		for (Elem *p = first; p!=nullptr; p=p->next)
			cout << p->data << " ";
		cout << endl;
	}

	void printBck() {
		for (Elem *p = last; p!=nullptr; p=p->prev)
			cout << p->data << " ";
		cout << endl;
	}
 */
	virtual ~List() { 
		while (first != nullptr) {
			Elem *toDelete = first;
			first = first->next;
			lock;
			delete toDelete;
			unlock;
		}

		first = last = nullptr;
		n = 0;
	}

	class Iterator {
		public: 
		List<T>* list;
		List<T>::Elem *current, *nextDeleted;

		Iterator(List<T>* list, List<T>::Elem* current) : list(list), current(current), nextDeleted(nullptr) {}
		Iterator(List<T>* list) : list(list), current(list->first), nextDeleted(nullptr) {}
		Iterator() : list(nullptr), current(nullptr), nextDeleted(nullptr) {}
		
		bool exists() { return current!=nullptr; }
		T operator*() { return current->data; }
		void operator++() { 
			if (nextDeleted!=nullptr) current=nextDeleted;
			else if (current!=nullptr) current=current->next;
			nextDeleted = nullptr;
		}
		void operator++(int k) { ++(*this); }
		void remove() {
			if (current==nullptr) return;
			List<T>::Elem *prev = current->prev, *next = current->next, *toDelete = current;

			if (prev!=nullptr) prev->next = next;
			else list->first = next;

			if (next!=nullptr) next->prev = prev;
			else list->last = prev;

			nextDeleted = next;
			current = nullptr;
			lock;
			delete toDelete;
			unlock;
			list->n--;
		}
		void insertBefore(T data) {
			if (list->size() == 0) list->pushFront(data); // empty list
			else if (current == nullptr) list->pushBack(data); // end of list
			else if (current->prev == nullptr) list->pushFront(data); // before first
			else { //inside
				List<T>::Elem *newelem = new List<T>::Elem(data);
				newelem->next = current;
				newelem->prev = current->prev;
				current->prev->next = newelem;
				current->prev = newelem;
				list->n++;
			}
		}
	};

	Iterator begin() { return Iterator(this); }
}
#endif LIST_H