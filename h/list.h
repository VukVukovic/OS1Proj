#ifndef LIST_H
#define LIST_H
#include "utils.h"
#include <iostream.h>

template<class T>
class List {
public:
	struct Elem {
		T data;
		Elem *next, *prev;
		Elem(T data) {
			this->data = data;
			next = prev = nullptr;
		}
	};

	Elem *first, *last;
	int n;

	List() {
		first = last = nullptr;
		n = 0;
	}

	int size() const { return n; }
	bool empty() const { return (n == 0); }

	void pushBack(T data);
	void pushFront(T data);

	void copy(const List& list);
	void erase();

	List(const List& list) { copy(list); }
	List& operator=(const List& list) { if (this != &list) { erase(); copy(list); } return *this; }
	virtual ~List() { erase(); }

 	class Iterator {
		List *list;
		Elem *current, *deleted_next;
	public:
		Iterator(List *list, Elem *current) {
			this->list = list;
			this->current = current;
			deleted_next = nullptr;
		}
		T operator*() const { return current->data; }
		void operator++() { current = ((current==nullptr)?deleted_next:current->next); deleted_next = nullptr; }
		void operator++(int k) { ++(*this); }
		bool exists() const { return current != nullptr; }
		void remove() {
			if (current == nullptr) return;
			Elem *prev = current->prev;
			Elem *next = current->next;
			deleted_next = next;

			((prev!=nullptr)?prev->next:list->first) = next;
			((next!=nullptr)?next->prev:list->last) = prev;

			lock;
			delete current;
			unlock;
			current = nullptr;
			list->n--;
		}
	};

	Iterator begin() { return Iterator(this, first); }
};

template<class T>
void List<T>::copy(const List<T> & list)
{
	for (Elem *it = list.first; it != nullptr; it = it->next)
		pushBack(it->data);
}

template<class T>
void List<T>::erase()
{
	Elem *it = first;
	while (it != nullptr) {
		Elem *it_next = it->next;
		lock;
		delete it;
		unlock;
		it = it_next;
	}

	first = last = nullptr;
	n = 0;
}

template<class T>
void List<T>::pushBack(T data)
{
	lock;
	Elem *newelem = new Elem(data);
	unlock;
	newelem->prev = last;
	(empty()?first:last->next) = newelem;
	last = newelem;
	n++;
}

template<class T>
void List<T>::pushFront(T data)
{
	lock;
	Elem *newelem = new Elem(data);
	unlock;
	newelem->next = first;
	(empty()?last:first->prev) = newelem;
	first = newelem;
	n++;
}
#endif