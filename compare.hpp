#ifndef compare_hpp
#define compare_hpp


#include <filesystem>
#include <fstream>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>


namespace fs = std::filesystem;


class CompareFiles {
public:
    CompareFiles(const fs::path&, const fs::path&);
    void show_stats() const;

private:
    fs::path p1;
    fs::path p2;
    std::string common;

    void compare();
};


std::vector<fs::path> get_paths(const std::string&);
std::vector<CompareFiles> get_pairs(const std::vector<fs::path>&);


#endif /* compare_hpp */
