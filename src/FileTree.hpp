#ifndef FILE_TREE_HPP
#define FILE_TREE_HPP

#include <vector>
#include <string>

std::vector<std::string> get_subpaths(const std::string &root_path);
void get_nested_files(std::vector<std::string> &paths, std::string root_path);

#endif
