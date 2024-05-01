#include "Note.hpp"
#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std;

Note::Note(const string& heading, const string& content)
    : heading(heading), content(content) {
    time(&modificationTimestamp);
}

void Note::save() const {
    filesystem::create_directories("notes"); // Ensure the directory exists
    string filename = "notes/" + heading + ".txt";
    ofstream file(filename);
    if (!file) {
        cerr << "Error opening file for note: " << heading << endl;
        return;
    }
    file << "Heading: " << heading << "\n";
    file << "Last Modified: " << ctime(&modificationTimestamp);
    file << "Content:\n" << content;
}