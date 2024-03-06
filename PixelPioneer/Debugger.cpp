#include "debug.h"

Debugger Debugger::debuggerInstance;

void Debugger::_wl(int line, const char* file, const char* function, std::string s) {
	std::cout << "debug: " << s << "\nby " << function << " in " << file << ":" << line << "\n\n";
}
void Debugger::_wl(int line, const char* file, const char* function, int integer) {
	std::cout << "debug: " << integer << "\nby " << function << " in " << file << ":" << line << "\n\n";
}