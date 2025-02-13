#pragma once
#include <iostream>
#include <string>
#include <stdexcept>

class ErrorHandler {
public:
    static void logError(const std::string& message);
    static void throwError(const std::string& message);
};
