#pragma once

#include <string>

class InputBuffer {
public:
	void GetChar(char&);
	char UngetChar(char);
	std::string UngetString(std::string);
	bool EndOfInput();

private:
	std::vector<char> input_buffer;
};
