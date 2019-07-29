#pragma once
#include "linked_list.h"
#include "tag.h"

class OperationOnList {
private:
	LinkedList<double> list;
	Tag operation;
	bool operationHasBeenApplied;

	void applyMapOperation();
	void applyAggregateOperation();
	void applySortingOperation();

public:

	OperationOnList();

	OperationOnList(string operation, string attribute = "");

	// inserts data at the end of the list
	void addToList(double data);

	// appends the given list at the end of the list that is a member variable
	void appendList(LinkedList<double>&);

	// applies the operation on the list if still not applied
	void applyOperation();

	// returns the resulting list
	LinkedList<double> getResultAsList();

	// returns the resulting list as a string
	string getResultAsString();
};

double add(double, double);
double multiply(double, double);
