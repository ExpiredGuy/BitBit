#include "error_handler.h"

void ErrorHandler::logError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

void ErrorHandler::throwError(const std::string& message) {
    throw std::runtime_error(message);
}
