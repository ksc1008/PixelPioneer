#pragma once
#include<iostream>
#include<string>

#define DEBUG

#ifdef DEBUG
#define writeLine(x) \
            _wl(__LINE__, __FILE__, __FUNCTION__, x)
#else
#define writeLine(x) nothing()
#endif

class Debugger {
public:
	static Debugger* getInstance() { return &debuggerInstance; }
	void _wl(int line, const char* file, const char* function, std::string s);
	void _wl(int line, const char* file, const char* function, int integer);
    void _nothing(){}

private:
	static Debugger debuggerInstance;
};
