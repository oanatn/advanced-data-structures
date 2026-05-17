#ifndef OPERATION_LOGGER_H
#define OPERATION_LOGGER_H

#include <string>
#include <vector>

class OperationLogger {
private:
    bool enabled;
    std::vector<std::string> messages;

public:
    explicit OperationLogger(bool enabled = false);

    void setEnabled(bool value);
    bool isEnabled() const;

    void clear();
    void log(const std::string& message);
    void print() const;
};

#endif