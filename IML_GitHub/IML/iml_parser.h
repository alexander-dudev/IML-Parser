#pragma once
#include <string>
#include "linked_list.h"
#include "tag.h"
using namespace std;

class IMLparser {
private:
	LinkedList<string> file;
	LinkedList<Tag> knownTags;

	// checks whether a tag is valid
	bool isValidTag(const Tag& tag);

	// checks whether the tags in a line are correctly nested and whether all tags are from the IML
	bool isValidExpression (const LinkedListIterator<string>& it);

	// reads the operation that begins at (*it)[lineIndex] and retuns it
	string readOpeningTagOperation(const LinkedListIterator<string>& it, size_t& lineIndex);

	// reads the operation that begins at (*it)[lineIndex] and retuns it
	string readClosingTagOperation(const LinkedListIterator<string>& it, size_t& lineIndex);

	// reads the attribute that begins at (*it)[lineIndex] and retuns it
	string readAttribute(const LinkedListIterator<string>& it, size_t& lineIndex);

	// reads a floating point number that begins at (*it)[lineIndex] and retuns it
	double readNumber(const LinkedListIterator<string>& it, size_t& lineIndex);

	// checks for validity, processes and returns the text that has to be written
	string processExpression(const LinkedListIterator<string>& it);

	// parses a valid expression and returns the resuling list as a string
	string parse(const LinkedListIterator<string>& it);

	// reads the input file and stores its content in memory
	bool readFile(const char* inputFile);

public:

	IMLparser();

	// reads the input file and writes the transformed versions of the valid expressions in it in output file
	void translate(const char* inputFile, const char* outputFile);

};
