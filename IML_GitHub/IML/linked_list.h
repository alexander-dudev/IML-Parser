#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct LinkedListElement {
	T data;
	LinkedListElement* next;

	LinkedListElement(const T& data, LinkedListElement* next = nullptr) {
		this->data = data;
		this->next = next;
	}
};

template <typename T>
class LinkedListIterator;

template <typename T>
class LinkedList {
private:
	LinkedListElement<T>* front;
	LinkedListElement<T>* back;

	void copy(const LinkedList& l) {
		for (LinkedListIterator<T> it = l.begin(); it; it++)
			this->insertEnd(*it);
	}

	void clear() {
		while (!empty()) {
			T temporary;
			this->deleteEnd(temporary);
		}
	}

	LinkedListIterator<T> findPrevious(LinkedListIterator<T> it) {
		LinkedListIterator<T> previous = begin();
		while (previous && previous.pointer->next != it.pointer)
			previous++;
		return previous;
	}

public:

	LinkedList() : front(nullptr), back(nullptr) {}

	LinkedList(const LinkedList& l) : front(nullptr), back(nullptr) { copy(l); }

	LinkedList& operator= (const LinkedList& l) {
		if (this != &l) {
			clear();
			copy(l);
		}
		return *this;
	}

	~LinkedList() { clear(); }

	bool empty() const { return front == nullptr && back == nullptr; }

	bool insertAfter(const T& newData, LinkedListIterator<T> it) {
		// if the list is empty
		if (it.pointer == nullptr && empty()) {
			front = new LinkedListElement<T>(newData);
			back = front;
			return true;
		}
		else if (!it)
			return false;

		LinkedListElement<T>* newElement = new LinkedListElement<T>(newData, it.pointer->next);
		it.pointer->next = newElement;
		if (it == end())
			back = newElement;
		return true;
	}

	bool insertBefore(const T& newData, LinkedListIterator<T> it) {
		// taking care of the cases in which:
		// 1) it is an iterator pointing to the first element in the list
		// 2) when the list is empty and it is a nullpointer iterator
		if (it == begin()) {
			LinkedListElement<T>* newElement = new LinkedListElement<T>(newData, front);
			if (empty())
				back = newElement;
			front = newElement;
			return true;
		}

		if (!it || empty())
			return false;

		// the iterator is valid and the list is not empty
		// the iterator is not poiting to the first element
		// the check for validity will be delegated to insertAfter

		return insertAfter(newData, findPrevious(it));
	}

	bool deleteAfter(T& x, LinkedListIterator<T> it) {
		if (!it || it == end() || empty())
			return false;

		// it is a valid iterator, it is not pointing to the last element
		// and there are at least 2 elements in the list

		LinkedListElement<T>* toDelete = it.pointer->next;
		it.pointer->next = it.pointer->next->next;
		x = toDelete->data;
		if (toDelete->next == nullptr)
			back = it.pointer;
		delete toDelete;
		return true;
	}

	bool deleteAt(T& x, LinkedListIterator<T> it) {
		if (!it || empty())
			return false;

		// I have to take care of the case in which it == begin()
		// the other cases will be taken care of by deleteAfter

		if (it == begin()) {
			LinkedListElement<T>* toDelete = front;
			front = front->next;
			if (front == nullptr)
				back = nullptr;
			x = toDelete->data;
			delete toDelete;
			return true;
		}

		return deleteAfter(x, findPrevious(it));
	}

	bool deleteBefore(T& x, LinkedListIterator<T> it) {
		if (!it || it == begin() || empty())
			return false;

		// it is a valid iterator
		// it is not pointing to the first element in the list
		// there are at least 2 elements in the list

		deleteAt(x, findPrevious(it));
	}

	T& getAt(LinkedListIterator<T> it) const { return it.get(); }

	void insertBegin(const T& x) { insertBefore(x, begin()); }

	void insertEnd(const T& x) { insertAfter(x, end()); }

	bool deleteBegin(T& x) { return deleteAt(x, begin()); }

	bool deleteEnd(T& x) { return deleteAt(x, end()); }

	LinkedListIterator<T> begin() const { return LinkedListIterator<T>(front); }

	LinkedListIterator<T> end() const { return LinkedListIterator<T>(back); }

	LinkedList& operator += (const T& newData) {
		insertEnd(newData);
		return *this;
	}

	void print(ostream& out = cout) const {
		out << '(';
		for (LinkedListIterator<T> it = begin(); it; it++) {
			out << *it;
			if (it != end())
				out << ',';
		}
		out << ')' << endl;
	}

