#pragma once

template <typename T>
struct StackElement {
	T data;
	StackElement* previous;
};

template <typename T>
class LinkedStack {
private:
	StackElement<T>* top;

	// copies a stack whose element on top is *toCopy
	void copy(StackElement<T>* toCopy);
	void eraseStack();

public:

	LinkedStack();

	LinkedStack(const LinkedStack&);

	LinkedStack& operator= (const LinkedStack&);

	~LinkedStack();

	void push(const T&);

	T pop();

	T& peek() const;

	bool empty() const;

	LinkedStack<T>& operator+= (const T& newData);
};

template <typename T>
LinkedStack<T>::LinkedStack() : top(nullptr) {}

template <typename T>
bool LinkedStack<T>::empty() const { return top == nullptr; }

template <typename T>
void LinkedStack<T>::push(const T& newData) {
	StackElement<T>* newElement = new StackElement<T>;
	(*newElement).data = newData;
	newElement->previous = top;
	top = newElement;
}

template <typename T>
T LinkedStack<T>::pop() {
	if (empty()) {
		cout << "Warning! The stack is already empty!\n";
		return T();
	}
	else {
		StackElement<T>* temporary = top;
		top = temporary->previous;

		T returnedData = temporary->data;
		delete temporary;
		return returnedData;
	}
}

template <typename T>
T& LinkedStack<T>::peek() const {
	if (empty()) {
		cout << "Warning! The stack is already empty!\n";
		return T();
	}

	return top->data;
}

template <typename T>
void LinkedStack<T>::eraseStack() {
	while (!empty())
		pop();
}

template <typename T>
LinkedStack<T>::~LinkedStack() {
	eraseStack();
}

template <typename T>
void LinkedStack<T>::copy(StackElement<T>* toCopy) {

	// base case
	if (toCopy == nullptr)
		return;

	// first, we copy all elements pushed before *toCopy
	copy(toCopy->previous);

	// finally, we push the last element from the stack
	push(toCopy->data);
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator= (const LinkedStack<T>& ls) {
	if (this != &ls) {
		eraseStack();
		copy(ls.top);
	}
	return *this;
}

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T>& ls) : top(nullptr) {
	copy(ls.top);
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator+= (const T& newData) {
	push(newData);
	return *this;
}