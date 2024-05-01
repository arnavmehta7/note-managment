#pragma once
#include "Folder.hpp"
#include <vector>

class SearchEngine {
public:
    std::vector<Note*> search(const Folder& folder, const std::string& query) const;
};