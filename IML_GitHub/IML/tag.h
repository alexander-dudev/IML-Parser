#pragma once
using namespace std;

class Tag {
private: 
	string operation;
	string attribute;
	bool expectsAttribute;

public:
	Tag(string operation = "", string attribute = "", bool expectsAttribute = false) {
		this->operation = operation;
		this->attribute = attribute;
		this->expectsAttribute = expectsAttribute;
	}

	string getOperation() const { return operation; }
	string getAttribute() const { return attribute; }
	bool getExpectationOfAttribute() const { return expectsAttribute; }

	void setOperation(string newOperation) { operation = newOperation; }
	void setAttribute(string newAttribute) { attribute = newAttribute; }
	void setExpectationOfAttribute(bool newValue) { expectsAttribute = newValue; }
};
