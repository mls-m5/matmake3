#pragma once

#include <iostream>

class Log {
public:
    template <typename... Args>
    void print(std::ostream &target, const Args &...args) {
        ((target << args << " "), ...);
        target << "\n";
    }

    template <typename... Args>
    void debug(const Args &...args) {
        if (_enableDebugOutput) {
            print(_debugout, args...);
        }
    }

    void enableDebugOutput(bool value) {
        _enableDebugOutput = value;
    }

private:
    std::ostream &_debugout = std::cout;
    bool _enableDebugOutput = false;
};

inline Log mlog;
