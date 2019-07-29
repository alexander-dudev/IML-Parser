#include "operation_on_list.h"
#include <string>
#include <sstream>

OperationOnList::OperationOnList() {
	operationHasBeenApplied = false;
}

OperationOnList::OperationOnList(string operation, string attribute) {
	operationHasBeenApplied = false;
	this->operation.setOperation(operation);
	if (attribute.length() > 0) {
		this->operation.setAttribute(attribute);
		this->operation.setExpectationOfAttribute(true);
	}
}

void OperationOnList::addToList(double data) { 
	list.insertEnd(data);
}

void OperationOnList::appendList(LinkedList<double>& ls) {
	list.append(ls);
}

void OperationOnList::applyOperation() {
	if (operationHasBeenApplied)
		return;

	string operationToApply = (operation.getOperation()).substr(0, 3);
	if (operationToApply == "MAP")
		applyMapOperation();
	if (operationToApply == "AGG")
		applyAggregateOperation();
	if (operationToApply == "SRT")
		applySortingOperation();

	operationHasBeenApplied = true;
}

void OperationOnList::applyMapOperation() {
	string specificOperation = (operation.getOperation()).substr(4, 3);
	double(*f)(double, double);
	specificOperation == "INC" ? f = add : f = multiply;

	stringstream ss;
	ss << operation.getAttribute();
	double parameter;
	ss >> parameter;

	for (LinkedListIterator<double> it = list.begin(); it; it++) {
		*it = f(*it, parameter);
	}
}

void OperationOnList::applyAggregateOperation() {
	string specificOperation = operation.getOperation().substr(4, 3);
	double x;

	if (specificOperation == "FST") {
		while (list.begin() != list.end())
			list.deleteEnd(x);
		return;
	}

	if (specificOperation == "LST") {
		while (list.begin() != list.end())
			list.deleteBegin(x);
		return;
	}

	// the operation is sum, product or average 
	double(*f)(double, double);
	LinkedList<double> result;
	if (specificOperation == "PRO") {
		f = multiply;
		x = 1;
	}
	else {
		f = add;
		x = 0;
	}
	
	for (LinkedListIterator<double> it = list.begin(); it; it++)
		x = f(x, *it);

	if (specificOperation == "AVG") {
		x /= list.length();
	}

	result.insertEnd(x);
	list = result;
}

void OperationOnList::applySortingOperation() {
	string specificOperation = operation.getOperation().substr(4, 3);
	if (specificOperation == "DST") {
		list.unique();
		return;
	}

	if (specificOperation == "ORD") {
		mergeSort(list);
	}

	if (specificOperation == "REV" || operation.getAttribute() == "DSC")
		list.reverse();

	if (specificOperation != "SLC")
		return;

	stringstream ss;
	double x;
	int index;
	ss << operation.getAttribute();
	ss >> index;
	for (int i = 0; i < index ; i++)
		list.deleteBegin(x);
}

LinkedList<double> OperationOnList::getResultAsList() {
	return list;
}

string OperationOnList::getResultAsString() {
	string result;
	stringstream os;
	for (LinkedListIterator<double> it = list.begin(); it; it++) {
		os << *it;
		if (it != list.end())
			os << " ";
	}
	result = os.str();
	return result;
}

double add(double x, double y) {
	return x + y;
}

double multiply(double x, double y) {
	return x * y;
}