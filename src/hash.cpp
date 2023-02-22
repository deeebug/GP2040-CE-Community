#include <map>
#include <string>

size_t hash(std::string data) {
    return std::hash<std::string>{}(data);
}