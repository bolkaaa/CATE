#include <vector>

#include "FileTree.hpp"

#include <boost/filesystem.hpp>

std::vector<std::string> get_subpaths(const std::string &root_path)
/* Get vector of filenames containing each subpath within <root_path>. */
{
    std::vector<boost::filesystem::directory_entry> entries;
    std::vector<std::string> sub_paths;

    std::copy(boost::filesystem::directory_iterator(root_path),
              boost::filesystem::directory_iterator(),
              std::back_inserter(entries));

    for (auto entry : entries)
    {
        sub_paths.push_back(entry.path().string());
    }

    return sub_paths;
}

void get_nested_files(std::vector<std::string> &paths, std::string root_path)
/* Recursively get all nested files from a root directory. */
{
    std::vector<std::string> sub_paths = get_subpaths(root_path);

    for (const auto &path : sub_paths)
    {
        if (boost::filesystem::is_directory(path))
        {
            get_nested_files(paths, path);
        }
        else
        {
            paths.push_back(path);
        }
    }
}
