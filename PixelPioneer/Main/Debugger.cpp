#include "debug.h"

Debugger Debugger::debuggerInstance;

void Debugger::writeLine(std::string s) {
	std::cout << s << std::endl;
}