#include <iostream>
#include "iml_parser.h"
#include "operation_on_list.h"
using namespace std;

void testTranslation();
void testMapOperations();
void testAggregateOperations();
void testSortingOperations();

int main() {
	testTranslation();
//	testMapOperations();
//	testAggregateOperations();
//	testSortingOperations();

	return 0;
}

void testTranslation() {
	IMLparser parser;
	parser.translate("InputFile.txt", "OutputFile.txt");
}

void testMapOperations() {
	OperationOnList list1("MAP-INC", "5.5");
	list1.addToList(5);
	list1.addToList(10);
	list1.addToList(15);
	list1.applyOperation();
	LinkedList<double> l = list1.getResultAsList();
	l.print();

	OperationOnList list2("MAP-MLT", "5");
	list2.addToList(1);
	list2.addToList(5);
	list2.addToList(10);
	list2.applyOperation();
	cout << list2.getResultAsString() << endl;
}

void testAggregateOperations() {
	OperationOnList list1("AGG-SUM");
	list1.addToList(5);
	list1.addToList(10);
	list1.addToList(15);
	list1.applyOperation();
	LinkedList<double> l = list1.getResultAsList();
	l.print();

	OperationOnList list2("AGG-PRO");
	list2.addToList(2);
	list2.addToList(3);
	list2.addToList(4);
	list2.addToList(5);
	list2.applyOperation();
	cout << list2.getResultAsString() << endl;

	OperationOnList list3("AGG-AVG");
	list3.addToList(2);
	list3.addToList(4);
	list3.addToList(6);
	list3.addToList(8);
	list3.applyOperation();
	cout << list3.getResultAsString() << endl;

	OperationOnList list4("AGG-FST");
	list4.addToList(2);
	list4.addToList(4);
	list4.addToList(6);
	list4.addToList(8);
	list4.applyOperation();
	cout << list4.getResultAsString() << endl;

	OperationOnList list5("AGG-LST");
	list5.addToList(2);
	list5.addToList(4);
	list5.addToList(6);
	list5.addToList(8);
	list5.applyOperation();
	cout << list5.getResultAsString() << endl;
}

void testSortingOperations() {
	OperationOnList list1("SRT-REV");
	list1.addToList(5);
	list1.addToList(10);
	list1.addToList(15);
	list1.addToList(20);
	list1.applyOperation();
	LinkedList<double> l = list1.getResultAsList();
	l.print();

	OperationOnList list2("SRT-ORD", "ASC");
	list2.addToList(8);
	list2.addToList(3.3);
	list2.addToList(6.75);
	list2.addToList(4.2);
	list2.addToList(1);
	list2.applyOperation();
	cout << list2.getResultAsString() << endl;

	OperationOnList list3("SRT-ORD", "DSC");
	list3.addToList(8);
	list3.addToList(3.3);
	list3.addToList(6.75);
	list3.addToList(4.2);
	list3.addToList(1);
	list3.applyOperation();
	cout << list3.getResultAsString() << endl;

	OperationOnList list4("SRT-SLC", "3");
	list4.addToList(1);
	list4.addToList(2);
	list4.addToList(3);
	list4.addToList(4);
	list4.addToList(5);
	list4.applyOperation();
	cout << list4.getResultAsString() << endl;

	OperationOnList list5("SRT-DST");
	list5.addToList(1);
	list5.addToList(5);
	list5.addToList(2);
	list5.addToList(2);
	list5.addToList(2);
	list5.addToList(5);
	list5.addToList(5);
	list5.addToList(5);
	list5.addToList(2);
	list5.addToList(3);
	list5.addToList(5);
	list5.applyOperation();
	cout << list5.getResultAsString() << endl;
}
