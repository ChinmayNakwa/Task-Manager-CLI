#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <string>
#include <vector>

class ApiClient {
public:
    std::vector<std::string> generateCommands(const std::string& goal);
};

#endif