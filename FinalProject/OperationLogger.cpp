#include "OperationLogger.h"

#include <iostream>

OperationLogger::OperationLogger(bool enabled)
    : enabled(enabled) {
}

void OperationLogger::setEnabled(bool value) {
    enabled = value;
}

bool OperationLogger::isEnabled() const {
    return enabled;
}

void OperationLogger::clear() {
    messages.clear();
}

void OperationLogger::log(const std::string& message) {
    if (enabled) {
        messages.push_back(message);
    }
}

void OperationLogger::print() const {
    if (!enabled || messages.empty()) {
        return;
    }

    std::cout << "\nOperation log:\n";

    for (const std::string& message : messages) {
        std::cout << "- " << message << "\n";
    }

    std::cout << "\n";
}