	// appends the list sent as parameter to *this and turns the parameter into an empty list
	void append(LinkedList& other) {

		if (!empty()) {
			back->next = other.front;
			if (!other.empty())
				back = other.back;
		}
		else {
			front = other.front;
			back = other.back;
		}

		other.front = nullptr;
		other.back = nullptr;
	}

	int length() const {
		int length = 0;
		for (LinkedListIterator<T> it = begin(); it; it++)
			length++;
		return length;
	}

	void reverse() {
		if (empty() || front->next == nullptr)
			return;

		// there are at least 2 elements in the list
		LinkedListElement<T>* previous = front;
		LinkedListElement<T>* following = front->next;
		back = front;
		back->next = nullptr;

		while (following != nullptr) {
			front = following;
			following = following->next;
			front->next = previous;
			previous = front;
		}
	}

	// removes duplicates and turns the list into a set
	void unique() {
		T x;
		for (LinkedListIterator<T> it1 = begin(); it1; it1++) {
			for (LinkedListIterator<T> it2 = it1; it2.next(); ) {
				if (*it1 == *(it2.next()))
					deleteAfter(x, it2);
				else
					it2++;
			}
		}
	}
};

template <typename T>
void append(LinkedList<T>& ls1, const LinkedList<T>& ls2) {
	for (LinkedListIterator<T> it = ls2.begin(); it; it++) {
		ls1 += *it;
	}
}

template <typename T>
void reverse(LinkedList<T>& ls) {
	LinkedListIterator<T> it = ls.begin();
	T x;
	while (ls.deleteAfter(x, it))
		ls.insertBegin(x);
}

template <typename T>
void split(const LinkedList<T>& ls, LinkedList<T>& ls1, LinkedList<T>& ls2) {
	LinkedList<T>* addNow = &ls1;
	LinkedList<T>* addLater = &ls2;
	for (LinkedListIterator<T> it = ls.begin(); it; it++) {
		(*addNow).insertEnd(*it);
		swap(addNow, addLater);
	}
}

template <typename T>
LinkedList<T> merge(const LinkedList<T>& ls1, const LinkedList<T>& ls2) {
	LinkedList<T> sorted;
	LinkedListIterator<T> it1 = ls1.begin(), it2 = ls2.begin();

	while (it1 && it2) {
		if (*it1 <= *it2) {
			sorted += *it1;
			it1++;
		}
		else {
			sorted += *it2;
			it2++;
		}
	}

	while (it1) {
		sorted += *it1;
		it1++;
	}
	while (it2) {
		sorted += *it2;
		it2++;
	}
	return sorted;
}

template <typename T>
void mergeSort(LinkedList<T>& ls) {
	// if the list is empty or contains only 1 element then it is sorted
	if (ls.begin() == ls.end())
		return;

	LinkedList<T> ls1, ls2;
	split(ls, ls1, ls2);

	mergeSort(ls1);
	mergeSort(ls2);

	ls = merge(ls1, ls2);
}

template <typename T>
class LinkedListIterator {
private:

	LinkedListElement<T>* pointer;
	// static T error;

public:

	friend class LinkedList<T>;

	// construction from a pointer
	LinkedListIterator(LinkedListElement<T>* p = nullptr) : pointer(p) {}

	// next position
	LinkedListIterator next() const {
		// assuming the iterator is valid
		// if (!valid())
		//  return *this;
		return LinkedListIterator(pointer->next);
	}

	// previous position
	LinkedListIterator prev() const;

	// access to an element with a possibility of changing it
	T& get() const {
		// assuming the iterator is valid
		// if (!valid())
		//  return error;
		return pointer->data;
	}

	// access to an element without a possibility of changing it
	const T& getConst() const {
		// assuming the iterator is valid
		// if (!valid())
		//  return error;
		return pointer->data;
	}

	// check for validity
	bool valid() const { return pointer != nullptr; }

	// comparison of two iterators
	bool operator==(const LinkedListIterator& it) const { return pointer == it.pointer; }

	bool operator!=(const LinkedListIterator& it) const { return !(*this == it);  }

	// syntactic sugar

	// *it <-> it.get()
	T& operator*() const { return get(); }

	// it++ <-> it = it.next(), returns the old value of it
	LinkedListIterator operator++(int) {
		LinkedListIterator old = *this;
		*this = next();
		return old;
	}

	// ++it <-> it = it.next(), returns the new value of it
	LinkedListIterator& operator++() {
		*this = next();
		return *this;
	}

	// it <-> it.valid()
	operator bool() const { return valid(); }
};