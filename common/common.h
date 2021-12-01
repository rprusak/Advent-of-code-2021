#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>

namespace common {
std::vector<std::string> readInput(const std::string& filename);

std::vector<std::string> split(const std::string& str, const std::string& delim);

std::string trim(const std::string& str);
}  // namespace common

#endif  // COMMON_H
