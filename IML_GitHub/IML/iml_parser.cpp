#include "iml_parser.h"
#include "operation_on_list.h"
#include "linked_stack.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool IMLparser::isValidExpression(const LinkedListIterator<string>& it) {
	LinkedStack<Tag> stack;
	string operation;
	string attribute;

	for (size_t i = 0; i < (*it).length(); i++) {

		// if a new tag is about to be read
		if ((*it)[i] == '<' && (*it)[i + 1] != '/') {
			operation = readOpeningTagOperation(it, i);

			// if there is an attribute that has to be read
			if ((*it)[i] != '>') {
				attribute = readAttribute(it, i);
				Tag openingTag(operation, attribute, true);
				if (!isValidTag(openingTag))
					return false;
				stack.push(openingTag);
				i++; // getting after the closing quotation mark
				// no need to get after the '>'
				// i will be incremented once at the end of the iteration
//				attribute = "";
			}
			else {
				// the read tag doesn't have an attribute
				Tag openingTag(operation);
				if (!isValidTag(openingTag))
					return false;
				stack.push(openingTag);
				// no need to get after the '>'
				// i will be incremented once at the end of the iteration
			}
//			operation = "";
		}

		// if a closing tag is about to be read
		if ((*it)[i] == '<' && (*it)[i + 1] == '/') {
			if (stack.empty())
				return false;
			operation = readClosingTagOperation(it, i);
			Tag closingTag(operation);
			Tag corresponding = stack.pop();
			if (closingTag.getOperation() != corresponding.getOperation())
				return false;
			// no need to get after the '>'
			// i will be incremented once at the end of the iteration
//			operation = "";
		}
	}
	return stack.empty();
}

string IMLparser::parse(const LinkedListIterator<string>& it) {
	LinkedStack<OperationOnList> stack;
	string operation;
	string attribute;
	double number;

	for (size_t i = 0; i < (*it).length(); i++) {

		// an opening tag is about to be read
		if ((*it)[i] == '<' && (*it)[i + 1] != '/') {
			operation = readOpeningTagOperation(it, i);

			// there is an attribute that has to be read
			if ((*it)[i] != '>') {
				attribute = readAttribute(it, i);
				OperationOnList newList(operation, attribute);
				stack.push(newList);
				i++; // getting after the closing quotation mark
					 // no need to get after the '>'
					 // i will be incremented once at the end of the iteration
//				attribute = "";
			}
			else {
				// the read tag doesn't have an attribute
				OperationOnList newList(operation);
				stack.push(newList);
				// no need to get after the '>'
				// i will be incremented once at the end of the iteration
			}
//			operation = "";
		}

		// if a number is about to be read
		if ((*it)[i] == '-' || '0' <= (*it)[i] && (*it)[i] <= '9') {
			number = readNumber(it, i);
			stack.peek().addToList(number);
		}

		// if a closing tag is about to be read
		if ((*it)[i] == '<' && (*it)[i+1] == '/') {
			// the expression is valid so the operation that
			// has to be performed is on the top of the stack
			// reading the operation only to move i on '>'
			operation = readClosingTagOperation(it, i);
			OperationOnList currentOperation = stack.pop();
			currentOperation.applyOperation();
			if (stack.empty())
				return currentOperation.getResultAsString();
			else {
				stack.peek().appendList(currentOperation.getResultAsList());
			}
		}
	}
}

double IMLparser::readNumber(const LinkedListIterator<string>& it, size_t& lineIndex) {
	stringstream ss;
	double num;
	
	while ((*it)[lineIndex] != ' ' && (*it)[lineIndex] != '<') {
		ss << (*it)[lineIndex];
		lineIndex++;
	}

	ss >> num;

	// i is positioned either on an interval or on a '<'
	// if it is positioned on '<' there will be a problem
	// because at the end of the loop i will be incremented once
	// and the following tag won't be read
	if ((*it)[lineIndex] == '<')
		lineIndex--;

	return num;
}

string IMLparser::readOpeningTagOperation(const LinkedListIterator<string>& it, size_t& lineIndex) {
	string operation;
	lineIndex++; // getting after the '<'
	while ((*it)[lineIndex] != ' ' && (*it)[lineIndex] != '>') {
		operation += (*it)[lineIndex];
		lineIndex++;
	}
	return operation;
}

string IMLparser::readClosingTagOperation(const LinkedListIterator<string>& it, size_t& lineIndex) {
	string operation;
	lineIndex++; // getting after '<'
	lineIndex++; // getting after '/'
	while ((*it)[lineIndex] != '>') {
		operation += (*it)[lineIndex];
		lineIndex++;
	}
	return operation;
}

string IMLparser::readAttribute(const LinkedListIterator<string>& it, size_t& lineIndex) {
	string attribute;
	lineIndex++; // getting after the interval
	lineIndex++; // getting afther the first quotation mark
	while ((*it)[lineIndex] != '\"') {
		attribute += (*it)[lineIndex];
		lineIndex++;
	}
	return attribute;
}

bool IMLparser::isValidTag(const Tag& openingTag) {
	for (LinkedListIterator<Tag> it = knownTags.begin(); it; ++it) {
		if (openingTag.getOperation() == (*it).getOperation()
			&& openingTag.getExpectationOfAttribute() == (*it).getExpectationOfAttribute())
			return true;
	}
	return false;
}

string IMLparser::processExpression(const LinkedListIterator<string>& it) {
	string translatedLine;
	if (!isValidExpression(it))
		translatedLine = "The input line is invalid and cannot be translated!";
	else
		translatedLine = parse (it);

	return translatedLine;
}

bool IMLparser::readFile(const char* inputFile) {
	ifstream fileStream(inputFile, ios::in);

	if (!fileStream.is_open()) {
		cerr << "An error occured when trying to open the file for reading!\n";
		return false;
	}
	
	string line;
	while (getline(fileStream, line)) {
		this->file.insertEnd(line);
	}

	fileStream.close();
	return true;
}

void IMLparser::translate(const char* inputFile, const char* outputFile) {
	if (!readFile(inputFile))
		return;

	string outputLine;
	ofstream fileStream(outputFile, ios::out | ios::trunc);
	if (!fileStream.is_open()) {
		cerr << "An error occured when trying to open the file for writing!\n";
		return;
	}

	for (LinkedListIterator<string> it = file.begin(); it; ++it) {
		outputLine = processExpression(it);
		fileStream << outputLine << '\n';
	}

	fileStream.close();
}

IMLparser::IMLparser() {
	int numberOfTags;
	string operation;
	bool expectationOfAttribute;

	ifstream fileStream("Tags.txt", ios::in);
	if (!fileStream.is_open())
		cerr << "An error occured when trying to open the file containing IML tags for reading!\n";
	else {
		fileStream >> numberOfTags;
		for (int i = 0; i < numberOfTags; i++) {
			fileStream >> operation >> expectationOfAttribute;
			knownTags.insertEnd(Tag(operation, "", expectationOfAttribute));
		}
		fileStream.close();
	}
}