#pragma once
#include<iostream>

class Debugger {
public:
	static Debugger* getInstance() { return &debuggerInstance; }
	void writeLine(std::string s);

private:
	static Debugger debuggerInstance;
};
