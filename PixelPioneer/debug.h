#pragma once
#include<iostream>

#ifdef _DEBUG
#define writeLine( ... ) \
            _wl(__LINE__, __FILE__, __FUNCTION__, ##__VA_ARGS__ )
#else
#define writeLine(x) _nothing()
#endif

class Debugger {
    static Debugger debuggerInstance;

    template<typename T>
    void _wl_recursive(int line, const char* file, const char* function, T t) {
        std::cout << t;
        std::cout << "\n    by " << function << " in " << file << ":" << line << "\n";
    }

    template<typename T, typename ...Args>
    void _wl_recursive(int line, const char* file, const char* function, T t, Args ...args) {
        std::cout << t;
        _wl_recursive(line, file, function, args...);
    }
public:
	static Debugger* getInstance() { return &debuggerInstance; }


    template<typename T, typename... Args>
    void _wl(int line, const char* file, const char* function, std::initializer_list<T> list)
    {
        std::cout << "debug: ";
        for (auto elem : list)
        {
            std::cout << elem;
        }
        std::cout << "\n    by " << function << " in " << file << ":" << line << "\n";
    }

    template<typename T, typename ...Args>
    void _wl(int line, const char* file, const char* function, T t, Args ...args)
    {
        std::cout << "debug: ";
        std::cout << t;
        _wl_recursive(line, file, function, args...);
    }

    template<typename T>
    void _wl(int line, const char* file, const char* function, T t)
    {
        std::cout << "debug: " << t << "\n    by " << function << " in " << file << ":" << line << "\n";
    }
    void _nothing(){}
};
