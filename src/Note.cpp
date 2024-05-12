#include "Note.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std;

Note::Note(const string& heading, const string& content)
    : heading(heading), content(content) {
    time(&modificationTimestamp);
}
